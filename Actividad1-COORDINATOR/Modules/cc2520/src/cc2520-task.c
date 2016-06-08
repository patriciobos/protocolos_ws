/*
 * cc2520-task.c
 *
 *  Created on: 5 de jun. de 2016
 *      Author: patricio
 */

#include "cc2520-task.h"

extern int countdown_tx;

State curr_state = idle;

uint8_t addr_src;

void ccFrameForming(int ADDR_DEST, char *payload_to_transmit)
{
	tx.fcf = ccWrapperFCF(MAC_FRAME_TYPE_DATA, 0, 0, 0, 0, MAC_ADDR_MODE_SHORT, 0, MAC_ADDR_MODE_SHORT);
	tx.src.shortAddr.panid = PANID_LOCAL;
	tx.src.shortAddr.addr = ADDR_LOCAL;
	tx.dst.shortAddr.panid = PANID_LOCAL;
	tx.dst.shortAddr.addr = ADDR_DEST;

	tx.payload = payload_to_transmit;

	tx.pl_length = strlen(payload_to_transmit);

	tx.seqn = 1;

	ccFrameTx(tx);   // guarda la trama formada en el buffer de salida

	return;
}

void ccTask(){

	char tx_payload[128];
	char tmp_buff[32];
	uint8_t retries = 0;

	while(1)
	{
		switch (curr_state)
		{

		case idle:

			if (ccFrameRx(&rx) >= 0)
			{
				if (rx.dst.shortAddr.addr == ADDR_LOCAL)
				{
					if (0 == strncmp(rx.payload,"dataRequest from device",23)){
						addr_src = rx.src.shortAddr.addr;
						ledFlash(0, 100);
						curr_state = sendingAck;
					}
				}
			}

			break;

		case sendingAck:

			sprintf(tx_payload,"ACK");

			ccFrameForming(addr_src, tx_payload);

			ccTx();

			ledFlash(0, 100);

			curr_state = sendingData;
			break;


		case sendingData:

			sprintf(tx_payload,"hola nodo %d",addr_src);

			ccFrameForming(addr_src, tx_payload);

			ccTx();

			ledFlash(1, 100);

			curr_state = waitingAck;

			break;

		case waitingAck:

			if(retries < MAX_RETRIES)
			{
				if (ccFrameRx(&rx) >= 0)
				{
					if (rx.dst.shortAddr.addr == ADDR_LOCAL)
					{
						sprintf(tmp_buff,"ack from device %d",rx.src.shortAddr.addr);

						if (0 == strncmp(rx.payload,tmp_buff,17))
						{
							ledFlash(1, 100);
							retries = 0;
							curr_state = idle;
						}
					}
				}

			}
			else {
				retries = 0;
				curr_state = sendingData;
			}

			break;

		default:
				break;
		}

	}
return;

}

