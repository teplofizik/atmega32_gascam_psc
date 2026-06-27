#include <stdint.h>

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

	typedef enum
	{
		REG_STATUS  = 0,  // Текущий статус
		REG_PERIOD  = 2,  // Период пробуждения в минутах
		REG_VOLTAGE = 4,  // Напряжение батареи
		REG_COUNTER = 6,  // Количество пробуждений от батареи
		REG_TIME    = 8,  // Время в секундах (примерно) от запуска
		REG_MEM     = 16  // Память для заметок ESP32
	} enum_register;

	void protocol_Init(void);

	void on_i2c_write(uint8_t reg, uint8_t value);

	uint8_t on_i2c_read(uint8_t reg);

	// Прочитать период
	uint16_t proto_get_period();
	
	// Задать период
	void proto_set_period(uint16_t minutes);
	
#endif
