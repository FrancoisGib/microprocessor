#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "microprocessor.h"

typedef struct {
    int8_t byte_number;
    int8_t mask;
    int8_t shift;
} argument;

typedef struct {
    int8_t code;
    int8_t mask;
    int8_t nb_bytes;
    char* assembly_string;
    int8_t string_size;
    int8_t nb_args;
    argument arguments[3];
} instruction_informations;

typedef struct {
    int8_t opcode;
    int16_t memory_address;
    int8_t* bytes;
    int8_t* args;
} instruction_s;

int8_t decodeOpcode(int8_t byte);
int16_t hex_to_dec(char *hex);
void test_all_instructions();
void readFile(char* path);
int main(int argc, char* argv[]);