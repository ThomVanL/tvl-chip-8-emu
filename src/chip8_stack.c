#include "CHIP8_stack.h"
#include "CHIP8.h"

static void CHIP8_IsStackPointerInBounds(struct CHIP8 *chip8)
{
    assert(chip8->registers.stackPointer < CHIP8_TOTAL_STACK_DEPTH);
}

void CHIP8_PushStack(struct CHIP8 *chip8, uint16_t value)
{
    CHIP8_IsStackPointerInBounds(chip8);
    chip8->stack.stack[chip8->registers.stackPointer] = value;
    chip8->registers.stackPointer++;
}
uint16_t CHIP8_PopStack(struct CHIP8 *chip8)
{
    chip8->registers.stackPointer--;
    CHIP8_IsStackPointerInBounds(chip8);
    uint16_t value = chip8->stack.stack[chip8->registers.stackPointer];
    return value;
}
