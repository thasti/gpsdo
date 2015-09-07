#include <msp430.h>
#include <inttypes.h>
#include "main.h"

/*
 * hw_init
 *
 * hardware initialisation. configures:
 *
 * USCIA0
 *   GPS communications USART
 *
 * USCIA1
 *   debug UART (frequency measurement)
 *
 */
void hw_init(void) {
	/* DCO init */
	CSCTL0_H = 0xA5;					/* write CS password */
	CSCTL1 = DCOFSEL_3;					/* set DCO to 8MHz */
	CSCTL2 = SELA__DCOCLK + SELS__DCOCLK + SELM__DCOCLK;	/* DCO as ACLK, SMCLK, MCLK */
	CSCTL3 = DIVA__1 + DIVS__1 + DIVM__1;			/* divide all sources */
	CSCTL4 = XT1OFF + XT2OFF;				/* disable oscillators */

	/* GPIO init port 1 */
	P1OUT = 0;
	P1DIR = 0;

	/* GPIO init port 2 */
	P2DIR = TXD_GPS + TXD_DEBUG;				
	P2SEL1 |= RXD_GPS + TXD_GPS + RXD_DEBUG + TXD_DEBUG;
	P2SEL0 &= ~(RXD_GPS + TXD_GPS + RXD_DEBUG + TXD_DEBUG);

	/* USCI_A0 (GPS UART) init */
	UCA0CTL1 = UCSWRST; 			/* reset USCI */
	UCA0CTL1 |= UCSSEL_2;			/* SMCLK */
	UCA0BR0 = 52;				/* 9600 baud at 8MHz DCO */
	UCA0BR1 = 0;
	UCA0MCTLW = (0x49<<8)+(1<<4)+UCOS16;	/* set UCA0BRS(<<8) and BRF(<<4) */
	UCA0CTL1 &= ~UCSWRST;			/* release from reset */

	/* USCI_A1 (debug UART) init */
	UCA1CTL1 = UCSWRST; 			/* reset USCI */
	UCA1CTL1 |= UCSSEL_2;			/* SMCLK */
	UCA1BR0 = 52;				/* 9600 baud at 8MHz DCO */
	UCA1BR1 = 0;
	UCA1MCTLW = (0x49<<8)+(1<<4)+UCOS16;	/* set UCA0BRS(<<8) and BRF(<<4) */
	UCA1CTL1 &= ~UCSWRST;			/* release from reset */

	/* TODO enable timer interrupt */
	TA0CTL = TASSEL__TACLK + ID__1 + MC__CONTINUOUS;
	TA0CCTL0 = CM_1 + CCIS_0 + CAP + CCIE;

	/* Enable Interrupts */
	__bis_SR_register(GIE);			/* set interrupt enable bit */
}
