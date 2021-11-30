#ifndef CONFIG_H
#define CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    #define EMULATOR_WINDOW_TITLE "⚡️ CHIP-8 EMU ⚡️"
    #define CHIP8_MEMORY_MAX_SIZE 4096
    #define CHIP8_SCREEN_WIDTH 64
    #define CHIP8_HEIGHT 32
    #define CHIP8_WINDOW_MULTIPLIER 10
    #define CHIP8_TOTAL_DATA_REGISTERS 16
    #define CHIP8_TOTAL_STACK_DEPTH 16
    #define CHIP8_TOTAL_KEYS 16
    #define CHIP8_CHARACTER_SET_LOAD_ADDRESS 0x00
    #define CHIP8_PROGRAM_LOAD_ADDR 0x200
    #define CHIP8_DEFAULT_SPRITE_HEIGHT 0x5

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
