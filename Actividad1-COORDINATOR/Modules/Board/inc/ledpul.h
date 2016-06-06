/*
 * ledpul.h
 *
 *  Created on: May 4, 2012
 *      Author: pablo
 */

#ifndef LEDPUL_H_
#define LEDPUL_H_

#define LED0_PORT	LPC_GPIO1
#define LED0_BIT	5

#define LED1_PORT	LPC_GPIO2
#define LED1_BIT	0

#define LED2_PORT	LPC_GPIO1
#define LED2_BIT	8

void ledpulTick(void);
void ledWrite(u8 led, u8 val);
void ledpulInit(void);
void ledFlash(u8 led, u8 ticks);

extern int contled0;

#endif /* LEDPUL_H_ */
