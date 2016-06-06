/*
 * cc2520.c
 * Interfaz con CC2520 conectado a SSP1
 *
 *  Created on: 30/01/2012
 *      Author: pablo
 */

#include "includes.h"

u8 ccCmd(u8 cmd, u16 addr, u8 n, void * data)
{
	uint8_t txdata[n+10];
	uint8_t rxdata[n+10];
	u8 length;
	int i;

	if(n>128) return -1;

	txdata[0] = cmd;

	switch(cmd)
	{
		case SFLUSHTX:
		case SFLUSHRX:
		case STXON:
		case STXONCCA:
		case SRXON:
		case SSAMPLECCA:
		case SNOP:
			length = 1;
			break;

		case TXBUF:
			length = n+1;
			for(i=0; i<n; i++) txdata[i+1] = ((u8*)data)[i];
			break;

		case RXBUF:
			for(i=1; i<n+1; i++) txdata[i] = 0;
			length = n+1;
			break;

		case BCLR:
		case BSET:
			if(addr > 0x1F) return -1;
			if(n > 7) return -1;
			txdata[1] = ((addr&0x1F)<<3)|(n&0x7);
			length = 2;
			break;

		case MEMWR:
			txdata[0] |= ((addr&0xFFF)>>8);
			txdata[1] = (u8)(addr&0xFF);
			for(i=0; i<n; i++) txdata[i+2]=((u8*)data)[i];
			length = n+2;
			break;

		case MEMRD:
			txdata[0] |= ((addr&0xFFF)>>8);
			txdata[1] = (u8)(addr&0xFF);
			for(i=2; i<n+2; i++) txdata[i]=0;
			length = n+2;
			break;

		case REGRD:
			txdata[0] |= addr&0x3F;
			for(i=1; i<n+1; i++) txdata[i] = 0;
			length = n+1;
			break;

		case REGWR:
			txdata[0] |= addr&0x3F;
			for(i=0; i<n; i++) txdata[i+1]=((u8*)data)[i];
			length = n+1;
			break;

		default:
			return -1;
	}

	SSEL_Clr();
	for(i=0; i<0xff; i++);
	SSPSendReceive(txdata, rxdata, length);
	for(i=0; i<0xff; i++);
	SSEL_Set();

	switch(cmd)
	{
		case SFLUSHTX:
		case SFLUSHRX:
		case STXON:
		case STXONCCA:
		case SRXON:
		case SSAMPLECCA:
		case SNOP:
		case TXBUF:
		case BCLR:
		case BSET:
			break;

		case RXBUF:
		case REGRD:
		case REGWR:
			for(i=0; i<n; i++) ((u8*)data)[i] = rxdata[i+1];
			break;

		case MEMWR:
		case MEMRD:
			for(i=0; i<n; i++) ((u8*)data)[i] = rxdata[i+2];
			break;

		default:
			return -1;
	}

	return rxdata[0];
}

u8 ccRegRd(u8 reg, u8 n, void * out)
{
	if(reg < 0x040)
		return ccCmd(REGRD, reg, n, out);
	else
		return ccCmd(MEMRD, reg, n, out);
}

u8 ccRegWr(u8 reg, u8 n, void * in)
{
	if(reg < 0x040)
		return ccCmd(REGWR, reg, n, in);
	else
		return ccCmd(MEMWR, reg, n, in);
}


/* Actualiza registros del CC2520 siguiendo Tabla 21 de la hoja de datos */
void ccConfig(void)
{
	u8 i;

	i=0xF8;
	ccCmd(MEMWR, CCACTRL0, 1, &i);
	i=0x85;
	ccCmd(MEMWR, MDMCTRL0, 1, &i);
	i=0x14;
	ccCmd(MEMWR, MDMCTRL1, 1, &i);
	i=0x3F;
	ccCmd(MEMWR, RXCTRL, 1, &i);
	i=0x5A;
	ccCmd(MEMWR, FSCTRL, 1, &i);
	i=0x2B;
	ccCmd(MEMWR, FSCAL1, 1, &i);
	i=0x11;
	ccCmd(MEMWR, AGCCTRL1, 1, &i);
	i=0x10;
	ccCmd(MEMWR, ADCTEST0, 1, &i);
	i=0x0E;
	ccCmd(MEMWR, ADCTEST1, 1, &i);
	i=0x03;
	ccCmd(MEMWR, ADCTEST2, 1, &i);

	//Potencia de salida
	//i=0x32; //0dBm
	//i=0xF7; //5dBm
	i=0xF9;	//17dBm (CC2591)
	ccCmd(MEMWR, TXPOWER, 1, &i);
	//config. CC2591 (ver AN065 - SWRA229A)
	i=0x7F;
	ccRegWr(GPIOCTRL3, 1, &i);
	i=0x46;
	ccRegWr(GPIOCTRL4, 1, &i);
	i=0x47;
	ccRegWr(GPIOCTRL5, 1, &i);
	i=0x0F;
	ccRegWr(GPIOPOLARITY, 1, &i);
	i=0xC1;
	ccRegWr(TXCTRL, 1, &i);
	i=0x16;
	ccRegWr(AGCCTRL1, 1, &i);

	//ch. 11
	ccChannelSelect(18);

}

/* parametros: PANID y direcciones locales */
s8 ccInit(u16 panid, u16 shortAddr, u64 extAddr, u8 autoAck)
{
	//P0.0: RST -> controlado por hardware
//	LPC_GPIO0->DIR |= 1<<0;
//	LPC_GPIO0->DATA |= 1<<0;

	//P1.1: VREG_EN
	LPC_IOCON->JTAG_TDO_PIO1_1 |= 1;
	LPC_GPIO1->DIR |= 1<<1;
	LPC_GPIO1->DATA |= 1<<1;

	//P2.2: SSEL -> Controlado por software
	LPC_GPIO2->DIR |= 1<<2;
	SSEL_Set(); //modificar esta macro con el puerto correspondiente

	SSPInit();

	u8 id;
	ccRegRd(CHIPID, 1, &id);

	if(id != 0x84)
		/*error comunicacion spi*/
		return -1;

	ccConfig();

	if(autoAck) ccCmd(BSET, FRMCTRL0, 5, 0);

	ccSetLocalExtAddr(extAddr);
	ccSetLocalPANID(panid);
	ccSetLocalShortAddr(shortAddr);

	ccCmd(SRXON, 0, 0, 0);

	return 0;
}
