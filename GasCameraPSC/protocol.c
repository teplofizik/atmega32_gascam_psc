#include "protocol.h"
#include "drivers/time.h"

#define REG_COUNT 64

static uint8_t Registers[REG_COUNT];

void protocol_Init(void)
{
	uint8_t i;
	for(i = 0; i < REG_COUNT; i++)
		Registers[i] = 0;
}

void on_i2c_write(uint8_t reg, uint8_t value)
{
	if(reg < REG_COUNT)
		Registers[reg] = value;
}

uint8_t on_i2c_read(uint8_t reg)
{
	// Синхронное обновление времени
	if(reg == REG_TIME)
	{
		uint32_t s = time_GetSeconds();
		Registers[REG_TIME + 0] = (uint8_t)(s & 0xFF);
		Registers[REG_TIME + 1] = (uint8_t)((s >> 8) & 0xFF);
		Registers[REG_TIME + 2] = (uint8_t)((s >> 16) & 0xFF);
		Registers[REG_TIME + 3] = (uint8_t)((s >> 24) & 0xFF);
	}
	// Синхронное обновление напряжения
	if(reg == REG_VOLTAGE)
	{
		// ...
	}

	if(reg < REG_COUNT)
		return Registers[reg];

	return 0;
}


// Прочитать период
uint16_t proto_get_period()
{
	uint8_t vl = Registers[REG_PERIOD];
	uint8_t vh = Registers[REG_PERIOD + 1];
	
	uint16_t value = vl + ((uint16_t)vh) << 8;
	return value;
} 