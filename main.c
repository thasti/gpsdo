/*
 * GPS disciplined OCXO control
 *
 * Stefan Biereigel
 *
 */

#include <msp430.h>
#include <inttypes.h>
#include "gps.h"
#include "main.h"
#include "hw.h"
#include "string.h"
#include "debug.h"

volatile uint16_t start_count = 0;
volatile uint16_t end_count = 0;
volatile uint16_t meas_finished = 0;

int main(void) {
	uint16_t diff;
	int16_t count_deviation;
	uint32_t count;
	char count_string[COUNT_STRING_LEN + 1];

	int16_t c_int = 0;
	int16_t vc = 0;

	WDTCTL = WDTPW + WDTHOLD;
	hw_init();
	gps_startup_delay();
	while(!(gps_disable_nmea_output()));
	while(!(gps_set_gps_only()));
	
	while(1) {
		if (P1IN & CLK_PPS) {
			PJOUT |= LED4;
		} else {
			PJOUT &= ~LED4;
		}
		if (meas_finished == 1) {
			meas_finished = 0;
			diff = end_count - start_count;
			count_deviation = (int16_t) (diff - SETPOINT_COUNT_MOD);
			/* visual lock indication */
			if (count_deviation < -FDEV_DEADZONE) {
				PJOUT = LED1;
			} else if (count_deviation > FDEV_DEADZONE) {
				PJOUT = LED3;
			} else {
				PJOUT = LED2;
			}

			/* output absolute count number to USART */
			count = SETPOINT_COUNT + count_deviation;
			i32toa(count, COUNT_STRING_LEN, count_string);	
			count_string[COUNT_STRING_LEN] = '\n';
			debug_transmit_string_fixed(count_string, COUNT_STRING_LEN + 1);

			/* control algorithm, count_deviation contains the control error */
			c_int = c_int + count_deviation;
			if (c_int > 200) c_int = 200;
			if (c_int < -200) c_int = -200;
			vc = (32768 - 350 * c_int) >> 8;
			if (vc > 255) vc = 255;
			if (vc < 0) vc = 0;
			TA1CCR2 = vc;
		}
	} /* while(1) */
} /* main() */

/*
 * Timer0 A0 ISR
 * CCR0 capture interrupt
 */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
{
	static uint16_t count_pps = INTERVAL_SEC + BREAK_SEC;
	count_pps--;
	PJOUT ^= LED4;
	switch (count_pps) {
		case INTERVAL_SEC:
			start_count = TA0CCR0;
			break;
		case 0: 
			end_count = TA0CCR0;
			count_pps = INTERVAL_SEC + BREAK_SEC + 1;
			meas_finished = 1;
			break;
		default:
			break;
	}


}

