/*
 * main tracker software
 *
 * Stefan Biereigel
 *
 */

#ifndef MAIN_H_
#define MAIN_H_

#define F_SETPOINT	10000000ULL
/* define the measurement interval in seconds */
#define INTERVAL_SEC	20
/* set the measurement break time (accomodate for frequency change) */
#define BREAK_SEC	2
/* set the dead-zone of the frequency indicator FDEV_DEADZONE / INTERVAL_SEC  = 0,5 Hz @ 10, 20sec */
#define FDEV_DEADZONE	2
/* maximum deviation for correct calculation: 2^15 / INTERVAL_SEC = 163Hz @ 20sec */
#define SETPOINT_COUNT	(F_SETPOINT * INTERVAL_SEC) % 65536

#define FREQ_STRING_LEN	10

/* Port 1 */
#define CLK_OCXO	BIT2
#define CLK_PPS		BIT6

/* Port 2 */
#define RXD_GPS		BIT1
#define TXD_GPS		BIT0
#define RXD_DEBUG	BIT6
#define TXD_DEBUG	BIT5

/* Port J */
#define LED1		BIT0
#define LED2		BIT1
#define LED3		BIT2
#define LED4		BIT3

#endif
