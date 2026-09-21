#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../macro.h"

volatile uint8_t event = 0;

static void sleep_mode_init(void)
{
	CLR_BIT(SMCR, SM2);
	SET_BIT(SMCR, SM1);
	CLR_BIT(SMCR, SM0);
}

static void input_init(void)
{
	CLR_BIT(DDRD, PD2);
	SET_BIT(PORTD, PD2);

	SET_BIT(EICRA, ISC01);
	CLR_BIT(EICRA, ISC00);

	SET_BIT(EIMSK, INT0);
}

ISR(INT0_vect)
{
	CLR_BIT(EIMSK, INT0);
	event = 1;
}

int main(void)
{
	input_init();
	sleep_mode_init();
	SET_BIT(DDRB, PB5);

	while(1) {
		cli();

		if (event) {
			CLR_BIT(SMCR, SE);
			event = 0;
			sei();
			FLIP_BIT(PORTB, PB5);
			_delay_ms(200);
			cli();

			SET_BIT(EIFR, INTF0);
			SET_BIT(EIMSK, INT0);
		}
		SET_BIT(SMCR, SE);
		sei();
		asm volatile ("sleep");
	}
}
