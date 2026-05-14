#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>

volatile uint8_t count1 = 0;
volatile uint8_t count2 = 0;
volatile uint8_t count3 = 0;

void timer0_init() {
	TCCR0A = (1<<WGM01);
	OCR0A = 156;
	TIMSK0 = (1<<OCIE0A);
	TCCR0B = (1<<CS02) | (1<<CS00);
}

void led_init() {
	DDRB = (1<<DDB5) | (1<<DDB4) | (1<<DDB3);
}

ISR(TIMER0_COMPA_vect) {
	count1++;
	count2++;
	count3++;

	if(count1 >= 100) {
		PORTB ^= (1<<PORTB5);
		count1 = 0;
	}

	if(count2 >= 50) {
		PORTB ^= (1<<PORTB4);
		count2 = 0;
	}

	if(count3 >= 30) {
		PORTB ^= (1<<PORTB3);
		count3 = 0;
	}
}

int main() {
	timer0_init();
	sei();
	led_init();
	while(1) {

	}
}
