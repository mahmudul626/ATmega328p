#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>

typedef struct {
	volatile uint16_t count;
	uint16_t target;
	uint8_t pin;
} led_t;

led_t led1 = {0, 500, PORTB5};
led_t led2 = {0, 400, PORTB4};
led_t led3 = {0, 300, PORTB3};
led_t led4 = {0, 200, PORTB2};
led_t led5 = {0, 100, PORTB1};
led_t led6 = {0, 50, PORTB0};

void timer0_init() {
	TCCR0A = (1<<WGM01);
	OCR0A = 156;
	TIMSK0 = (1<<OCIE0A);
	TCCR0B = (1<<CS02) | (1<<CS00);
}

void led_init() {
	DDRB |= (1<<DDB5) | (1<<DDB4) | (1<<DDB3) | (1<<DDB2) | (1<<DDB1) | (1<<DDB0);
}

void led_update(led_t *led) {
	led->count++;

	if(led->count >= led->target) {
		PORTB ^= (1<<led->pin);
		led->count = 0;
	}
}

ISR(TIMER0_COMPA_vect) {
	led_update(&led1);
	led_update(&led2);
	led_update(&led3);
	led_update(&led4);
	led_update(&led5);
	led_update(&led6);
}

int main() {
	timer0_init();
	sei();
	led_init();
	while(1);
}
