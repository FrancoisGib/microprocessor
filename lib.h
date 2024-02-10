#include "assembly.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int* fillMemory(uint8_t* RAM,int8_t* hasInstruction, size_t size);

void displayMemory(uint8_t* RAM,int8_t* hasInstruction,size_t size);

void fillWithZero(int8_t* hasInstruction, size_t size);

