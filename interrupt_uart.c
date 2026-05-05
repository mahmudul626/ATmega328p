#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

/*Baud rate mask*/
#define BAUD_RATE 0x67

#define BUFFER_SIZE 50
volatile char rx_index = 0;
volatile char data;
volatile char flag = 0;
char buffer[BUFFER_SIZE];

void uart_init(void) {
	UBRR0 = BAUD_RATE;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); //8 bit data pack
}

void send_str(char *str) {
	while(*str) {
		while(!(UCSR0A & (1<<UDRE0)));
			UDR0 = *str++;
	}
}

ISR(USART_RX_vect) {
	data = UDR0;
	UCSR0B |= (1<<UDRIE0);
	if(data == '\r') {
		buffer[rx_index] = '\0';
		rx_index = 0;
		flag = 1;
	} else if(rx_index < (BUFFER_SIZE - 1)) {
		buffer[rx_index++] = data;
	}
}

ISR(USART_UDRE_vect) {
	UDR0 = data;
	UCSR0B &= ~(1<<UDRIE0);
}

int main(void) {
	DDRB = (1<<DDB5);
	uart_init();
	sei();
	while(1) {
		if(flag) {
			flag = 0;
			if(strcmp(buffer, "led on") == 0) {
                        	        PORTB |= (1<<PORTB5);
                                	send_str("\t\t-> OK\r\n");
                	}else if(strcmp(buffer, "led off") == 0) {
                        	        PORTB &= ~(1<<PORTB5);
                               		send_str("\t\t-> OK\r\n");
                	}else if(strcmp(buffer, "led status") == 0) {
                        	if(PORTB & (1<<PORTB5)) {
                                	send_str("\t\t-> ON\r\n");
                        	}else {
                                	send_str("\t\t-> OFF\r\n");
                        	}
                	}else if(strcmp(buffer, "help") == 0) {
                        	        send_str("\t\t-> COMMAND : led on, led off, led status, help\r\n");
                	}else {
                        		send_str("\t\t-> ERROR\r\n");
                	}
		}
	}
	return 0;
}
