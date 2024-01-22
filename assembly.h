#include <stdint.h>
#include "microprocessor.h"

typedef struct {
    int8_t opcode;
    int8_t mask;
    void(*operation)(int8_t variablePart);
    int8_t size;
} Instruction;

void ADD(int8_t Rn, int8_t Rm);
void SUB(int8_t Rn, int8_t Rm);
void MULT(int8_t Rn, int8_t Rm);
void DIV(int8_t Rn, int8_t Rm);
void MV(int8_t Rn, int8_t Rm);
void SWP(int8_t Rn, int8_t Rm);