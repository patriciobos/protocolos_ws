#include "includes.h"

/* statistics of all the interrupts */
volatile uint32_t interruptRxStat = 0;
volatile uint32_t interruptOverRunStat = 0;
volatile uint32_t interruptRxTimeoutStat = 0;

void SSP_IRQHandler(void) 
{
  uint32_t regValue;

  regValue = LPC_SSP->MIS;
  if ( regValue & SSPMIS_RORMIS )	/* Receive overrun interrupt */
  {
	interruptOverRunStat++;
	LPC_SSP->ICR = SSPICR_RORIC;		/* clear interrupt */
  }
  if ( regValue & SSPMIS_RTMIS )	/* Receive timeout interrupt */
  {
	interruptRxTimeoutStat++;
	LPC_SSP->ICR = SSPICR_RTIC;		/* clear interrupt */
  }
  if ( regValue & SSPMIS_RXMIS )	/* Rx at least half full */
  {
	interruptRxStat++;		/* receive until it's empty */		
  }
  return;
}

void SSPInit( void )
{
  uint8_t i, Dummy=Dummy;

  LPC_SYSCON->PRESETCTRL |= (0x1<<0);
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<11);
  LPC_SYSCON->SSPCLKDIV = 0x02;			/* Divided by 2 */
  LPC_IOCON->PIO0_8           &= ~0x07;	/*  SSP I/O config */
  LPC_IOCON->PIO0_8           |= 0x01;		/* SSP MISO */
  LPC_IOCON->PIO0_9           &= ~0x07;	
  LPC_IOCON->PIO0_9           |= 0x01;		/* SSP MOSI */

  /* On HummingBird 1(HB1), SSP CLK can be routed to different pins,
  other than JTAG TCK, it's either P2.11 func. 1 or P0.6 func. 2. */
  LPC_IOCON->SCKLOC = 0x01;
  LPC_IOCON->PIO2_11 = 0x01;/* P2.11 function 1 is SSP clock, need to combined
	  						with IOCONSCKLOC register setting */
		
  /* Set DSS data to 8-bit, Frame format SPI, CPOL = 0, CPHA = 0, and SCR is 15 */
  LPC_SSP->CR0 = 0x0707;

  /* SSPCPSR clock prescale register, master mode, minimum divisor is 0x02 */
  LPC_SSP->CPSR = 0x02;

  for ( i = 0; i < FIFOSIZE; i++ )
  {
	Dummy = LPC_SSP->DR;		/* clear the RxFIFO */
  }

  /* Enable the SSP Interrupt */
  NVIC_EnableIRQ(SSP_IRQn);
	
  /* Master mode */
  LPC_SSP->CR1 = SSPCR1_SSE;

  /* Set SSPINMS registers to enable interrupts */
  /* enable all error related interrupts */
  LPC_SSP->IMSC = SSPIMSC_RORIM | SSPIMSC_RTIM;
  return;
}

void SSPSendReceive( uint8_t * txbuf, uint8_t * rxbuf, uint32_t Length )
{
  uint8_t i;
  for ( i = 0; i < Length; i++ )
  {
	/* Move on only if NOT busy and TX FIFO not full. */
	while ( (LPC_SSP->SR & (SSPSR_TNF|SSPSR_BSY)) != SSPSR_TNF );
	LPC_SSP->DR = *txbuf;
	txbuf++;
	while ( (LPC_SSP->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE );
	/* Whenever a byte is written, MISO FIFO counter increments, Clear FIFO
	on MISO. Otherwise, when SSP0Receive() is called, previous data byte
	is left in the FIFO. */
	*rxbuf = LPC_SSP->DR;
	rxbuf++;
  }
  return;
}
