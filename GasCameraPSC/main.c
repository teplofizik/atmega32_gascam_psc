#include "drivers.h"
#include "protocol.h"

int main(void)
{
	drivers_Init();
	protocol_Init();
	psc_Init();
	psc_SetPeriod(proto_get_period());
	psc_Start();

    while(1)
    {
		psc_Main();
		protocol_Update();
    }
}
