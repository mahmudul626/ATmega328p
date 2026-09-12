#include <stdint.h>

#define PORTB (*(volatile uint8_t *)(0x25))
#define DDRB (*(volatile uint8_t *)(0x24))
#define PINB (*(volatile uint8_t *)(0x23))

#define SET_BIT(reg, bit) (reg |= (1 << (bit)))
#define CLR_BIT(reg, bit) (reg &= ~(1 << (bit)))
#define READ_BIT(reg, bit) (((reg) >> (bit)) & 0x01)
#define FLIP_BIT(reg, bit) (reg ^= (1 << (bit)))

#define PB4 4
#define PB5 5

static void delay(int time)
{
	volatile int i,j;
	for(i = 0; i < time; i++) {
		for(j = 0; j < 1000; j++);
	}
}

static void init(void)
{
	CLR_BIT(DDRB, PB4);
	SET_BIT(DDRB, PB5);
}

int main(void)
{
	init();
	SET_BIT(PORTB, PB4); //input pull up

	while(1) {
		uint8_t cur = READ_BIT(PINB, PB4);
		if(cur == 0) {
			FLIP_BIT(PORTB, PB5);
			delay(500);
		}
	}
}


