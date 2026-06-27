#include "drivers.h"
#include "protocol.h"

int main(void)
{
	drivers_Init();
	protocol_Init();
	psc_Init();
	psc_Start();

    while(1)
    {
		psc_Main();
    }
}
