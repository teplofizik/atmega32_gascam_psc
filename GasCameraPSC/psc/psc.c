#include "psc.h"
#include "../drivers/gpio.h"
#include "../drivers/voltmeter.h"
#include "../config.h"

static const TPin OutputPin  = { PD, 2 };
static const TPin TriggerPin = { PD, 5 };

static bool OutputState = false;
static uint8_t PscStatus;

void psc_Init(void)
{
	uint8_t i;

	PscStatus = PSC_STATUS_OK;

	gp_Input(&TriggerPin);
	gp_PullUp(&TriggerPin);

	gp_Output(&OutputPin);
	gp_Low(&OutputPin);
}

void psc_Main(void)
{
	uint16_t vIn = psc_GetInputMillivolts();

	if(vIn > PSC_OVP_THRESHOLD)
	{
		PscStatus |= PSC_STATUS_OVP | PSC_STATUS_FAULT;

		OutputState = false;
		gp_Low(&OutputPin);
	}
	else if(vIn < PSC_UVP_THRESHOLD)
	{
		PscStatus |= PSC_STATUS_UVP;
	}
	else
	{
		PscStatus = PSC_STATUS_OK;
	}
}

void psc_SetOutput(bool on)
{
	if(PscStatus & PSC_STATUS_FAULT)
		return;

	OutputState = on;
	gp_Set(&OutputPin, on);
}

bool psc_GetOutput(void)
{
	return OutputState;
}

bool psc_IsTriggered(void)
{
	return gp_Get(&TriggerPin) == false;
}

uint16_t psc_GetInputMillivolts(void)
{
	return voltmeter_GetMillivolts(PSC_INPUT_ADC_CH);
}

uint8_t psc_GetStatus(void)
{
	return PscStatus;
}
