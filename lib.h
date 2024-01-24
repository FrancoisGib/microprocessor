#include "assembly.h"
#include "decoder.h"
#include <stdlib.h>
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
    int8_t* bytes;
    int8_t nb_bytes;
} instructions_details;

int16_t hex_to_dec(char* str);
void readFile(char* path);
instructions_details* decodeInstructionArguments(int8_t first_byte, FILE* input, FILE* output);
void decodeWhenInstructionNameLengthEqualsEight(int8_t first_byte, instructions_details* details, FILE* input, FILE* output);
void decodeWhenInstructionNameLengthEqualsFive(int8_t first_byte, instructions_details* details, FILE* input, FILE* output);
void decodeWhenInstructionNameLengthEqualsThree(int8_t first_byte, instructions_details* details, FILE* output);
void decodeWhenInstructionNameLengthEqualsTwo(int8_t first_byte, instructions_details* details, FILE* output);
void decodeWhenInstructionNameLengthEqualsSix(int8_t first_byte, instructions_details* details, FILE* output);