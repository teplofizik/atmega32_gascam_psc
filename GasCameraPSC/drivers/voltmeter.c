#include "voltmeter.h"
#include "../config.h"
#include <avr/io.h>

void voltmeter_Init(void)
{
	ADMUX = VREF_SELECT;
	ADCSRA = (1 << ADEN) | ADC_PRESCALER;
}

uint16_t voltmeter_GetMillivolts(uint8_t channel)
{
	ADMUX = VREF_SELECT | (channel & 0x07);

	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));

	uint8_t low = ADCL;
	uint16_t adc = (ADCH << 8) | low;

	return ((uint32_t)adc * VREF_MV) / 1024;
}
