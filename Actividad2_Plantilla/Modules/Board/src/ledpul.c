/*
 * ledpul.c
 *
 *  Created on: May 4, 2012
 *      Author: pablo
 */

#include "includes.h"

int contled1, contled0;
u8 countLed0,countLed1,countLed2;

void ledCountDown()
{
	if(countLed0!=0){
		countLed0--;
		if(countLed0==0) ledWrite(0,0);
	}

	if(countLed1!=0){
		countLed1--;
		if(countLed1==0) ledWrite(1,0);
	}

	if(countLed2!=0){
		countLed2--;
		if(countLed2==0) ledWrite(2,0);
	}
}

void ledpulTick(void)
{
	contled0++;

	if((contled0>=1900)&&(contled0<=2000))
		ledWrite(0, 1);

	if((contled0 > 2000)||(contled0<0))
		ledWrite(0, 0), contled0=0;

	if(contled1)
	{
		contled1--;
		if(contled1==0)
			ledWrite(1, 0);
	}
}

void ledFlash(u8 led, u8 ticks)
{
	switch(led)
	{
		case 0:
			countLed0 = ticks;
			ledWrite(0,1);
			break;
		case 1:
			countLed1 = ticks;
			ledWrite(1,1);
			break;
		case 2:
			countLed2 = ticks;
			ledWrite(2,1);
			break;
		default:
			while(1);	// error al invocar a la funcion

	}

}

void ledWrite(u8 led, u8 val)
{
	switch(led)
	{
		case 0:
			if(val)
				LED0_PORT->DATA |= 1<<LED0_BIT;
			else
				LED0_PORT->DATA &= ~(1<<LED0_BIT);
			break;
		case 1:
			if(val)
				LED1_PORT->DATA |= 1<<LED1_BIT;
			else
				LED1_PORT->DATA &= ~(1<<LED1_BIT);
			break;
		case 2:
			if(val)
				LED2_PORT->DATA |= 1<<LED2_BIT;
			else
				LED2_PORT->DATA &= ~(1<<LED2_BIT);
			break;
	}
}

void ledpulInit(void)
{
	LED0_PORT->DIR |= 1<<LED0_BIT;
	LED0_PORT->DATA &= ~(1<<LED0_BIT);
	LED1_PORT->DIR |= 1<<LED1_BIT;
	LED1_PORT->DATA &= ~(1<<LED1_BIT);
	LED2_PORT->DIR |= 1<<LED2_BIT;
	LED2_PORT->DATA &= ~(1<<LED2_BIT);
}
