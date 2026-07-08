#include "i2c_slave.h"
#include "../protocol.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static uint8_t reg_address = 0;
static bool is_reg_set = false;

void i2c_slave_Init(uint8_t address, TI2CWriteCallback write_cb, TI2CReadCallback read_cb)
{
	(void)write_cb;
	(void)read_cb;

	TWAR = address << 1;

	TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
}

void i2c_slave_Enable(void)
{
	TWSR = 0;
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
}

void i2c_slave_Disable(void)
{
	TWCR = (1 << TWINT);
	TWCR = 0;
}

ISR(TWI_vect)
{
	uint8_t status = TWSR & 0xF8;

	switch(status)
	{
		case 0x60:
		case 0x68:
		case 0x70:
		case 0x78:
			is_reg_set = false;
			(void)TWDR;
			TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
			break;

		case 0x80:
			if(!is_reg_set)
			{
				reg_address = TWDR;
				is_reg_set = true;
			}
			else
			{
				on_i2c_write(reg_address, TWDR);
				reg_address++;
			}
			TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
			break;

		case 0xA8:
		case 0xB0:
			TWDR = on_i2c_read(reg_address);
			reg_address++;
			TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
			break;

		case 0xB8:
			TWDR = on_i2c_read(reg_address);
			reg_address++;
			TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
			break;

		case 0xC0:
		case 0xC8:
			TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
			break;

		default:
			TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
			break;
	}
}
