#include <stdint.h>
#define DDRB (*(volatile uint8_t *)(0x24))
#define PORTB (*(volatile uint8_t *)(0x25))

#define SET_BIT(reg, bit) (reg |= (1<<(bit)))
#define FLIP_BIT(reg, bit) (reg ^= (1<<(bit)))

#define PB5 5

static void delay(int time)
{
	volatile int i,j;
	for(i = 0; i<time; i++) {
		for(j = 0; j<1000; j++);
	}
}

static void init_led(void)
{
	SET_BIT(DDRB, PB5);
}

int main(void)
{
	init_led();
	while(1) {
		FLIP_BIT(PORTB, PB5);
		delay(1000);
	}
}
