#include "protocol.h"
#include "drivers/time.h"
#include "psc/psc.h"

#define  MEM_SIZE  32

static uint8_t Registers[MEM_SIZE];

void protocol_Init(void)
{
	uint8_t i;
	for(i = 0; i < MEM_SIZE; i++)
		Registers[i] = 0;

	proto_set_period(10);
	proto_set_timeout(5);
	proto_set_retry(0);
}

void on_i2c_write(uint8_t reg, uint8_t value)
{
	if(reg == REG_COMMAND)
	{
		psc_Command(value);
		return;
	}

	if(reg < MEM_SIZE)
		Registers[reg] = value;


	if(reg == REG_PERIOD)
	{
		//uint16_t minutes = Registers[REG_PERIOD] | ((uint16_t)Registers[REG_PERIOD + 1] << 8);
		//psc_SetPeriod(minutes);
	}

	if(reg == REG_PERIOD + 1)
	{
		uint16_t minutes = Registers[REG_PERIOD] | ((uint16_t)Registers[REG_PERIOD + 1] << 8);
		psc_SetPeriod(minutes);
	}

	if(reg == REG_TIMEOUT)
	{
		uint16_t t = Registers[REG_TIMEOUT] | ((uint16_t)Registers[REG_TIMEOUT + 1] << 8);
		psc_SetTimeout(t);
	}

	if(reg == REG_TIMEOUT + 1)
	{
		uint16_t t = Registers[REG_TIMEOUT] | ((uint16_t)Registers[REG_TIMEOUT + 1] << 8);
		psc_SetTimeout(t);
	}

	if(reg == REG_RETRY)
	{
		uint16_t r = Registers[REG_RETRY] | ((uint16_t)Registers[REG_RETRY + 1] << 8);
		psc_SetRetry(r);
	}

	if(reg == REG_RETRY + 1)
	{
		uint16_t r = Registers[REG_RETRY] | ((uint16_t)Registers[REG_RETRY + 1] << 8);
		psc_SetRetry(r);
	}
}

uint8_t on_i2c_read(uint8_t reg)
{
	if(reg < MEM_SIZE)
		return Registers[reg];

	return 0;
}

uint16_t proto_get_period(void)
{
	return Registers[REG_PERIOD] | ((uint16_t)Registers[REG_PERIOD + 1] << 8);
}

void proto_set_period(uint16_t minutes)
{
	Registers[REG_PERIOD] = minutes & 0xFF;
	Registers[REG_PERIOD + 1] = (minutes >> 8) & 0xFF;
}

uint16_t proto_get_timeout(void)
{
	return Registers[REG_TIMEOUT] | ((uint16_t)Registers[REG_TIMEOUT + 1] << 8);
}

void proto_set_timeout(uint16_t minutes)
{
	Registers[REG_TIMEOUT] = minutes & 0xFF;
	Registers[REG_TIMEOUT + 1] = (minutes >> 8) & 0xFF;
}

uint16_t proto_get_retry(void)
{
	return Registers[REG_RETRY] | ((uint16_t)Registers[REG_RETRY + 1] << 8);
}

void proto_set_retry(uint16_t minutes)
{
	Registers[REG_RETRY] = minutes & 0xFF;
	Registers[REG_RETRY + 1] = (minutes >> 8) & 0xFF;
}

void protocol_Update(void)
{
	{
		uint8_t s = psc_GetStatus();
		Registers[REG_STATUS] = s;
		Registers[REG_STATUS2] = 0;
	}

	{
		uint8_t s2 = 0;
		if(psc_GetOutput())
		s2 |= 0x01;

		if(psc_IsFirstPowerOn())
		s2 |= 0x02;
		
		Registers[REG_STATUS2] = s2;
	}
	
	{
		uint16_t v = psc_GetInputMillivolts();
		Registers[REG_VOLTAGE] = v & 0xFF;
		Registers[REG_VOLTAGE + 1] = (v >> 8) & 0xFF;
	}

	{
		uint32_t s = time_GetSeconds();
		Registers[REG_TIME + 0] = (uint8_t)(s & 0xFF);
		Registers[REG_TIME + 1] = (uint8_t)((s >> 8) & 0xFF);
		Registers[REG_TIME + 2] = (uint8_t)((s >> 16) & 0xFF);
		Registers[REG_TIME + 3] = (uint8_t)((s >> 24) & 0xFF);
	}
}