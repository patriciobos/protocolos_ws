/*
 * cc2520-task.c
 *
 *  Created on: 5 de jun. de 2016
 *      Author: patricio
 */

#include "cc2520-task.h"

State curr_state = idle;

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
	uint8_t retries = 0;

	while(1)
	{
		switch (curr_state)
		{
			case idle:

				__ASM volatile ("wfi");
				break;

			case dataRequest:

				sprintf(tx_payload,"dataRequest from device %d",ADDR_LOCAL);

				ccFrameForming(ADDR_COOR, tx_payload);

				ccTx();

				ledFlash(0, 100);

				curr_state = waitingAck;

				break;

			case waitingAck:

				if (retries < MAX_RETRIES){

					if (ccFrameRx(&rx) >= 0)
					{
						if (rx.dst.shortAddr.addr == ADDR_LOCAL)
						{
							if (0 == strncmp(rx.payload,"ACK",3)){
								ledFlash(1, 100);
								retries = 0;
								curr_state = idle;
							}
						}
					}
					retries++;
				}

				else {
					retries = 0;
					curr_state = dataRequest;
				}

				break;

			default:
				break;
			}

	}
return;  	// Esta función no debe retornar
}

