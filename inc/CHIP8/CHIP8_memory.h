#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include "CHIP8_config.h"

#include <stdint.h>
#include <memory.h>
#include <assert.h>


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    struct CHIP8_Memory
    {
        uint8_t memory[CHIP8_MEMORY_MAX_SIZE];
    };

    void CHIP8_SetByteInMemory(struct CHIP8_Memory *memory, int index, uint8_t val);
    uint8_t CHIP8_GetByteFromMemory(struct CHIP8_Memory *memory, int index);
    uint16_t CHIP8_GetWordFromMemory(struct CHIP8_Memory *memory, int index);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
