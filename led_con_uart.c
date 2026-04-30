#include <avr/io.h>

void uart_init() {
	UBRR0 = 0x67; //Baud rate set as 9600. (value 103 which is in hex 0x67)
	UCSR0B = 0x18; //RX & TX both high
	UCSR0C = 0x06; // 8 bit data size set
}

int main(void) {
	DDRB = 0x20;
	uart_init();
	while(1) {
		if(UCSR0A & 0x80) {
		char data = UDR0;
			if(data == '1') {
				PORTB = PORTB | 0x20;
			} else {
				PORTB = PORTB & ~0x20;
			}
		}
	}


return 0;
}
