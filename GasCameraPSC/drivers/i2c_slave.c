#include "i2c_slave.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static TI2CWriteCallback i2c_write_cb = 0;
static TI2CReadCallback i2c_read_cb = 0;
static uint8_t reg_address = 0;
static bool is_reg_set = false;

void i2c_slave_Init(uint8_t address, TI2CWriteCallback write_cb, TI2CReadCallback read_cb)
{
	i2c_write_cb = write_cb;
	i2c_read_cb = read_cb;

	TWAR = address << 1;

	TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
}

void i2c_slave_Enable(void)
{
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
			TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
			break;

		case 0x80:
		{
			uint8_t data = TWDR;
			if(!is_reg_set)
			{
				reg_address = data;
				is_reg_set = true;
			}
			else
			{
				if(i2c_write_cb)
					i2c_write_cb(reg_address, data);
				reg_address++;
			}
			TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
			break;
		}

		case 0xA8:
		case 0xB0:
			TWDR = i2c_read_cb ? i2c_read_cb(reg_address) : 0;
			reg_address++;
			TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
			break;

		case 0xB8:
			TWDR = i2c_read_cb ? i2c_read_cb(reg_address) : 0;
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
