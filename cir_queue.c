#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>

#define BUFFER_SIZE 64
#define CMD_SIZE 32
volatile char buffer[BUFFER_SIZE];
uint8_t tail = 0;
uint8_t head = 0;
char a_char;

typedef struct{
	char *cmd;
	void (*handler)(void);
}sys;

void led_on() {
	PORTB |= (1<<PORTB5);
}

void led_off() {
	PORTB &= ~(1<<PORTB5);
}

void uart_init() {
	UBRR0 = 0x67; //Baud rate 9600
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); //data pack size 8 bit
}

void enqueue(char data) {
	uint8_t next = (tail+1) % BUFFER_SIZE;
	if(next == head) {
		return;
	}

	buffer[tail] = data;
	tail = next;
}

char dequeue() {
	if(head == tail) {
		return '\0';
	}

	char data = buffer[head];
	head = (head+1) % BUFFER_SIZE;
	return data;
}

ISR(USART_RX_vect) {
	a_char = UDR0;
	enqueue(a_char);
	UCSR0B |= (1<<UDRIE0);
}

ISR(USART_UDRE_vect) {
	UDR0 = a_char;
	UCSR0B &= ~(1<<UDRIE0);
}

sys Sys[] = {
	{"[led on]", led_on},
	{"[led off]", led_off}
};

int main() {
	uart_init();
	sei();
	DDRB = (1<<DDB5);
	char cmd[CMD_SIZE];
	uint8_t cmdind = 0;

	while(1) {
		char c = dequeue();
		if(c == '\0') continue;
		if(c == '\r') {
			cmd[cmdind] = '\0';
			cmdind = 0;
			
			for(int i = 0; i<2; i++) {
				if(strcmp(cmd, Sys[i].cmd) == 0) {
					Sys[i].handler();
				}
			}
		} else if(cmdind < (CMD_SIZE-1)) {
			cmd[cmdind++] = c;
		}
	}
}
