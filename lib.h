#include "assembly.h"
#include "decoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

typedef struct {
    char* desc;
    int8_t instruction_name_size;
    int nb_args;
} assembly_instructions;

typedef struct {
    int8_t opcode;    
    int8_t nb_args;
    int8_t* args;
} instructions_details;

int8_t hex_to_dec(char* str);
void readFile(char* path);
instructions_details* decodeInstructionArguments(int8_t first_byte, FILE* input);
void decodeWhenInstructionNameLengthEqualsEight(instructions_details* details, FILE* input);
void decodeWhenInstructionNameLengthEqualsFive(int8_t first_byte, instructions_details* details, FILE* input);
void decodeWhenInstructionNameLengthEqualsThree(int8_t first_byte, instructions_details* details);
void decodeWhenInstructionNameLengthEqualsTwo(int8_t first_byte, instructions_details* details);
void decodeWhenInstructionNameLengthEqualsSix(int8_t first_byte, instructions_details* details);