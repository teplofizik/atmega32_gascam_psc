# GasCameraPSC

Power Supply Controller for gas camera. Built on ATmega48PA.

## Hardware

- **MCU:** ATmega48PA
- **Clock:** 4 MHz (internal RC oscillator, clock_div_2 from 8 MHz)
  Minimum frequency for 100 kHz I2C slave operation.
- **I2C:** TWI slave at 100 kHz
- **Timer:** Timer1 in CTC mode for 1-second timekeeping

## Architecture

```
main.c
├── drivers.c/h          — Driver initialization
│   ├── drivers/gpio.c/h — Port/pin abstraction (PORTA–PORTD)
│   ├── drivers/i2c_slave.c/h — TWI slave with register callbacks
│   └── drivers/time.c/h — 1-second counter via Timer1
└── protocol.c/h         — I2C register callbacks (on_i2c_write / on_i2c_read)
```

Drivers follow the same pattern as the ADL NewDimmer project:
- `drivers_Init()` in `drivers.c` initialises all hardware
- GPIO uses `TPin` structs with port/pin abstraction
- I2C slave registers callbacks at init: `i2c_slave_Init(addr, on_i2c_write, on_i2c_read)`
- Timer counts seconds in the background

## I2C Protocol

I2C slave address: `0x50` (configurable in `config.h`).

Register map (64 bytes):
| Register | Access | Description |
|----------|--------|-------------|
| 0x00–0x03 | R | 32-bit uptime in seconds |
| 0x04–0x3F | R/W | General purpose |

Write sequence: `START` `addr+W` `reg` `data` `STOP`
Read sequence: `START` `addr+W` `reg` `STOP` `START` `addr+R` `data...` `STOP`

## Build

Open `GasCameraPSC.cproj` in Atmel Studio 7.0 and build.
