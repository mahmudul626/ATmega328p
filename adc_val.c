#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>

static void adc_init(void)
{
	ADMUX = (1 << REFS0);
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

static uint16_t adc_read(void)
{
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));

	uint8_t low = ADCL;
	uint8_t high = ADCH;

	return (high << 8) | low;
}

static void uart_init(void)
{
	UCSR0B |= (1 << TXEN0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	UBRR0 = 0x67;
}

static void send(const char data)
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

static void send_str(const char *buf)
{
	while(*buf)
		send(*buf++);
}

int main(void)
{
	adc_init();
	uart_init();
	char buffer[24];
	volatile uint16_t value = 0;

	while(1) {
		value = adc_read();
		itoa(value, buffer, 10);
		send_str(buffer);
		send_str("\r\n");
		_delay_ms(500);
	}
}
