#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define BUFFER_SIZE 50
char buffer[BUFFER_SIZE];
volatile int rx_index = 0;
volatile char flag = 0;

void uart_init(void) {
	UBRR0 = 0x67;
	UCSR0B = 0x98;
	UCSR0C = 0x06;
}

void send_char(char data) {
	while(!(UCSR0A & 0x20));
	UDR0 = data;;
}

void send_str(char *str) {
	while(*str) {
		send_char(*str++);
	}
}

ISR(USART_RX_vect) {
	char c = UDR0;
	if(c == '\r') {
		buffer[rx_index] = '\0';
		rx_index = 0;
		flag = 1;
	} else if(rx_index < BUFFER_SIZE) {
		buffer[rx_index++] = c;
		send_char(c);
	}
}

int main(void) {
	uart_init();
	sei();
	DDRB = 0x20;

	while(1) {
		if(flag) {
			flag = 0;
			if(strcmp(buffer, "led on") == 0) {
				PORTB |= 0x20;
				send_str(" -> OK");
				send_str("\r\n");
			} else if(strcmp(buffer, "led off") == 0) {
				PORTB &= ~0x20;
				send_str(" -> OK");
				send_str("\r\n");
			} else {
				send_str(" -> ERROR");
				send_str("\r\n");
			}
		}
	}
	return 0;
}

