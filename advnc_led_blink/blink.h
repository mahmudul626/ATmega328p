#ifndef BLINK_H
#define BLINK_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void uart_init();
void timer0_init();
void rx_enqueue(char data);
void tx_enqueue(char data);
char rx_dequeue();
char tx_dequeue();

extern volatile uint8_t flag;

#endif
