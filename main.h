#ifndef MAIN_H_
#define MAIN_H_

#define F_SETPOINT		10000000ULL
/* define the measurement interval in seconds */
#define INTERVAL_SEC		20
/* set the measurement break time (accomodate for frequency change) */
#define BREAK_SEC		2
/* set the dead-zone of the frequency indicator, in Hz = frequency resolution * X @ 10MHz, 20sec */
#define FDEV_DEADZONE		1
/* maximum deviation for correct calculation: 2^15 / INTERVAL_SEC = 1638Hz @ 20sec */
#define SETPOINT_COUNT  	(F_SETPOINT * INTERVAL_SEC) 
#define SETPOINT_COUNT_MOD	SETPOINT_COUNT % 65536

#define COUNT_STRING_LEN	10

/* Port 1 */
#define CLK_OCXO	BIT2
#define CLK_PPS		BIT6
#define VC_PWM		BIT3

/* Port 2 */
#define RXD_GPS		BIT6
#define TXD_GPS		BIT5
#define RXD_DEBUG	BIT1
#define TXD_DEBUG	BIT0

/* Port J */
#define LED1		BIT0
#define LED2		BIT1
#define LED3		BIT2
#define LED4		BIT3

#endif
