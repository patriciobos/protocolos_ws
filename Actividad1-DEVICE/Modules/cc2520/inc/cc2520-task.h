/*
 * cc2520-task.h
 *
 *  Created on: Apr 24, 2012
 *      Author: pablo
 */

#ifndef CC2520_TASK_H_
#define CC2520_TASK_H_

#include "includes.h"
#include "stdio.h"

#define MAX_RETRIES 200

#define DEVICE

#define PANID_LOCAL	1
#define ADDR_COOR	1

#define ADDR_LOCAL	2

typedef enum {
	idle,
	dataRequest,
	waitingAck,
	waitingData,
	sendingAck,
	sendingData
} State;

frameData tx,rx;


void ccTask(void);

void ccFrameForming(int ADDR_DEST, char *payload_to_transmit);

#endif /* CC2520_TASK_H_ */
