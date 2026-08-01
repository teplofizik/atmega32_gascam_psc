#include "psc.h"
#include "../drivers/gpio.h"
#include "../drivers/i2c_slave.h"
#include "../drivers/voltmeter.h"
#include "../drivers/time.h"
#include "../config.h"

static const TPin OutputPin  = { PD, 2 };
static const TPin I2CPullupPin  = { PB, 0 };
static const TPin TriggerPin = { PD, 5 };

static bool NeedSleep         = false;
static bool OutputState       = true;
static bool FirstPowerOn      = true;
static bool TimeoutSleep      = false;
static uint16_t PeriodMinutes = 10;
static uint16_t TimeoutMinutes = 5;
static uint16_t RetryMinutes  = 0;
static uint32_t SleepTime     = 0;
static uint32_t SessionStart  = 0;
static uint32_t WakeTime      = 0;
static uint8_t PscStatus      = 0;

static void delay_ms(uint16_t ms)
{
	volatile uint16_t i, j;
	for(i = 0; i < ms; i++)
		for(j = 0; j < 500; j++);
}

static void PowerOn(void)
{
	gp_Set(&OutputPin, true);

	delay_ms(5);

	gp_Set(&I2CPullupPin, true);
	i2c_slave_Enable();

	OutputState = true;
	TimeoutSleep = false;
	PscStatus &= ~PSC_STATUS_TIMEOUT;
	SessionStart = time_GetSeconds();
	WakeTime = SessionStart;
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
	TimeoutMinutes = 5;
	RetryMinutes = 0;
	SleepTime = 0;
	SessionStart = 0;
	OutputState = false;
	TimeoutSleep = false;

	gp_Input(&TriggerPin);

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
		if(NeedSleep)
		{
			uint32_t now = time_GetSeconds();

			if(now - WakeTime > 2)
			{
				NeedSleep = false;
				TimeoutSleep = false;
				PscStatus |= PSC_STATUS_SLEEPING;
				PowerOff();
			}
		}
		else if(TimeoutMinutes > 0)
		{
			uint32_t now = time_GetSeconds();
			if(now - SessionStart >= (uint32_t)TimeoutMinutes * 60)
			{
				TimeoutSleep = true;
				PscStatus |= PSC_STATUS_SLEEPING | PSC_STATUS_TIMEOUT;
				PowerOff();
			}
		}
	}
	else
	{
		if(SleepTime != 0)
		{
			uint32_t now = time_GetSeconds();
			uint32_t interval;
			int32_t  diff = now - SleepTime;

			if(TimeoutSleep && RetryMinutes > 0)
				interval = (uint32_t)RetryMinutes * 60;
			else if(PeriodMinutes > 0)
				interval = (uint32_t)PeriodMinutes * 60;
			else
				interval = 0;

			if((interval > 0) && (diff >= interval))
			{
				PscStatus &= ~PSC_STATUS_SLEEPING;
				SleepTime = 0;
				PowerOn();
			}
		}
	}
}

void psc_Command(uint8_t command)
{
	if(command == 0x51)
	{
		NeedSleep = true;
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
	return gp_Get(&TriggerPin) == true;
}

bool psc_IsFirstPowerOn(void)
{
	return FirstPowerOn;
}

void psc_SetPeriod(uint16_t minutes)
{
	PeriodMinutes = minutes;
}

void psc_SetTimeout(uint16_t minutes)
{
	TimeoutMinutes = minutes;
}

void psc_SetRetry(uint16_t minutes)
{
	RetryMinutes = minutes;
}

uint16_t psc_GetInputMillivolts(void)
{
	uint32_t raw = voltmeter_GetMillivolts(PSC_INPUT_ADC_CH);

	return raw * 4782 / 1024;
}

uint8_t psc_GetStatus(void)
{
	return PscStatus;
}