#include <avr/io.h>

void delay(int time) {
	volatile int i,j;
	for(i = 0; i<time; i++) {
		for(j = 0; j<1000; j++) ;
	}
}

int main() {
	DDRB |= (1<<PB5);

	while(1) {
		PORTB ^= (1<<PB5);
		delay(200);
	}
}
