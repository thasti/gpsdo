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

volatile uint16_t start_count = 0;
volatile uint16_t end_count = 0;
volatile uint16_t meas_finished = 0;

void software_delay(void) {
	__delay_cycles(60000);
	__delay_cycles(60000);
	__delay_cycles(60000);
	__delay_cycles(60000);
	__delay_cycles(60000);
	__delay_cycles(60000);
	__delay_cycles(60000);
	__delay_cycles(60000);
}

int main(void) {
	uint16_t diff;
	int32_t freq_deviation;

	WDTCTL = WDTPW + WDTHOLD;
	hw_init();
	gps_startup_delay();
	while(!(gps_disable_nmea_output()));
	while(!(gps_set_gps_only()));
	
	while(1) {
		if (meas_finished == 1) {
			meas_finished = 0;
			diff = end_count - start_count;
			freq_deviation = (int32_t) (diff - SETPOINT_COUNT);
			if (freq_deviation < -FDEV_DEADZONE) {
				PJOUT = LED1;
			} else if (freq_deviation > FDEV_DEADZONE) {
				PJOUT = LED3;
			} else {
				PJOUT = LED2;
			}
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

