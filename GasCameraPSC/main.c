#include "drivers.h"
#include "protocol.h"

int main(void)
{
	drivers_Init();
	protocol_Init();
	psc_Init();
	
    while(1)
    {
		psc_Main();
    }
}
