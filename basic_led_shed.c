#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

/*
 * led config mask
 */

#define LED1_PIN PB5
#define LED2_PIN PB4
#define LED3_PIN PB3
#define LED4_PIN PB2
#define LED5_PIN PB1
#define LED6_PIN PB0

/*
 * system control info
 */

typedef struct {
	volatile uint16_t count;
	uint16_t target;
	uint8_t pin;
} led_t;

/*
 * system data object
 */

led_t led[] = {
	{0, 500, LED1_PIN},
	{0, 400, LED2_PIN},
	{0, 300, LED3_PIN},
	{0, 200, LED4_PIN},
	{0, 100, LED5_PIN},
	{0, 50,  LED6_PIN}
};

/*
 * timer0 init
 */

void timer0_init() {
	TCCR0A = (1<<WGM01);
	OCR0A = 156;
	TIMSK0 = (1<<OCIE0A);
	TCCR0B = (1<<CS02) | (1<<CS00);
}

/*
 * update led
 */

void led_update(led_t *led) {
	led->count++;
	if(led->count >= led->target) {
		PORTB ^= (1<<led->pin);
		led->count = 0;
	}
}

/*
 * timer0 interrupt
 */

ISR(TIMER0_COMPA_vect) {
	for(uint8_t i = 0; i<6; i++) {
		led_update(&led[i]);
	}
}

int main() {
	timer0_init();
	sei();
	for(uint8_t i = 0; i<6; i++) {
                DDRB |= (1<<led[i].pin);
        }
	while(1);
}

//DEV: Md. Mahmudul Hasan Mabud
//DATA: 14 May 2026 time: 6:55 PM
