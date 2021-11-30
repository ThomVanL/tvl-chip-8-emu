#include "CHIP8.h"

static void CHIP8_ExecuteOpCodePrefix8(struct CHIP8 *chip8, uint16_t x, uint16_t y, uint8_t opCodeType);
static void CHIP8_ExecuteOpCodePrefixF(struct CHIP8 *chip8, uint16_t x, uint16_t opCodeType);

const char CHIP8_DefaultCharacterSet[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void CHIP8_Initialize(struct CHIP8 *chip8)
{
    memset(chip8, 0, sizeof(struct CHIP8));
    memcpy(&chip8->memory.memory, CHIP8_DefaultCharacterSet, sizeof(CHIP8_DefaultCharacterSet));
    chip8->registers.delayTimer = 30;
}

void CHIP8_LoadRomIntoMemory(struct CHIP8 *chip8, const int8_t *romBuffer, size_t romBufferSize)
{
    assert((romBufferSize + CHIP8_PROGRAM_LOAD_ADDR) < CHIP8_MEMORY_MAX_SIZE); // Check whether program fits in memory
    memcpy(&chip8->memory.memory[CHIP8_PROGRAM_LOAD_ADDR], romBuffer, romBufferSize);
    chip8->registers.programCounter = CHIP8_PROGRAM_LOAD_ADDR;
}

int CHIP8_LoadRomFromFile(struct CHIP8 *chip8, char const *filePath)
{
    printf("File to load: %s!\n", filePath);
    FILE *fileptr = fopen(filePath, "rb");
    if (!fileptr)
    {
        printf("Failed to open the ROM!\n");
        return -1;
    }

    fseek(fileptr, 0, SEEK_END);    // Go to end of file
    size_t fileSize = ftell(fileptr); // Give me the position, which is the size of the file
    fseek(fileptr, 0, SEEK_SET);    // Go back to the start

    int8_t fileBuffer[fileSize];
    int fileReadResult = fread(fileBuffer, fileSize, 1, fileptr); //
    fclose(fileptr);

    if (fileReadResult != 1)
    {
        printf("Failed to read the ROM!\n");
        return -1;
    }

    CHIP8_LoadRomIntoMemory(chip8, fileBuffer, fileSize);
    return 0;
}

void CHIP8_ExecuteOpCode(struct CHIP8 *chip8, uint16_t opCode)
{
    uint16_t nnn = opCode & 0x0fff;  // Of opCode's 16 bits: ignore first 4 bits, take last 12 bits;
    uint16_t x = nnn >> 8;           // Of nnn's 16 bits: "remove" last 8 bits, keeping first 8.
    uint16_t y = (nnn & 0x0f0) >> 4; // Of nnn's 16 bits: AND with 0000 1111 0000, keeping middle 4 bits, "remove" last 4 bits.
    uint16_t kk = nnn & 0x0ff;       // Of nnn' 16 bits: AND with 0000 1111 1111, keeping last 8 bits

    switch (opCode & 0xf000) // Of opCode's 16 bits: look at the most significant digit
    {
    case 0x0000:
        switch (opCode & 0x00FF)
        {
        case 0x00E0: // CLS - Clear the display
            CHIP8_ClearScreen(&chip8->screen);
            break;
        case 0x00EE: // RET - Return from a subroutine
            chip8->registers.programCounter = CHIP8_PopStack(chip8);
            break;
        default:
            printf("Unsupported opCode %x\n", opCode);
            break;
        }
        break;
    case 0x1000: // 1nnn - JP addr - Jump to location nnn
        chip8->registers.programCounter = nnn;
        break;
    case 0x2000: // 2nnn - CALL addr - Call subrouting at location nnn
        CHIP8_PushStack(chip8, chip8->registers.programCounter);
        chip8->registers.programCounter = nnn;
        break;
    case 0x3000: // 3xkk - SE Vx, byte -  Skip next instruction if register Vx = kk.
        if (chip8->registers.v[x] == kk)
        {
            chip8->registers.programCounter += 2; // Each instruction is two bytes, main loop will
        }                                         // also +=2 the PC so goes up by 4 in total
        break;
    case 0x4000: // 4xkk - SE Vx, byte -  Skip next instruction if register Vx != kk.
        if (chip8->registers.v[x] != kk)
        {
            chip8->registers.programCounter += 2; // Each instruction is two bytes, main loop will
        }                                         // also +=2 the PC so goes up by 4 in total
        break;
    case 0x5000: // 5xy0 - SE Vx, Vy -  Skip next instruction if register Vx = Vy.
        if (chip8->registers.v[x] == chip8->registers.v[y])
        {
            chip8->registers.programCounter += 2; // Each instruction is two bytes, main loop will
        }                                         // also +=2 the PC so goes up by 4 in total
        break;
    case 0x6000: // 6xkk - LD Vx, byte -  The interpreter puts the value kk into register Vx.
        chip8->registers.v[x] = kk;
        break;
    case 0x7000: // 7xkk - ADD Vx, byte - Set Vx = Vx + kk.
        chip8->registers.v[x] += kk;
        break;
    case 0x8000: //8... codes
    {
        uint8_t opCodeType = nnn & 0x00f;
        CHIP8_ExecuteOpCodePrefix8(chip8, x, y, opCodeType);
    }
    break;
    case 0x9000: // 9xy0 - SNE Vx, Vy -  Skip next instruction if Vx != Vy.
        if (chip8->registers.v[x] != chip8->registers.v[y])
        {
            chip8->registers.programCounter += 2;
        }
        break;
    case 0xA000: // Annn - LD I, addr -  Set I = nnn.
        chip8->registers.i = nnn;
        break;
    case 0xB000: // Bnnn - JP V0, addr -  Jump to location nnn + V0.
        chip8->registers.programCounter = nnn + chip8->registers.v[0x0];
        break;
    case 0xC000: // Cxkk - RND Vx, byte -  Set Vx = random byte AND kk.
        srand(clock());
        chip8->registers.v[x] = (rand() % 255) & kk;
        break;
    case 0xD000: // Dxyn - DRW Vx, Vy, nibble -  Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
    {
        uint8_t n = nnn & 0x00f;
        const uint8_t *sprite = (const uint8_t *)&chip8->memory.memory[chip8->registers.i];
        chip8->registers.v[0xF] = CHIP8_DrawSpriteToScreen(&chip8->screen, chip8->registers.v[x], chip8->registers.v[y], sprite, n);
    }
    break;
    case 0xE000:
        switch (opCode & 0x00FF)
        {
        case 0x9E: // Ex9E - SKP Vx -  Skip next instruction if key with the value of Vx is pressed.
            if (CHIP8_HasDownPressToVirtualKeyboard(&chip8->keyboard, chip8->registers.v[x]))
            {
                chip8->registers.programCounter += 2;
            }
            break;
        case 0xA1: //  ExA1 - SKNP Vx - Skip next instruction if key with the value of Vx is not pressed.
            if (!CHIP8_HasDownPressToVirtualKeyboard(&chip8->keyboard, chip8->registers.v[x]))
            {
                chip8->registers.programCounter += 2;
            }
            break;
        default:
            printf("Unsupported opCode %x\n", opCode);
            break;
        }
        break;
    case 0xF000:
        CHIP8_ExecuteOpCodePrefixF(chip8, x, kk);
        break;
    default:
        break;
    }
}

static void CHIP8_ExecuteOpCodePrefixF(struct CHIP8 *chip8, uint16_t x, uint16_t opCodeType)
{
    switch (opCodeType)
    {
    case 0x07: // Fx07 - LD Vx, DT - Set Vx = delay timer value.
        chip8->registers.v[x] = chip8->registers.delayTimer;
        break;
    case 0x0A: //  Fx0A - LD Vx, K -  Wait for a key press, store the value of the key in Vx.
    {
        SDL_Event event;
        char CHIP8_key = -1;
        while (SDL_WaitEvent(&event))
        {
            if (event.type != SDL_KEYDOWN)
            {
                continue;
            }

            char c = event.key.keysym.sym;
            CHIP8_key = CHIP8_MapPhysicalToVirtualKeyboard(&chip8->keyboard, c);
            if (CHIP8_key != -1)
            {
                break;
            }
        }
        chip8->registers.v[x] = CHIP8_key;
    }
    break;
    case 0x15: // Fx15 - LD DT, Vx -  Set delay timer = Vx.
        chip8->registers.delayTimer = chip8->registers.v[x];
        break;
    case 0x18: //  Fx18 - LD ST, Vx -  Set sound timer = Vx.
        chip8->registers.soundTimer = chip8->registers.v[x];
        break;
    case 0x1E: //   Fx1E - ADD I, Vx -  Set I = I + Vx.
        chip8->registers.i += chip8->registers.v[x];
        break;
    case 0x29: // Fx29 - LD F, Vx -  Set I = location of sprite for digit Vx.
        chip8->registers.i = chip8->registers.v[x] * CHIP8_DEFAULT_SPRITE_HEIGHT;
        break;
    case 0x33: //  Fx33 - LD B, Vx - Store BCD representation of Vx in memory locations I, I+1, and I+2.
    {
        uint8_t hundreds = chip8->registers.v[x] / 100;
        uint8_t tens = chip8->registers.v[x] / 10 % 10;
        uint8_t ones = chip8->registers.v[x] % 10;
        CHIP8_SetByteInMemory(&chip8->memory, chip8->registers.i, hundreds);
        CHIP8_SetByteInMemory(&chip8->memory, chip8->registers.i + 1, tens);
        CHIP8_SetByteInMemory(&chip8->memory, chip8->registers.i + 2, ones);
    }
    break;
    case 0x55: // Fx55 - LD [I], Vx - Store registers V0 through Vx in memory starting at location I.
    {
        for (uint8_t i = 0; i <= x; i++)
        {
            CHIP8_SetByteInMemory(&chip8->memory, chip8->registers.i + i, chip8->registers.v[i]);
        }
    }
    break;
    case 0x65: //  Fx65 - LD Vx, [I] - Read registers V0 through Vx from memory starting at location I.
    {
        for (uint8_t i = 0; i <= x; i++)
        {
            chip8->registers.v[i] = CHIP8_GetByteFromMemory(&chip8->memory, chip8->registers.i + i);
        }
    }
    break;
    default:
        break;
    }
}

static void CHIP8_ExecuteOpCodePrefix8(struct CHIP8 *chip8, uint16_t x, uint16_t y, uint8_t opCodeType)
{
    switch (opCodeType)
    {
    case 0x0: //  8xy0 - LD Vx, Vy -  Set Vx = Vy.
        chip8->registers.v[x] = chip8->registers.v[y];
        break;
    case 0x1: // 8xy1 - OR Vx, Vy -  Set Vx = Vx OR Vy.
        chip8->registers.v[x] |= chip8->registers.v[y];
        break;
    case 0x2: //  8xy2 - AND Vx, Vy -  Set Vx = Vx AND Vy.
        chip8->registers.v[x] &= chip8->registers.v[y];
        break;
    case 0x3: // 8xy3 - XOR Vx, Vy - Set Vx = Vx XOR Vy.break;
        chip8->registers.v[x] ^= chip8->registers.v[y];
        break;
    case 0x4: // 8xy4 - ADD Vx, Vy -  Set Vx = Vx + Vy, set VF = carry.
    {
        uint16_t add = chip8->registers.v[x] + chip8->registers.v[y];
        chip8->registers.v[x] = add;
        if (add > 0xff)
        {
            chip8->registers.v[0xF] = 1;
        }
        else
        {
            chip8->registers.v[0xF] = 0;
        }
    }
    break;
    case 0x5: // 8xy5 - SUB Vx, Vy -  Set Vx = Vx - Vy, set VF = NOT borrow.
        if (chip8->registers.v[x] > chip8->registers.v[y])
        {
            chip8->registers.v[0xF] = 1;
        }
        else
        {
            chip8->registers.v[0xF] = 0;
        }
        chip8->registers.v[x] -= chip8->registers.v[y];
        break;
    case 0x6:                                                   // 8xy6 - SHR Vx {, Vy} -  Set Vx = Vx SHR 1.
        chip8->registers.v[0xF] = chip8->registers.v[x] & 0x01; // set 1 if least significant bit is 1 else set 0
        chip8->registers.v[x] >>= 1;                            // divide by 2
        break;
    case 0x7: // 8xy7 - SUBN Vx, Vy -  Set Vx = Vy - Vx, set VF = NOT borrow.
        if (chip8->registers.v[y] > chip8->registers.v[x])
        {
            chip8->registers.v[0xF] = 1;
        }
        else
        {
            chip8->registers.v[0xF] = 0;
        }
        chip8->registers.v[x] = chip8->registers.v[y] - chip8->registers.v[x];
        break;
    case 0xE:                                                 // 8xyE - SHL Vx {, Vy} -  Set Vx = Vx SHL 1.
        chip8->registers.v[0xF] = chip8->registers.v[x] >> 7; // set 1 of most significant bit is 1 else set 0
        chip8->registers.v[x] <<= 1;                          // multiply by 2
        break;
    default:
        printf("Unsupported opCode 8%x%x%x\n", x, y, opCodeType);
        break;
    }
}
