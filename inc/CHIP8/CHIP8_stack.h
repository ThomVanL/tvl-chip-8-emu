#ifndef CHIP8_STACK_H
#define CHIP8_STACK_H
#include "CHIP8_config.h"

#include <stdint.h>
#include <assert.h>
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    struct CHIP8; //forward (incomplete) declaration

    struct CHIP8_Stack
    {
        uint16_t stack[CHIP8_TOTAL_STACK_DEPTH];
    };

    void CHIP8_PushStack(struct CHIP8 *chip8, uint16_t value);
    uint16_t CHIP8_PopStack(struct CHIP8 *chip8);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
