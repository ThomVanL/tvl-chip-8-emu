#include "CHIP8_keyboard.h"

static void CHIP8_IsVkeyInVkeyboardBounds(int vkey)
{
    assert(vkey >= 0 && vkey < CHIP8_TOTAL_KEYS);
}

void CHIP8_SetVirtualKeyboardMap(struct CHIP8_Keyboard *vkeyboard, const char *map)
{
    vkeyboard->keyboard_map = map;
}

int CHIP8_MapPhysicalToVirtualKeyboard(struct CHIP8_Keyboard *vkeyboard, char vkey)
{
    for (int i = 0; i < CHIP8_TOTAL_KEYS; i++)
    {
        if (vkeyboard->keyboard_map[i] == vkey)
        {
            return i;
        }
    }
    return -1;
}
void CHIP8_SetDownPressToVirtualKeyboard(struct CHIP8_Keyboard *vkeyboard, int vkey)
{
    CHIP8_IsVkeyInVkeyboardBounds(vkey);
    vkeyboard->keyboard[vkey] = true;
}
void CHIP8_SetUpPressToVirtualKeyboard(struct CHIP8_Keyboard *vkeyboard, int vkey)
{
    CHIP8_IsVkeyInVkeyboardBounds(vkey);
    vkeyboard->keyboard[vkey] = false;
}
bool CHIP8_HasDownPressToVirtualKeyboard(struct CHIP8_Keyboard *vkeyboard, int vkey)
{
    CHIP8_IsVkeyInVkeyboardBounds(vkey);
    return vkeyboard->keyboard[vkey];
}
