#include <stdint.h>
#include <stdio.h>

#define NUM_INSTRUCTIONS 17

typedef struct {
    int8_t opcode;
    int8_t mask;
} instruction_info;

int8_t decodeOpcode(int8_t opcode);