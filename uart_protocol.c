#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define BUFFER_SIZE 50
volatile char c;
char buffer[BUFFER_SIZE];
volatile char flag = 0;
volatile int rx_index = 0;

void uart_init() {
	UBRR0 = 0x67;
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
}

ISR(USART_RX_vect) {
	c = UDR0;
	UCSR0B |= (1<<UDRIE0);
	if(c == '\r') {
		buffer[rx_index] = '\0';
		flag = 1;
		rx_index = 0;
	} else if(rx_index < (BUFFER_SIZE - 1)) {
		buffer[rx_index++] = c;
	}
}

ISR(USART_UDRE_vect) {
	UDR0 = c;
	UCSR0B &= ~(1<<UDRIE0);
}

int main() {
	uart_init();
	sei();
	DDRB = (1<<DDB5);

	while(1) {
		if(flag) {
			flag = 0;
			int len = strlen(buffer);
			if(buffer[0] == '[' && buffer[len-1] == ']') {
				if(strcmp(buffer, "[on]") == 0) {
					PORTB |= (1<<PORTB5);
				} else if (strcmp(buffer, "[off]") == 0) {
					PORTB &= ~(1<<PORTB5);
				}
			}
		}
	}
	return 0;
}
