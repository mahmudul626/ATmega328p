#include <avr/io.h>
#include <avr/interrupt.h>

volatile char c;
volatile char flag = 0;

void uart_init() {
	UBRR0 = 0x67;
	UCSR0B = 0x98;
	UCSR0C = 0x06;
}

ISR(USART_RX_vect) {
	c = UDR0;
	UCSR0B |= 0xB8;
}

ISR(USART_UDRE_vect) {
	UDR0 = c;
	UCSR0B &= ~0x20;
}

int main() {
	uart_init();
	sei();
	while(1) {
		//nothing
	}
return 0;
}
