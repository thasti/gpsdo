/*
 * helper functions - string conversion
 *
 * Stefan Biereigel
 *
 */

#include <inttypes.h>

/* i32toa
 * 32 bit number to fixed-length output char
 */
void i32toa(uint32_t in, uint8_t len, volatile char *out) {
	uint8_t i;
	uint32_t mult = 1;
	for (i = len; i > 0; i--) {
		*(out + i - 1) = ((in % (mult*10)) / mult) + '0';
		mult *= 10;
	}
}

/* i16toa
 * 16 bit number to fixed-length output char
 */
void i16toa(uint16_t in, uint8_t len, volatile char *out) {
	uint8_t i;
	uint16_t mult = 1;
	for (i = len; i > 0; i--) {
		*(out + i - 1) = ((in % (mult*10)) / mult) + '0';
		mult *= 10;
	}
}

