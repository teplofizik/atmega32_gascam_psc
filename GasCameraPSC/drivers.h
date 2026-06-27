#include <stdint.h>

#ifndef DRIVERS_H_
#define DRIVERS_H_

	#include "drivers/gpio.h"
	#include "drivers/i2c_slave.h"
	#include "drivers/time.h"
	#include "drivers/voltmeter.h"
	#include "psc/psc.h"

	void drivers_Init(void);

#endif
