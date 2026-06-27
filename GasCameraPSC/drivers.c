#include "drivers.h"
#include "protocol.h"
#include "config.h"
#include <avr/power.h>
#include <avr/interrupt.h>

void drivers_Init(void)
{
	clock_prescale_set(clock_div_2);

	gp_Init();

	voltmeter_Init();

	i2c_slave_Init(I2C_ADDRESS, on_i2c_write, on_i2c_read);

	time_Init();

	sei();
}
