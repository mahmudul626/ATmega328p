#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SET_BIT(reg, bit) (reg |= (1<<(bit)))
#define CLR_BIT(reg, bit) (reg &= ~(1<<(bit)))
#define FLIP_BIT(reg, bit) (reg ^= (1<<(bit)))
#define READ_BIT(reg, bit) (((reg) >> (bit)) & 0x01)

volatile uint8_t pc_event = 0;
volatile uint8_t int0_event = 0;

static void init()
{
	SET_BIT(DDRB, PB4);
	SET_BIT(DDRB, PB5);

	CLR_BIT(DDRD, PD2);
	CLR_BIT(DDRB, PB0);

	SET_BIT(EICRA, ISC01);
	CLR_BIT(EICRA, ISC00);

	SET_BIT(PORTD, PD2);
	SET_BIT(PORTB, PB0);

	SET_BIT(EIMSK, INT0);

	SET_BIT(PCICR, PCIE0);

	SET_BIT(PCMSK0, PCINT0);
}

static void sleep_mode()
{
	CLR_BIT(SMCR, SM2);
	SET_BIT(SMCR, SM1);
	CLR_BIT(SMCR, SM0);
}

ISR(INT0_vect)
{
	CLR_BIT(EIMSK, INT0);
	int0_event = 1;
}

ISR(PCINT0_vect)
{
	CLR_BIT(PCMSK0, PCINT0);
	pc_event = 1;
}

int main()
{
	init();
	sleep_mode();

	while(1) {
		cli();

		if(int0_event) {
			CLR_BIT(SMCR, SE);
			int0_event = 0;

			sei();
			FLIP_BIT(PORTB, PB4);
			_delay_ms(200);
			cli();

			SET_BIT(EIFR, INTF0);
			SET_BIT(EIMSK, INT0);
		}

		if(pc_event) {
			CLR_BIT(SMCR, SE);
			pc_event = 0;

			if(READ_BIT(PINB, PB0) == 0)
				FLIP_BIT(PORTB, PB5);

			sei();
			_delay_ms(200);
			cli();

			SET_BIT(PCIFR, PCIF0);
			SET_BIT(PCMSK0, PCINT0);
		}

		SET_BIT(SMCR, SE);
		sei();
		asm volatile ("sleep");
	}
}
