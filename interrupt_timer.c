#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

volatile uint8_t count = 0;

void timer0_init() {
	TCCR0A = (1<<WGM01);
	OCR0A = 156;
	TIMSK0 = (1<<OCIE0A);
	TCCR0B = (1<<CS02) | (1<<CS00);
}

ISR(TIMER0_COMPA_vect) {
	count++;
	if(count >= 100) {
		PORTB ^= (1<<PORTB5);
		count = 0;
	}
}

int main() {
	timer0_init();
	sei();
	DDRB = (1<<DDB5);
	while(1) {

	}
}
