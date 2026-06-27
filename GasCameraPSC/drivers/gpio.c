#include "gpio.h"
#include <avr/io.h>

typedef struct
{
	volatile uint8_t * DDR;
	volatile uint8_t * PORT;
	volatile uint8_t * PIN;
} TPhyPort;

const TPhyPort Ports[] = {
	{ &DDRB, &PORTB, &PINB },
	{ &DDRC, &PORTC, &PINC },
	{ &DDRD, &PORTD, &PIND },
};

const int PortCount = sizeof(Ports) / sizeof(Ports[0]);

void gp_Init(void)
{

}

void gp_Output(const TPin * Pin)
{
	uint8_t Port = Pin->Port;
	uint8_t Mask = (1 << Pin->Pin);

	*Ports[Port].DDR |= Mask;
}

void gp_Input(const TPin * Pin)
{
	uint8_t Port = Pin->Port;
	uint8_t Mask = (1 << Pin->Pin);

	*Ports[Port].DDR &= ~Mask;
}

void gp_High(const TPin * Pin)
{
	uint8_t Port = Pin->Port;
	uint8_t Mask = (1 << Pin->Pin);

	*Ports[Port].PORT |= Mask;
}

void gp_Low(const TPin * Pin)
{
	uint8_t Port = Pin->Port;
	uint8_t Mask = (1 << Pin->Pin);

	*Ports[Port].PORT &= ~Mask;
}

bool gp_Get(const TPin * Pin)
{
	uint8_t Port = Pin->Port;
	uint8_t Mask = (1 << Pin->Pin);

	return (*Ports[Port].PIN & Mask) != 0;
}

void gp_Set(const TPin * Pin, bool Value)
{
	if(Value)
		gp_High(Pin);
	else
		gp_Low(Pin);
}

void gp_PullUp(const TPin * Pin)
{
	uint8_t Port = Pin->Port;
	uint8_t Mask = (1 << Pin->Pin);

	*Ports[Port].PORT |= Mask;
}
