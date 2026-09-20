#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "macro.h"

volatile uint8_t count = 0;

static void timer_init(void)
{
	SET_BIT(TCCR0A, WGM01);
	OCR0A = 156;
	SET_BIT(TIMSK0, OCIE0A);
	SET_BIT(TCCR0B, CS02);
	SET_BIT(TCCR0B, CS00);
}

ISR(TIMER0_COMPA_vect)
{
	count++;
}

int main(void)
{
	SET_BIT(DDRB, PB5);
	timer_init();
	sei();

	while(1) {
		if(count >= 100) {
			count = 0;
			FLIP_BIT(PORTB, PB5);
		}
	}
}
