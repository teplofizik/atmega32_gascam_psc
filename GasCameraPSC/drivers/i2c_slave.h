#include <stdint.h>
#include <stdbool.h>

#ifndef I2C_SLAVE_H_
#define I2C_SLAVE_H_

	typedef void (*TI2CWriteCallback)(uint8_t reg, uint8_t value);
	typedef uint8_t (*TI2CReadCallback)(uint8_t reg);

	void i2c_slave_Init(uint8_t address, TI2CWriteCallback write_cb, TI2CReadCallback read_cb);

#endif
