#include "blink.h"
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
	if(next==tx_head) return;
	tx_buffer[tx_tail] = data;
	tx_tail = next;
	UCSR0B |= (1<<UDRIE0);
}

char tx_dequeue() {
	if(tx_tail==tx_head) {
		UCSR0B &= ~(1<<UDRIE0);
		return '\0';
	}
	char data = tx_buffer[tx_head];
	tx_head = (tx_head+1) % BUFFER_SIZE;
	return data;
}

ISR(USART_RX_vect) {
	rx_enqueue(UDR0);
}

ISR(USART_UDRE_vect) {
	UDR0 = tx_dequeue();
}

void send_str(char *str) {
	while(*str) {
		tx_enqueue(*str++);
	}
}
