#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "macro.h"
#define BRIGHTNESS 255

volatile uint8_t flag = 0;

static void pwm_init(void)
{
	SET_BIT(DDRD, PD6);
	TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
	TCCR0B |= (1 << CS01) | (1 << CS00);
}

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
	flag = 1;
}

int main(void)
{
	pwm_init();
	sleep_mode_init();
	input_init();

	while(1) {
		cli();

		if (flag) {
			CLR_BIT(SMCR, SE);
			flag = 0;

			sei();
			int led_b = BRIGHTNESS;
			for(; led_b >= 0; led_b--) {
				OCR0A = led_b;
				_delay_ms(10);
			}
			cli();

			SET_BIT(EIFR, INTF0);
			SET_BIT(EIMSK, INT0);
		}

		SET_BIT(SMCR, SE);
		sei();
		asm volatile ("sleep");
	}
}
