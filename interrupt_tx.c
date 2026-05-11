#include <avr/io.h>
#include <avr/interrupt.h>

#define SIZE 64
volatile char buffer[SIZE];
int tail = 0;
int head = 0;

void uart_init() {
	UBRR0 = 0x67;
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
}

void enqueue(char data) {
	int next = (tail+1) % SIZE;
	if(next == head) return;
	buffer[tail] = data;
	tail = next;
	UCSR0B |= (1<<UDRIE0);
}

void send_str(char *str) {
	while(*str) {
		enqueue(*str++);
	}
}

ISR(USART_UDRE_vect) {
	if(head == tail) {
		UCSR0B &= ~(1<<UDRIE0);
		return;
	}

	UDR0 = buffer[head];
	head = (head+1) % SIZE;
}

int main() {
	uart_init();
	sei();
	send_str("Alhamdulillah\r\n");
	send_str("Inshallah\r\n");
	while(1) {

	}
}
