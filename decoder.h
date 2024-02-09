#include <stdint.h>
#include <stdio.h>

#define NUM_INSTRUCTIONS 17

typedef struct {
    int8_t opcode;
    int8_t mask;
    char*(*operation)(int8_t variablePart);
} Instruction;


char* dec_Rn(int8_t variablePart);
char* inc_Rn(int8_t variablePart);
char* not_Rn(int8_t variablePart);
char* add_Rn_Rm(int8_t variablePart);
char* sub_Rn_Rm(int8_t variablePart);
char* and_Rn_Rm(int8_t variablePart);
char* swp_Rn_Rm(int8_t variablePart);
char* mv_Rn_Rm(int8_t variablePart);
char* jmp_hhll(int8_t variablePart);
char* jz_hhll(int8_t variablePart);
char* jc_hhll(int8_t variablePart);
char* jmp_rx0(int8_t variablePart);
char* st_r0_rxn(int8_t variablePart);
char* ld_r0_rxn(int8_t variablePart);
char* st_rn_hhll(int8_t variablePart);
char* ld_rn_hhll(int8_t variablePart);
char* mv_rn_arg(int8_t variablePart);

int getVariablePartLength(int8_t bitmask);
char* decodeOpcode(int8_t* opcode);

