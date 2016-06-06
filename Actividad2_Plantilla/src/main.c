#include "includes.h"
#include "stdio.h"

__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

void SysTick_Handler(void)
{

	ledCountDown();			/* Leds countdown to off state */

	}


int main(void)
{
	SysTick_Config(SystemCoreClock/1000);

	if(ccInit(PANID_LOCAL,ADDR_LOCAL,ADDR_LOCAL,1)<0)
	{
		ledWrite(0,1);
		while(1);
	}

	ledpulInit();

	ccTask();		/* ccTask doesn't return */

	while(1);		/* main should never reach this point */

	return 0;
}



