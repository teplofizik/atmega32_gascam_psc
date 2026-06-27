#include <stdint.h>

#ifndef VOLTMETER_H_
#define VOLTMETER_H_

	void voltmeter_Init(void);

	uint16_t voltmeter_GetMillivolts(uint8_t channel);

#endif
