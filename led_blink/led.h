#ifndef LED_H
#define LED_H

#include <avr/io.h>

void led_init();
void led_on();
void led_off();
void delay(int time);

#endif
