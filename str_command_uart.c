#include <avr/io.h>
#include <string.h>

void uart_init() {
	UBRR0 = 0x67;
	UCSR0B = 0x18;
	UCSR0C = 0x06;
}

char receive_data() {
	while(!(UCSR0A & 0x80));
	return UDR0;
}

void send_data(char data) {
	while(!(UCSR0A & 0x20));
	UDR0 = data;
}

void send_str(char *str) {
	while(*str) send_data(*str++);
}

int main (void) {
	DDRB = 0x20;
	uart_init();
	char str[50];

	while(1) {
		char c = receive_data();
		int i = 0;
		while(c != '\r') {
			str[i++] = c;
			c = receive_data();;
		}
		str[i] = '\0';
		send_str(str);

		/*--Led ON/OFF Logic--*/
		if(strcmp(str, "led on") == 0) {
			PORTB = PORTB | 0x20;
			send_str(" -> OK\r\n");
		} else if(strcmp(str, "led off") == 0) {
			PORTB = PORTB & ~0x20;
			send_str(" -> OK\r\n");
		} else {
			send_str(" -> ERROR\r\n");
		}
	}
return 0;
}
