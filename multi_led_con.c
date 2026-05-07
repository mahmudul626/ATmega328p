#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define BUFFER_SIZE 50
char buffer[BUFFER_SIZE];
volatile char c;
volatile char flag = 0;
volatile uint8_t rx_index = 0;

void send_str(char *str);

typedef struct {
	char *cmd;
	void (*handler) (void);
} cmd;

void red_on(void) {
	PORTB |= (1<<PORTB5);
	send_str("\t\t\t-> OK\r\n");
}

void red_off(void) {
	PORTB &= ~(1<<PORTB5);
	send_str("\t\t\t-> OK\r\n");
}

void green_on(void) {
	PORTB |= (1<<PORTB4);
	send_str("\t\t\t-> OK\r\n");
}

void green_off(void) {
	PORTB &= ~(1<<PORTB4);
	send_str("\t\t\t-> OK\r\n");
}

void yellow_on(void) {
	PORTB |= (1<<PORTB3);
	send_str("\t\t\t-> OK\r\n");
}

void yellow_off(void) {
	PORTB &= ~(1<<PORTB3);
	send_str("\t\t\t-> OK\r\n");
}

void led_status(void) {
	int count = 0;
	send_str("\t\t\t-> ");
	if(PORTB & (1<<PORTB5)) {
		send_str("RED ON ");
		count++;
	}
	
	if((PORTB & (1<<PORTB4)) && count == 0) {
		send_str("GREEN ON ");
		count++;
	}else if((PORTB & (1<<PORTB4)) && count > 0) {
                send_str("| GREEN ON ");
		count++;
	}
       
	if((PORTB & (1<<PORTB3)) && count == 0) {
		send_str("YELLOW ON ");
		count++;
	} else if((PORTB & (1<<PORTB3)) && count > 0) {
                send_str("| YELLOW ON ");
		count++;
	}
	
	if(count == 0) {
		send_str("NONE\r\n");
	}else {
		send_str("\r\n");
	}
}

void help(void) {
	send_str("\t\t\t-> cmd: red,green,yellow on/off, led status, help\r\n\t\t\t\texample: [red on] or [led status]\r\n");
}

void uart_init(void) {
	UBRR0 = 0x67;
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
}

void send_str(char *str) {
	while(*str) {
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = *str++;
	}
}

ISR(USART_RX_vect) {
	c = UDR0;
	UCSR0B |= (1<<UDRIE0);
	if(c == '\r') {
		buffer[rx_index] = '\0';
		rx_index = 0;
		flag = 1;
	} else if(rx_index < (BUFFER_SIZE - 1)) {
		buffer[rx_index++] = c;
	}
}

ISR(USART_UDRE_vect) {
	UDR0 = c;
	UCSR0B &= ~(1<<UDRIE0);
}

cmd command[] = {
                {"[red on]", red_on},
                {"[red off]", red_off},
                {"[green on]", green_on},
                {"[green off]", green_off},
                {"[yellow on]", yellow_on},
                {"[yellow off]", yellow_off},
                {"[led status]", led_status},
                {"[help]", help}
};

int main(void) {
	uart_init();
	sei();
	DDRB = (1<<DDB5) | (1<<DDB4) | (1<<DDB3);
	while(1) {
		if(flag) {
			flag = 0;
			for(int i = 0; i < 8; i++) {
				if(strcmp(buffer, command[i].cmd) == 0) {
					command[i].handler();
				}
			}
		}
	}
}
