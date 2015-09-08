#include <msp430.h>
#include <inttypes.h>
#include "debug.h"

void debug_transmit_string(char *str) {
	while(*str) {
		while (!(UCA1IFG&UCTXIFG));
		UCA1TXBUF = *str;
		str++;
	}
}

void debug_transmit_string_fixed(char *str, uint8_t length) {
	uint8_t i;

	for (i = 0; i < length; i++) {
		while (!(UCA1IFG&UCTXIFG));
		UCA1TXBUF = str[i];
	}
}
