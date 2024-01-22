#include <stdint.h>
#include <stdio.h>

#define NUM_INSTRUCTIONS 17

typedef struct {
    int8_t opcode;
    int8_t mask;
    void(*operation)(int8_t variablePart);
} Instruction;

void dec_Rn(int8_t variablePart);
void inc_Rn(int8_t variablePart);
void not_Rn(int8_t variablePart);
void add_Rn_Rm(int8_t variablePart);
void sub_Rn_Rm(int8_t variablePart);
void and_Rn_Rm(int8_t variablePart);
void swp_Rn_Rm(int8_t variablePart);
void mv_Rn_Rm(int8_t variablePart);
void jmp_hhll(int8_t variablePart);
void jz_hhll(int8_t variablePart);
void jc_hhll(int8_t variablePart);
void jmp_rx0(int8_t variablePart);
void st_r0_rxn(int8_t variablePart);
void ld_r0_rxn(int8_t variablePart);
void st_rn_hhll(int8_t variablePart);
void ld_rn_hhll(int8_t variablePart);
void mv_rn_arg(int8_t variablePart);

int getVariablePartLength(int8_t bitmask);
void decodeOpcode(int8_t* opcode);