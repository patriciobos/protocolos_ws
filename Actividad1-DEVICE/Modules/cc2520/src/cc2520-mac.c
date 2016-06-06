/*
 * cc2520-mac.c
 *
 *  Created on: 03/02/2012
 *      Author: pablo
 */

#include "includes.h"

u8 rxbuf[128], txbuf[128];

u8 ccChannelSelect(u8 chan_num)
{
	if(chan_num > 26) return -1;
	if(chan_num < 11) return -1;

	chan_num = 11+5*(chan_num-11);

	return ccRegWr(FREQCTRL, 1, &chan_num);

}

u8 ccSetLocalShortAddr(u16 addr)
{
	return ccCmd(MEMWR, RAM_SHORTADR, 2, &addr);
}

u16 ccGetLocalShortAddr(void)
{
	u16 r;
	ccCmd(MEMRD, RAM_SHORTADR, 2, &r);
	return r;
}

u8 ccSetLocalPANID(u16 panid)
{
	return ccCmd(MEMWR, RAM_PANID, 2, &panid);
}

u16 ccGetLocalPANID(void)
{
	u16 r;
	ccCmd(MEMRD, RAM_PANID, 2, &r);
	return r;
}

u8 ccSetLocalExtAddr(u64 addr)
{
	return ccCmd(MEMWR, RAM_IEEEADR, 8, &addr);
}

u64 ccGetLocalExtAddr(void)
{
	u64 r;
	ccCmd(MEMRD, RAM_IEEEADR, 8, &r);
	return r;
}

u8 ccSetSrcShortAddr(u8 n, u32 panid_addr)
{
	if(n>23) return -1;
	return ccCmd(MEMWR, SRC_MATCH_TABLE+4*n, 4, &panid_addr);
}

u32 ccGetSrcShortAddr(u8 n)
{
	u32 r;
	if(n>23) return -1;
	ccCmd(MEMRD, SRC_MATCH_TABLE+4*n, 4, &r);
	return r;
}

u8 ccSetSrcExtAddr(u8 n, u64 addr)
{
	if(n>11) return -1;
	return ccCmd(MEMWR, SRC_MATCH_TABLE+8*n, 8, &addr);
}

u64 ccGetSrcExtAddr(u8 n)
{
	u32 r;
	if(n>11) return -1;
	ccCmd(MEMRD, SRC_MATCH_TABLE+8*n, 8, &r);
	return r;
}

u16 ccWrapperFCF(macFrameType_e frameType, u8 securityEnabled, u8 framePending, u8 ackRequested,
		u8 PANIDcompression, macAddrMode_e destAddrMode, u8 frameVersion, macAddrMode_e sourceAddrMode)
{
	u16 fcf = 0;

	fcf |= frameType & 0x7;
	fcf |= (securityEnabled & 1) << 3;
	fcf |= (framePending & 1) << 4;
	fcf |= (ackRequested & 1) << 5;
	fcf |= (PANIDcompression & 1) << 6;
	fcf |= (destAddrMode & 0x3) << 10;
	fcf |= (frameVersion & 0x3) << 12;
	fcf |= (sourceAddrMode & 0x3) << 14;

	return fcf;
}

//si el buffer de tx contiene un frame, lo transmito
//llamar a esta funcion desde el task
int ccTx(void)
{
	if(txbuf[0]==0) return -1;

	//cargo fifo tx
	ccCmd(SFLUSHTX, 0, 0, 0);
	ccCmd(TXBUF, 0, txbuf[0]-1, txbuf);

	//CSMA-CA
	//primero, leo bit SAMPLED_CCA
	u8 rssiv, ccaok;
	u16 timeout1=0;
	u16 timeout2;
	do
	{
		timeout1++;
		ccCmd(SRXON, 0, 0, 0);
		if(timeout1==0xFFF)
			return -7;
		timeout2=0;
		do
		{
			timeout2++;
			if(timeout2==0xFFF)
				return -8;

			ccCmd(SSAMPLECCA, 0, 0, 0);
			ccRegRd(RSSISTAT, 1, &rssiv);
			ccRegRd(FSMSTAT1, 1, &ccaok);
		}while(!((rssiv == 1)&&(ccaok & 0x08)));
		ccCmd(STXONCCA, 0, 0, 0);
		ccRegRd(FSMSTAT1, 1, &ccaok);
	}while(!(ccaok & 0x08));

	int i=0;
	u8 exc[3];
	do
	{
		if(i==0xFFFF) return -6;
		ccRegRd(EXCFLAG0, 3, exc);
		i++;
	}while(!(exc[0] & TX_FRM_DONE_BIT));

	//limpio bit TX_FRM_DONE
	ccCmd(BCLR, EXCFLAG0, 1, 0);

	txbuf[0]=0;

	ccCmd(SRXON, 0, 0, 0);

	return 0;
}

