#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define BUFFER_SIZE 64
char rx_buffer[BUFFER_SIZE];
char tx_buffer[BUFFER_SIZE];
volatile uint8_t rx_tail = 0;
volatile uint8_t rx_head = 0;
volatile uint8_t tx_tail = 0;
volatile uint8_t tx_head = 0;

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
	if(rx_tail == rx_head) return '\0';
	char data = rx_buffer[rx_head];
	rx_head = (rx_head+1) % BUFFER_SIZE;
	return data;
}

void tx_enqueue(char data) {
	uint8_t next = (tx_tail+1) % BUFFER_SIZE;
	if(next == tx_tail) return;
	tx_buffer[tx_tail] = data;
	tx_tail = next;
	UCSR0B |= (1<<UDRIE0);
}

char tx_dequeue() {
	if(tx_tail == tx_head) return '\0';
	char data = tx_buffer[tx_head];
	tx_head = (tx_head+1) % BUFFER_SIZE;
	return data;
	UCSR0B &= ~(1<<UDRIE0);
}

void send_str(char *str) {
	while(*str) {
		tx_enqueue(*str++);
	}
}

ISR(USART_RX_vect) {
	rx_enqueue(UDR0);
}

ISR(USART_UDRE_vect) {
	UDR0 = tx_dequeue();
}

int main() {
	uart_init();
	sei();
	char cmd_buffer[BUFFER_SIZE];
	uint8_t i = 0;

	while(1) {
		char c = rx_dequeue();
		if(c == '\0') continue;
		if(c == '\r') {
			cmd_buffer[i] = '\0';
			i = 0;
			send_str("\r\n");

			if(strcmp(cmd_buffer, "hi") == 0) {
				send_str("Arduino: hello\r\n");
			} else if(strcmp(cmd_buffer, "how are you") == 0) {
				send_str("Arduino: fine\r\n");
			}
		} else if(i < BUFFER_SIZE - 1) {
			cmd_buffer[i++] = c;
			char str[4];
			sprintf(str, "%c", c);
			send_str(str);
		}
	}
}
