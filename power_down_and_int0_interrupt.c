#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

#define SET_BIT(reg, bit) (reg |= (1<<(bit)))
#define CLR_BIT(reg, bit) (reg &= ~(1<<(bit)))
#define FLIP_BIT(reg, bit) (reg ^= (1<<(bit)))

volatile uint8_t button_event = 0;

static void init(void)
{
	SET_BIT(DDRB, PB4);
	CLR_BIT(DDRD, PD2);

	SET_BIT(PORTD, PD2);

	SET_BIT(EICRA, ISC01);
	CLR_BIT(EICRA, ISC00);

	SET_BIT(EIMSK, INT0);
}

static void set_power_down(void)
{
	CLR_BIT(SMCR, SM2);
	SET_BIT(SMCR, SM1);
	CLR_BIT(SMCR, SM0);
}

ISR(INT0_vect)
{
	CLR_BIT(EIMSK, INT0);
	button_event = 1;
}

int main(void) {
	init();
	set_power_down();

	while(1) {
		cli();

		if(button_event) {
			CLR_BIT(SMCR, SE);
			button_event = 0;

			sei();
			FLIP_BIT(PORTB, PB4);
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