s8 ccFrameTx(frameData d)
{
	u8 i;

	//se esta transmitiendo otro paquete, abortar
	if(txbuf[0]) return -2;

	//comprobacion de errores
	if( (macFCFGetDestAddrMode(d.fcf)==0) &&
		(macFCFGetSrcAddrMode(d.fcf)==0) &&
		(d.fcf&PAN_ID_COMPRESSION_FLAG) )
		return -1;

	//armo la trama
	ccMemCpy(txbuf+1, &d.fcf, 2);
	txbuf[3] = d.seqn;

	i=4;
	switch(macFCFGetDestAddrMode(d.fcf))
	{
		case MAC_ADDR_MODE_NO_PAN_ADDR:
			break;
		case MAC_ADDR_MODE_1_RESERVED:
			return -3;
		case MAC_ADDR_MODE_SHORT:
			ccMemCpy(txbuf+i, &(d.dst.shortAddr.panid), 2);
			ccMemCpy(txbuf+i+2, &(d.dst.shortAddr.addr), 2);
			i+=4;
			break;
		case MAC_ADDR_MODE_EXTENDED:
			ccMemCpy(txbuf+i, &(d.dst.extAddr.panid), 2);
			ccMemCpy(txbuf+i+2, &(d.dst.extAddr.addr), 8);
			i+=10;
			break;
	}
	switch(macFCFGetDestAddrMode(d.fcf))
	{
		case MAC_ADDR_MODE_NO_PAN_ADDR:
			break;
		case MAC_ADDR_MODE_1_RESERVED:
			return -4;
		case MAC_ADDR_MODE_SHORT:
			if(d.fcf & PAN_ID_COMPRESSION_FLAG)
			{
				ccMemCpy(txbuf+i, &(d.src.shortAddr.addr), 2);
				i+=2;
			}
			else
			{
				ccMemCpy(txbuf+i, &(d.src.shortAddr.panid), 2);
				ccMemCpy(txbuf+i+2, &(d.src.shortAddr.addr), 2);
				i+=4;
			}
			break;
		case MAC_ADDR_MODE_EXTENDED:
			if(d.fcf & PAN_ID_COMPRESSION_FLAG)
			{
				ccMemCpy(txbuf+i, &(d.src.extAddr.addr), 8);
				i+=8;
			}
			else
			{
				ccMemCpy(txbuf+i, &(d.src.extAddr.panid), 2);
				ccMemCpy(txbuf+i+2, &(d.src.extAddr.addr), 8);
				i+=10;
			}
			break;
	}

	//aca iria el aux security header (no implementado aun...)

	//si el payload no entra, error
	if((i+d.pl_length+1) > 127) return -5;

	ccMemCpy(txbuf+i, d.payload, d.pl_length);

	txbuf[0] = i+d.pl_length+1;

	return txbuf[0];
}

s8 ccFrameRx(frameData * d)
{
	u8 i;
	u8 rxcont;

	ccRegRd(EXCFLAG1, 1, &i);
	if(!(i & RX_FRM_DONE_BIT)) return -1;

	ccCmd(BCLR, EXCFLAG1, 0, 0);

	ccCmd(MEMRD, RXFIFOCNT, 1, &rxcont);

	ccCmd(RXBUF, 0, rxcont, rxbuf);
	ccCmd(SFLUSHRX, 0, 0, 0);

	ccMemCpy(&(d->fcf), rxbuf+1, 2);
	ccMemCpy(&(d->seqn), rxbuf+3, 1);

	i=4;
	switch(macFCFGetDestAddrMode(d->fcf))
	{
		case MAC_ADDR_MODE_1_RESERVED: //este deberia filtrarse por hardware
			return -2;
		case MAC_ADDR_MODE_NO_PAN_ADDR:
			break;
		case MAC_ADDR_MODE_SHORT:
			ccMemCpy(&(d->dst.shortAddr.panid), rxbuf+i, 2);
			ccMemCpy(&(d->dst.shortAddr.addr), rxbuf+i+2, 2);
			i+=4;
			break;
		case MAC_ADDR_MODE_EXTENDED:
			ccMemCpy(&(d->dst.extAddr.panid), rxbuf+i, 2);
			ccMemCpy(&(d->dst.extAddr.addr), rxbuf+i+2, 8);
			i+=10;
			break;
	}
	switch(macFCFGetDestAddrMode(d->fcf))
	{
		case MAC_ADDR_MODE_1_RESERVED:
			return -3;
		case MAC_ADDR_MODE_NO_PAN_ADDR:
			break;
		case MAC_ADDR_MODE_SHORT:
			if(d->fcf & PAN_ID_COMPRESSION_FLAG)
			{
				ccMemCpy(&(d->src.shortAddr.addr), rxbuf+i, 2);
				i+=2;
			}
			else
			{
				ccMemCpy(&(d->src.shortAddr.panid), rxbuf+i, 2);
				ccMemCpy(&(d->src.shortAddr.addr), rxbuf+i+2, 2);
				i+=4;
			}
			break;
		case MAC_ADDR_MODE_EXTENDED:
			if(d->fcf & PAN_ID_COMPRESSION_FLAG)
			{
				ccMemCpy(&(d->src.extAddr.addr), rxbuf+i, 8);
				i+=8;
			}
			else
			{
				ccMemCpy(&(d->src.extAddr.panid),rxbuf+i, 2);
				ccMemCpy(&(d->src.extAddr.addr), rxbuf+i+2, 8);
				i+=10;
			}
			break;
	}

	//d->rssi = rxbuf[rxcont-2]-76; //ajusto segun hoja de datos (CC2520)
	d->rssi = rxbuf[rxcont-2]-79; //ajusto segun hoja de datos (CC2520+CC2591 HGM)
	//d->rssi = rxbuf[rxcont-2]-90; //ajusto segun hoja de datos (CC2520+CC2591 LGM)

	d->corr_crc = rxbuf[rxcont-1];

	/*aca iria el aux security header (no implementado aun...)*/

	ccMemCpy(rxbuf, rxbuf+i, rxcont-i-2);

	d->payload = rxbuf;
	d->pl_length = rxcont-i-2;

	return rxcont-i-2;
}

void ccMemCpy(void * dst, void * src, int n)
{
	int i;
	for(i=0; i<n; i++)
		((u8*)dst)[i] = ((u8*)src)[i];
	return;

}
