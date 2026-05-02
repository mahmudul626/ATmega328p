#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int wait_time = 1000;
char led_status[50] = "OFF";

void delay(int time) {
	volatile int i,j;
	for (i = 0; i < time; i++) {
		for(j = 0; j < 1000; j++);
	}
}

void led_blink(int count) {
	for(int i = 0; i < count; i++) {
		PORTB |= 0x20;
		delay(wait_time);
		PORTB &= ~0x20;
		delay(wait_time);
	}
}

void uart_init(void) {
	UBRR0 = 0x67;
	UCSR0B = 0x18;
	UCSR0C = 0x06;
}

char receive_data(void) {
	while(!(UCSR0A & 0x80));
	return UDR0;
}

void send(char data) {
	while(!(UCSR0A & 0x20));
	UDR0 = data;
}

void send_str(char *str) {
	while(*str) send(*str++);
}

void next() {
	send('\r');
	send('\n');
}

int main(void) {
	uart_init();
	DDRB = 0x20;
	char buffer[50];
	char *arg[50];

	while(1) {
		char c = receive_data();
		int i = 0;
		while(c != '\r' && i < 49) {
			send(c);
			buffer[i++] = c;
			c = receive_data();
		}
		buffer[i] = '\0';
		
		char *token = strtok(buffer, " \t\n");
		int j = 0;
		while(token != NULL) {
			arg[j++] = token;
			token = strtok(NULL, " \t\n");
		}
		arg[j] = NULL;

		if(j > 0) {
			if(strcmp(arg[0], "led") == 0 && strcmp(arg[1], "on") == 0) {
				PORTB |= 0x20;
				strcpy(led_status, "ON");
				send_str(" -> OK");
				next();
			} else if(strcmp(arg[0], "led") == 0 && strcmp(arg[1], "off") == 0) {
				PORTB &= ~0x20;
				strcpy(led_status, "OFF");
				send_str(" -> OK");
                                next();
			} else if(strcmp(arg[0], "led") == 0 && strcmp(arg[1], "status") == 0) {
				send_str(" -> ");
				send_str(led_status);
				next();
			} else if(strcmp(arg[0], "led") == 0 && strcmp(arg[1], "blink") == 0) {
				int count = atoi(arg[2]);
				led_blink(count);
				send_str(" -> OK");
                                next();
			} else if(strcmp(arg[0], "blink") == 0 && strcmp(arg[1], "delay") == 0) {
				int value = atoi(arg[2]);
				wait_time = value;
				send_str(" -> OK");
                                next();
			} else if(strcmp(arg[0], "help") == 0) {
				send_str(" -> OK\r\n");
			   	send_str("\r\nCOMMAND		\tJOB\r\n");
				send_str("-------------------------------");
				next();
				send_str("led on		\ton built in led\r\n");
				send_str("led off		\toff built in led\r\n");
				send_str("led status		check led's status\r\n");
				send_str("led blink 5		blink led 5 time (you can change number)\r\n");
				send_str("blink delay 500	\tset blink speed as 500 ms (you can change number)\r\n");
				send_str("help			show all command\r\n\r\n");
			} else {
				send_str(" -> ERROR");
				next();
			}
			
		}
	}

	return 0;
}
