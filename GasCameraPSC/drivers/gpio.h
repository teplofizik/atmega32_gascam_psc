#include <stdint.h>
#include <stdbool.h>

#ifndef GPIO_H_
#define GPIO_H_

    typedef enum { PB = 0, PC = 1, PD = 2 } TPort;

    typedef struct
    {
        TPort   Port;
        uint8_t Pin;
    } TPin;

    void gp_Init(void);

    void gp_Output(const TPin * Pin);

    void gp_Input(const TPin * Pin);

    void gp_High(const TPin * Pin);

    void gp_Low(const TPin * Pin);

    void gp_Set(const TPin * Pin, bool Value);

    bool gp_Get(const TPin * Pin);

    void gp_PullUp(const TPin * Pin);

#endif
