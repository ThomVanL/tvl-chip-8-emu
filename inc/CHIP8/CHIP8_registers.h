#ifndef CHIP8_REGISTERS_H
#define CHIP8_REGISTERS_H
#include "CHIP8_config.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    struct CHIP8_Registers
    {
        uint8_t v[CHIP8_TOTAL_DATA_REGISTERS];
        uint16_t i;
        uint8_t soundTimer;
        uint8_t delayTimer;
        uint16_t programCounter;
        uint8_t stackPointer;
    };

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
