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
}

void on_i2c_write(uint8_t reg, uint8_t value)
{
	if(reg < MEM_SIZE)
		Registers[reg] = value;

	if(reg == REG_PERIOD)
	{
		uint16_t minutes = Registers[REG_PERIOD] | ((uint16_t)Registers[REG_PERIOD + 1] << 8);
		psc_SetPeriod(minutes);
	}
	if(reg == REG_PERIOD + 1)
	{
		uint16_t minutes = Registers[REG_PERIOD] | ((uint16_t)Registers[REG_PERIOD + 1] << 8);
		psc_SetPeriod(minutes);
	}
}

uint8_t on_i2c_read(uint8_t reg)
{
	if(reg == REG_STATUS)
	{
		uint8_t s = psc_GetStatus();

		Registers[REG_STATUS] = s;
		Registers[REG_STATUS + 1] = 0;

		if(psc_GetOutput())
			Registers[REG_STATUS + 1] |= 0x01;

		if(psc_IsFirstPowerOn())
			Registers[REG_STATUS + 1] |= 0x02;
	}

	if(reg == REG_VOLTAGE)
	{
		uint16_t v = psc_GetInputMillivolts();
		Registers[REG_VOLTAGE] = v & 0xFF;
		Registers[REG_VOLTAGE + 1] = (v >> 8) & 0xFF;
	}

	if(reg == REG_TIME)
	{
		uint32_t s = time_GetSeconds();
		Registers[REG_TIME + 0] = (uint8_t)(s & 0xFF);
		Registers[REG_TIME + 1] = (uint8_t)((s >> 8) & 0xFF);
		Registers[REG_TIME + 2] = (uint8_t)((s >> 16) & 0xFF);
		Registers[REG_TIME + 3] = (uint8_t)((s >> 24) & 0xFF);
	}

	if(reg < MEM_SIZE)
		return Registers[reg];

	return 0;
}

uint16_t proto_get_period(void)
{
	return Registers[REG_PERIOD] | ((uint16_t)Registers[REG_PERIOD + 1] << 8);
}
