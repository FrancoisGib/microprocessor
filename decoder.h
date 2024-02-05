#include <stdint.h>
#include <stdio.h>

#define NUM_INSTRUCTIONS 17

typedef struct{
    int8_t start;
    int8_t end;
    int8_t nb_Bit;
} MinimalBit;

typedef struct {
    int8_t opcode;
    int8_t mask;
    int8_t(*operation)(int8_t variablePart);
} Instruction;


int8_t dec_Rn(int8_t variablePart);
int8_t inc_Rn(int8_t variablePart);
int8_t not_Rn(int8_t variablePart);
int8_t add_Rn_Rm(int8_t variablePart);
int8_t sub_Rn_Rm(int8_t variablePart);
int8_t and_Rn_Rm(int8_t variablePart);
int8_t swp_Rn_Rm(int8_t variablePart);
int8_t mv_Rn_Rm(int8_t variablePart);
int8_t jmp_hhll(int8_t variablePart);
int8_t jz_hhll(int8_t variablePart);
int8_t jc_hhll(int8_t variablePart);
int8_t jmp_rx0(int8_t variablePart);
int8_t st_r0_rxn(int8_t variablePart);
int8_t ld_r0_rxn(int8_t variablePart);
int8_t st_rn_hhll(int8_t variablePart);
int8_t ld_rn_hhll(int8_t variablePart);
int8_t mv_rn_arg(int8_t variablePart);

int getVariablePartLength(int8_t bitmask);
void decodeOpcode(int8_t* opcode);