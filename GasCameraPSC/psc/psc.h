#include <stdint.h>
#include <stdbool.h>

#ifndef PSC_H_
#define PSC_H_

	#define PSC_OUTPUT_COUNT 2

	typedef enum
	{
		PSC_STATUS_OK        = 0x00,
		PSC_STATUS_OVP       = 0x01,
		PSC_STATUS_UVP       = 0x02,
		PSC_STATUS_FAULT     = 0x80,
	} TPSCStatus;

	void psc_Init(void);
	void psc_Main(void);

	void psc_SetOutput(bool on);
	bool psc_GetOutput(void);

	bool psc_IsTriggered(void);

	uint16_t psc_GetInputMillivolts(void);
	uint8_t psc_GetStatus(void);

#endif
