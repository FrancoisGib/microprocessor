#include "decoder.h"

instruction_info instructionSet[] = {
    {0b01110000,0b11111111},
    {0b01110001,0b11111111},
    {0b01110010,0b11111111},
    {0b01110011,0b11111111},
    {0b01111011,0b11111100},
    {0b01111111,0b11111100},
    {0b01000111,0b11111000},
    {0b01001111,0b11111000},
    {0b01010111,0b11111000},
    {0b01011111,0b11111000}, 
    {0b01100111,0b11111000},
    {0b01101111,0b11111000},
    {0b10011111,0b11100000},
    {0b10111111,0b11100000},
    {0b11011111,0b11100000},
    {0b11111111,0b11100000},
    {0b00111111,0b11000000}
};

int8_t decodeOpcode(int8_t byte) {
    char* correspondingInstruction = "Unknown";
    int8_t variablePart = 0;
    for (int i = 0; i < NUM_INSTRUCTIONS; ++i) {
        if ((instructionSet[i].opcode & instructionSet[i].mask) == (byte & instructionSet[i].mask)) {
            return i;
        }
    }
}


