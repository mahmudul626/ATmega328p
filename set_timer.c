#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 64
char rx_buffer[BUFFER_SIZE];
char tx_buffer[BUFFER_SIZE];
volatile uint8_t rx_tail = 0;
volatile uint8_t rx_head = 0;
volatile uint8_t tx_tail = 0;
volatile uint8_t tx_head = 0;
volatile uint8_t count = 0;
volatile uint8_t flag = 0;
volatile uint8_t h = 0;
volatile uint8_t m = 0;
volatile uint8_t s = 0;

void timer0_init() {
	TCCR0A = (1<<WGM01);
	OCR0A = 156;
	TIMSK0 = (1<<OCIE0A);
	TCCR0B = (1<<CS02) | (1<<CS00);
}

void uart_init() {
	UBRR0 = 0x67;
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
}

void rx_enqueue(char data) {
	uint8_t next = (rx_tail+1) % BUFFER_SIZE;
	if(next == rx_head) return;
	rx_buffer[rx_tail] = data;
	rx_tail = next;
}

char rx_dequeue() {
	if(rx_head == rx_tail) return '\0';
	char data = rx_buffer[rx_head];
	rx_head = (rx_head+1) % BUFFER_SIZE;
	return data;
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

ISR(USART_RX_vect) {
	rx_enqueue(UDR0);
}

ISR(TIMER0_COMPA_vect) {
	count++;
	if(count >= 100) {
		flag = 1;
		s++;
		count = 0;
		if(s >= 60) {
			m++;
			s = 0;
			if(m >= 60) {
				h++;
				m = 0;
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
	char cmd_buffer[BUFFER_SIZE];
	volatile uint8_t i = 0;
	while(1) {

		if(flag) {
			flag = 0;
			char time_str[32];
			sprintf(time_str, "%02d:%02d:%02d\r\n", h,m,s);
			send_str(time_str);
		}
		char c = rx_dequeue();
		if(c == '\0') continue;
		if(c == '\r') {
			cmd_buffer[i] = '\0';
			i = 0;

			send_str(cmd_buffer);
			send_str("\r\n");
		}else if(i < BUFFER_SIZE-1) {
			cmd_buffer[i++] = c;
			char str_c[4];
			sprintf(str_c, "%c", c);
			send_str(str_c);
		}
	}
}
