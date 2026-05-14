#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define BUFFER_SIZE 64
char tx_buffer[BUFFER_SIZE];
volatile uint8_t tx_tail = 0;
volatile uint8_t tx_head = 0;
volatile uint8_t count = 0;
volatile uint8_t flag = 0;
volatile uint8_t s = 0;
volatile uint8_t m = 0;
volatile uint8_t h = 0;

void uart_init() {
	UBRR0 = 0x67;
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
}

void timer0_init() {
	TCCR0A = (1<<WGM01);
	OCR0A = 156;
	TIMSK0 = (1<<OCIE0A);
	TCCR0B = (1<<CS02) | (1<<CS00);
}

void tx_enqueue(char data) {
	uint8_t next = (tx_tail+1) % BUFFER_SIZE;
	if(next == tx_head) return;
	tx_buffer[tx_tail] = data;
	tx_tail = next;
	UCSR0B |= (1<<UDRIE0);
}

char tx_dequeue() {
	if(tx_tail == tx_head) {
		UCSR0B &= ~(1<<UDRIE0);
		return '\0';
	}

	char data = tx_buffer[tx_head];
	tx_head = (tx_head+1) % BUFFER_SIZE;
	return data;
}

void send_str(char *str) {
	while(*str) {
		tx_enqueue(*str++);
	}
}

ISR(USART_UDRE_vect) {
	UDR0 = tx_dequeue();
}

ISR(TIMER0_COMPA_vect) {
	count++;
	if(count >= 100) {
		count = 0;
		s++;
		flag = 1;

		if(s >= 60) {
			s = 0;
			m++;
			if(m >= 60) {
				m = 0;
				h++;
				if(h >= 24) {
					h = 0;
				}
			}
		}
	}
}

int main() {
	uart_init();
	timer0_init();
	sei();
	send_str("start_timer\r\n");
	while(1) { 
		if(flag) {
			flag = 0;
			char buffer[32];
			sprintf(buffer, "%02d:%02d:%02d", h,m,s);
			send_str(buffer);
			send_str("\r\n");
		}
	}
}
