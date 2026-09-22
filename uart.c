#include <avr/io.h>

static void send(char data)
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

static void send_str(char *data)
{
	while(*data)
		send(*data++);
}

static void uart_init()
{
	UBRR0 = 103;
	UCSR0B |= (1<<TXEN0);
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ02);
}

int main(void)
{
	uart_init();
	send_str("\r\n\r\nThis is test message\r\n");
	while(1);
}
