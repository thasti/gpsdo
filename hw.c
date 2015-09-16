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
 * TA0
 *   frequency counter (capture mode)
 *
 * TA1
 *   PWM output (output compare mode)
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
	P1DIR = VC_PWM;
	P1SEL1 |= CLK_OCXO + CLK_PPS;
	P1SEL1 &= ~VC_PWM;
	P1SEL0 |= CLK_PPS + VC_PWM;
	P1SEL0 &= ~CLK_OCXO;

	/* GPIO init port 2 */
	P2DIR = TXD_GPS + TXD_DEBUG;				
	P2SEL1 |= RXD_GPS + TXD_GPS + RXD_DEBUG + TXD_DEBUG;
	P2SEL0 &= ~(RXD_GPS + TXD_GPS + RXD_DEBUG + TXD_DEBUG);

	/* GPIO init port J */
	PJDIR = LED1 + LED2 + LED3 + LED4;
	PJOUT = 0;

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

	/* Timer A Capture/Compare 0 enable */
	TA0CTL = TASSEL__TACLK + ID__1 + MC__CONTINUOUS;
	TA0CCTL0 = CM_1 + CCIS_0 + CAP + CCIE;
	
	TA1CTL = TASSEL__SMCLK + ID__1 + MC__UP;
	TA1CCR0 = 255;
	TA1CCR2 = 127;
	TA1CCTL2 = OUTMOD_7;

	/* Enable Interrupts */
	__bis_SR_register(GIE);			/* set interrupt enable bit */
}

