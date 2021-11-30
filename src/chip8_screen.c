#include "CHIP8_screen.h"

static void CHIP8_IsPixelInScreenBounds(int x, int y)
{
    assert(x >= 0 && x < CHIP8_SCREEN_WIDTH && y >= 0 && y < CHIP8_HEIGHT);
}

bool CHIP8_IsPixelSetScreen(struct CHIP8_Screen *CHIP8_screen, int x, int y)
{
    CHIP8_IsPixelInScreenBounds(x, y);
    return CHIP8_screen->pixels[x][y];
}

bool CHIP8_DrawSpriteToScreen(struct CHIP8_Screen *CHIP8_screen, int x, int y, const uint8_t *sprite, int num)
{
    CHIP8_IsPixelInScreenBounds(x, y);
    bool pixel_collision = false;

    for (int lY = 0; lY < num; lY++)
    {
        uint8_t c = sprite[lY];
        for (int lX = 0; lX < 8; lX++)
        {
            if ((c & (0x80 >> lX)) == 0) //0x80 == 0b10000000
            {
                continue;
            }

            if (CHIP8_screen->pixels[(lX + x) % CHIP8_SCREEN_WIDTH][(lY + y) % CHIP8_HEIGHT]) // Wrap out of bounds
            {
                pixel_collision = true;
            }
            CHIP8_screen->pixels[(lX + x) % CHIP8_SCREEN_WIDTH][(lY + y) % CHIP8_HEIGHT] ^= true; //XOR
        }
    }

    return pixel_collision;
}

void CHIP8_ClearScreen(struct CHIP8_Screen *CHIP8_screen){
    memset(CHIP8_screen->pixels, 0, sizeof(CHIP8_screen->pixels));
}
