#include "assembly.h"
#include "decoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
    char* desc;
    int8_t size;
} assembly_instructions;


int8_t hex_to_dec(char* str);
void readFile();