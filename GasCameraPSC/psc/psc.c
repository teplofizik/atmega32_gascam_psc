#include "psc.h"
#include "../drivers/gpio.h"
#include "../drivers/i2c_slave.h"
#include "../drivers/voltmeter.h"
#include "../drivers/time.h"
#include "../config.h"

static const TPin OutputPin  = { PD, 2 };
static const TPin I2CPullupPin  = { PB, 0 };
static const TPin TriggerPin = { PD, 5 };

static bool OutputState;
static bool FirstPowerOn;
static uint16_t PeriodMinutes;
static uint32_t SleepTime;
static uint32_t WakeTime;
static uint8_t PscStatus;

static void delay_ms(uint16_t ms)
{
	volatile uint16_t i, j;
	for(i = 0; i < ms; i++)
		for(j = 0; j < 500; j++);
}

static void PowerOn(void)
{
	gp_Set(&I2CPullupPin, true);
	gp_Set(&OutputPin, true);

	delay_ms(5);

	i2c_slave_Enable();

	WakeTime = time_GetSeconds();
	OutputState = true;
}

static void PowerOff(void)
{
	i2c_slave_Disable();

	gp_Set(&I2CPullupPin, false);
	gp_Set(&OutputPin, false);

	OutputState = false;
	SleepTime = time_GetSeconds();
}

void psc_Init(void)
{
	PscStatus = PSC_STATUS_OK;
	FirstPowerOn = false;
	PeriodMinutes = 0;
	SleepTime = 0;
	OutputState = false;

	gp_Input(&TriggerPin);
	gp_PullUp(&TriggerPin);

	gp_Output(&I2CPullupPin);
	gp_Low(&I2CPullupPin);
	gp_Output(&OutputPin);
	gp_Low(&OutputPin);
}

void psc_Start(void)
{
	FirstPowerOn = true;

	PowerOn();
}

void psc_Main(void)
{
	uint16_t vIn = psc_GetInputMillivolts();

	if(vIn > PSC_OVP_THRESHOLD)
	{
		PscStatus |= PSC_STATUS_OVP | PSC_STATUS_FAULT;
		PowerOff();
		return;
	}
	else if(vIn < PSC_UVP_THRESHOLD)
	{
		PscStatus |= PSC_STATUS_UVP;
	}
	else
	{
		PscStatus &= ~(PSC_STATUS_OVP | PSC_STATUS_UVP);
	}

	if(PscStatus & PSC_STATUS_FAULT)
		return;

	if(OutputState)
	{
		if(psc_IsTriggered())
		{
			PscStatus |= PSC_STATUS_SLEEPING;

			PowerOff();
		}
	}
	else
	{
		if(SleepTime != 0)
		{
			uint32_t now = time_GetSeconds();
			uint32_t period = (uint32_t)PeriodMinutes * 60;

			if(now - SleepTime >= period)
			{
				PscStatus &= ~PSC_STATUS_SLEEPING;

				SleepTime = 0;
				PowerOn();
			}
		}
	}
}

void psc_SetOutput(bool on)
{
	if(PscStatus & PSC_STATUS_FAULT)
		return;

	if(on)
		PowerOn();
	else
		PowerOff();
}

bool psc_GetOutput(void)
{
	return OutputState;
}

bool psc_IsTriggered(void)
{
	return gp_Get(&TriggerPin) == false;
}

bool psc_IsFirstPowerOn(void)
{
	return FirstPowerOn;
}

void psc_SetPeriod(uint16_t minutes)
{
	PeriodMinutes = minutes;
}

uint16_t psc_GetInputMillivolts(void)
{
	// ������������ � ���������� �������
	uint32_t raw = voltmeter_GetMillivolts(PSC_INPUT_ADC_CH);

	return raw * 4782 / 1024;
}

uint8_t psc_GetStatus(void)
{
	return PscStatus;
}
