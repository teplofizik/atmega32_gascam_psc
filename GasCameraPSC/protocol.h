#include <stdint.h>

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

	typedef enum
	{
		REG_STATUS  = 0,  // Текущий статус
		REG_STATUS2 = 1,  // Текущий статус
		REG_PERIOD  = 2,  // 2 байта: период пробуждения (мин)
		REG_VOLTAGE = 4,  // 2 байта: напряжение питания (мВ)
		REG_COUNTER = 6,
		REG_TIME    = 8,  // 4 байта: время работы мк (сек)
		REG_COMMAND = 16,
		REG_TIMEOUT = 17, // 2 байта: таймаут сессии (мин)
		REG_RETRY   = 19, // 2 байта: время между попытками (мин)
	} enum_register;

	void protocol_Init(void);
	void protocol_Update(void);

	void on_i2c_write(uint8_t reg, uint8_t value);

	uint8_t on_i2c_read(uint8_t reg);

	uint16_t proto_get_period();
	void proto_set_period(uint16_t minutes);

	uint16_t proto_get_timeout();
	void proto_set_timeout(uint16_t minutes);

	uint16_t proto_get_retry();
	void proto_set_retry(uint16_t minutes);
	
#endif
