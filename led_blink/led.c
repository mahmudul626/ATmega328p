#include "led.h"

void led_init() {
        DDRB = (1<<DDB5);
}

void led_on() {
        PORTB |= (1<<PORTB5);
}

void led_off() {
        PORTB &= ~(1<<PORTB5);
}

void delay(int time) {
        volatile int i,j;
        for(i = 0; i<time; i++) {
                for(j = 0; j<1000; j++);
        }
}
