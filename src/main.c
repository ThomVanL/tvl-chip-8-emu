
#ifdef _WIN32
#include <Windows.h>
#endif
#include "CHIP8.h"

const char keyboard_map[CHIP8_TOTAL_KEYS] = {          // Hard coded key mapping
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5,    // Greatest kind of key mapping
    SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b,
    SDLK_c, SDLK_d, SDLK_e, SDLK_f};

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Provide the path to a ROM that you wish to load.\n");
        return -1;
    }

    struct CHIP8 chip8;
    CHIP8_Initialize(&chip8);
    CHIP8_SetVirtualKeyboardMap(&chip8.keyboard, keyboard_map);
    CHIP8_LoadRomFromFile(&chip8, argv[1]);

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        CHIP8_SCREEN_WIDTH * CHIP8_WINDOW_MULTIPLIER,
        CHIP8_HEIGHT * CHIP8_WINDOW_MULTIPLIER,
        SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    bool quit = false;

    const uint8_t timerHz = 60;
    const uint16_t clockSpeed = 1300;
    const uint8_t maxTimerCount = clockSpeed / timerHz;

    uint8_t timerCount = 0;
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
            {
                char key = event.key.keysym.sym;
                int vkey = CHIP8_MapPhysicalToVirtualKeyboard(&chip8.keyboard, key);
                if (vkey != -1)
                {
                    CHIP8_SetDownPressToVirtualKeyboard(&chip8.keyboard, vkey);
                }
            }
            break;
            case SDL_KEYUP:
            {
                char key = event.key.keysym.sym;
                int vkey = CHIP8_MapPhysicalToVirtualKeyboard(&chip8.keyboard, key);
                if (vkey != -1)
                {
                    CHIP8_SetUpPressToVirtualKeyboard(&chip8.keyboard, vkey);
                }
            }
            break;
            default:
                break;
            }
        }

        uint16_t opCode = CHIP8_GetWordFromMemory(&chip8.memory, chip8.registers.programCounter);
        chip8.registers.programCounter += 2;
        CHIP8_ExecuteOpCode(&chip8, opCode);

        SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0x0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0x0, 0xff, 0x0, 0x0);

        for (int x = 0; x < CHIP8_SCREEN_WIDTH; x++)
        {
            for (int y = 0; y < CHIP8_HEIGHT; y++)
            {
                if (CHIP8_IsPixelSetScreen(&chip8.screen, x, y))
                {
                    SDL_Rect r = {
                        .x = x * CHIP8_WINDOW_MULTIPLIER,
                        .y = y * CHIP8_WINDOW_MULTIPLIER,
                        .w = CHIP8_WINDOW_MULTIPLIER - 1,
                        .h = CHIP8_WINDOW_MULTIPLIER - 1,
                    };
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }

        if (timerCount >= maxTimerCount)
        {
            timerCount = 0;

            if (chip8.registers.delayTimer > 0)
            {
                chip8.registers.delayTimer -= 1;
            }

            if (chip8.registers.soundTimer > 0)
            {
#ifdef _WIN32
                Beep(15000, 1 * chip8.registers.soundTimer);
#endif
                chip8.registers.soundTimer -= 1;
            }
        }
        timerCount++;

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}
