#ifndef CHIP8_H
#define CHIP8_H

#include "CHIP8_config.h"
#include "CHIP8_memory.h"
#include "CHIP8_registers.h"
#include "CHIP8_stack.h"
#include "CHIP8_keyboard.h"
#include "CHIP8_screen.h"

#include <stdint.h>
#include <memory.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    struct CHIP8
    {
        struct CHIP8_Memory memory;
        struct CHIP8_Stack stack;
        struct CHIP8_Registers registers;
        struct CHIP8_Keyboard keyboard;
        struct CHIP8_Screen screen;
    };

    void CHIP8_Initialize(struct CHIP8 *chip8);
    void CHIP8_ExecuteOpCode(struct CHIP8 *chip8, uint16_t opCode);
    void CHIP8_LoadRomIntoMemory(struct CHIP8 *chip8, const int8_t *romBuffer, size_t romBufferSize);
    int CHIP8_LoadRomFromFile(struct CHIP8 *chip8, char const *filePath);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
