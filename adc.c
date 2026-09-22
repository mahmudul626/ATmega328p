#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>
#include <stdint.h>
#include <avr/io.h>
#include "macro.h"

static void adc_init(void)
{
	SET_BIT(ADMUX, REFS0);
	SET_BIT(ADCSRA, ADEN);
	SET_BIT(ADCSRA, ADPS2);
	SET_BIT(ADCSRA, ADPS1);
	SET_BIT(ADCSRA, ADPS0);
}

static uint16_t adc_read(void)
{
	SET_BIT(ADCSRA, ADSC);
	while(ADCSRA & (1<<ADSC));
	uint8_t low = ADCL;
	uint8_t high = ADCH;

	return (high << 8) | low;
}

int main(void)
{
	adc_init();
	volatile uint16_t sensor_val = 0;
	while(1) {
		sensor_val = adc_read();
		_delay_ms(500);
	}
}
