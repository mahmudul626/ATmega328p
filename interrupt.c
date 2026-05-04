#include <avr/io.h>
#include <avr/interrupt.h>

volatile char new_data = 0;
volatile char receive_data;

void uart_init() {
	UBRR0 = 0x67; 
	UCSR0B = 0x98;
	UCSR0C = 0x06;
}

void send_char(char data) {
	while(!(UCSR0A & 0x20));
	UDR0 = data;
}

ISR(USART_RX_vect) {
	receive_data = UDR0;
	new_data = 1;
}

int main(void) {
	uart_init();
	sei();
	
	while(1) {
		if(new_data) {
			new_data = 0;
			send_char(receive_data);
		}
	}
	return 0;
}
