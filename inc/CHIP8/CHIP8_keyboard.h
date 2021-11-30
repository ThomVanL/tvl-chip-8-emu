#ifndef CHIP8_KEYBOARD_H
#define CHIP8_KEYBOARD_H
#include "CHIP8_config.h"
#include <stdbool.h>
#include <assert.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    struct CHIP8_Keyboard
    {
        bool keyboard[CHIP8_TOTAL_KEYS];
        const char *keyboard_map;
    };

    void CHIP8_SetVirtualKeyboardMap(struct CHIP8_Keyboard *vkeyboard, const char *map);
    int CHIP8_MapPhysicalToVirtualKeyboard(struct CHIP8_Keyboard *vkeyboard, char vkey);
    void CHIP8_SetDownPressToVirtualKeyboard(struct CHIP8_Keyboard *vkeyboard, int vkey);
    bool CHIP8_HasDownPressToVirtualKeyboard(struct CHIP8_Keyboard *vkeyboard, int vkey);
    void CHIP8_SetUpPressToVirtualKeyboard(struct CHIP8_Keyboard *vkeyboard, int vkey);

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
