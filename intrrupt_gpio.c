#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#define SET_BIT(reg, bit) (reg |= (1 << (bit)))
#define CLR_BIT(reg, bit) (reg &= ~(1 << (bit)))
#define FLIP_BIT(reg, bit) (reg ^= (1 << (bit)))
#define READ_BIT(reg, bit) (((reg) >> (bit)) & 0x01)

volatile uint8_t flag = 0;

static void delay(int time)
{
	volatile int i, j;
	for(i = 0; i < time; i++) {
		for(j = 0; j < 1000; j++);
	}
}

static void init(void)
{
	SET_BIT(DDRB, PB4);

	CLR_BIT(DDRD, PD2);
	SET_BIT(PORTD, PD2);

	CLR_BIT(EICRA, ISC00);
	SET_BIT(EICRA, ISC01);

	SET_BIT(EIMSK, INT0);

	sei();
}

ISR(INT0_vect)
{
	CLR_BIT(EIMSK, INT0);
	flag = 1;
}

int main(void)
{
	init();
	while(1) {
		if(flag) {
			flag = 0;
			FLIP_BIT(PORTB, PB4);
			delay(200);

			SET_BIT(EIFR, INTF0);
			SET_BIT(EIMSK, INT0);
		}
	}
}

