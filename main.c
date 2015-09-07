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

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;
	hw_init();
	gps_startup_delay();
	while(!(gps_disable_nmea_output()));
	while(!(gps_set_gps_only()));
	
	while(1) {
	} /* while(1) */
} /* main() */

/*
 * Timer0 A0 ISR
 * CCR0 capture interrupt
 */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
{
}

