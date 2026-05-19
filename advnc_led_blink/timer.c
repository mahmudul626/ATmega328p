#include "blink.h"

volatile uint8_t count = 0;

void timer0_init() {
	TCCR0A = (1<<WGM01) | (1<<WGM00);
	OCR0A = 156;
	TIMSK0 = (1<<OCIE0A);
	TCCR0B = (1<<CS01) | (1<<CS00);
}

ISR(TIMER0_COMPA_vect) {
	count++;
	if(count >= 100) {
		flag = 1;
		count = 0;
	}
}
