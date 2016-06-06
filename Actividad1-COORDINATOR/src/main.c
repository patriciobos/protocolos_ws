#include "includes.h"
#include "stdio.h"

__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

extern u8 countLed0;
extern u8 countLed1;
extern u8 countLed2;

int countdown_tx=1;

extern State curr_state;

void SysTick_Handler(void)
{
#ifndef COORDINATOR
	if (0 != countdown_tx){
		countdown_tx--;
		if (0 == countdown_tx)
			curr_state = dataRequest;
	}
#endif

	if(countLed1!=0){
		countLed1--;
		if(countLed1==0) ledWrite(1,0);
	}

	if(countLed2!=0){
		countLed2--;
		if(countLed2==0) ledWrite(2,0);
	}
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



