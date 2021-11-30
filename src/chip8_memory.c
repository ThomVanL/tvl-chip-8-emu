#include "CHIP8_memory.h"

static void CHIP8_is_memory_in_bounds(int index)
{
    assert(index >= 0 && index < CHIP8_MEMORY_MAX_SIZE);
}

void CHIP8_SetByteInMemory(struct CHIP8_Memory *memory, int index, uint8_t val)
{
    CHIP8_is_memory_in_bounds(index);
    memory->memory[index] = val;
}

uint8_t CHIP8_GetByteFromMemory(struct CHIP8_Memory *memory, int index)
{
    CHIP8_is_memory_in_bounds(index);
    return memory->memory[index];
}

uint16_t CHIP8_GetWordFromMemory(struct CHIP8_Memory *memory, int index)
{
    CHIP8_is_memory_in_bounds(index +1);
    uint8_t byte1 = CHIP8_GetByteFromMemory(memory,index);
    uint8_t byte2 = CHIP8_GetByteFromMemory(memory, index+1);

    return byte1 << 8 | byte2;
}
