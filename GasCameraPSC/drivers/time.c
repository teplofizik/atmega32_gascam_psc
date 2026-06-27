#include "time.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile uint32_t Seconds = 0;

ISR(TIMER1_COMPA_vect)
{
	Seconds++;
}

void time_Init(void)
{
	TCCR1A = 0x00;

	OCR1A = (F_CPU / 256) - 1;

	TIMSK1 = (1 << OCIE1A);

	TCCR1B = (1 << WGM12) | (1 << CS12);
}

uint32_t time_GetSeconds(void)
{
	uint8_t sreg = SREG;
	cli();
	uint32_t s = Seconds;
	SREG = sreg;
	return s;
}
