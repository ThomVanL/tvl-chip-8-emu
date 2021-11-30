//===================
//| (0,0)	(63,0)  |
//| (0,31)	(63,31) |
//===================
#ifndef CHIP8_SCREEN_H
#define CHIP8_SCREEN_H

#include "CHIP8_config.h"

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <memory.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    struct CHIP8_Screen
    {
        bool pixels[CHIP8_SCREEN_WIDTH][CHIP8_HEIGHT];
    };

    bool CHIP8_IsPixelSetScreen(struct CHIP8_Screen *CHIP8_screen, int x, int y);
    bool CHIP8_DrawSpriteToScreen(struct CHIP8_Screen *CHIP8_screen, int x, int y, const uint8_t *sprite, int num);
    void CHIP8_ClearScreen(struct CHIP8_Screen *CHIP8_screen);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
