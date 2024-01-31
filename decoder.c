#include "decoder.h"

Instruction instructionSet[] = {
    {0b01110000,0b11111111,jmp_hhll, 3},
    {0b01110001,0b11111111,jz_hhll, 3},
    {0b01110010,0b11111111,jc_hhll, 3},
    {0b01110011,0b11111111,jmp_rx0, 1},
    {0b01111011,0b11111100,st_r0_rxn, 1},
    {0b01111111,0b11111100,ld_r0_rxn, 1},
    {0b01000111,0b11111000,st_rn_hhll, 3},
    {0b01001111,0b11111000,ld_rn_hhll, 2},
    {0b01010111,0b11111000,mv_rn_arg, 1},
    {0b01011111,0b11111000,dec_Rn, 1}, 
    {0b01100111,0b11111000,inc_Rn, 1},
    {0b01101111,0b11111000,not_Rn, 1},
    {0b10011111,0b11100000,add_Rn_Rm, 2},
    {0b10111111,0b11100000,sub_Rn_Rm, 1},
    {0b11011111,0b11100000,and_Rn_Rm, 1},
    {0b11111111,0b11100000,swp_Rn_Rm, 1},
    {0b00111111,0b11000000,mv_Rn_Rm, 1}
};

void nop(int8_t variablePart){
    printf("NOP\n");
}

void dec_Rn(int8_t variablePart){
    printf("DEC R%d\n",variablePart);
}
void inc_Rn(int8_t variablePart){
    printf("INC R%d\n",variablePart);
}
void not_Rn(int8_t variablePart){
    printf("NOT R%d\n",variablePart);
}
void add_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00011000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    printf("ADD R%d, R%d\n",Rn,Rm);
}
void sub_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00011000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    printf("SUB R%d, R%d\n",Rn,Rm);
}
void and_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00011000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    printf("AND R%d, R%d\n",Rn,Rm);
}
void swp_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00011000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    printf("SWP R%d, R%d\n",Rn,Rm);
}
void mv_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00111000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    printf("MV R%d, R%d\n",Rn,Rm);
}
void jmp_hhll(int8_t variablePart){
    printf("JMP HHLL\n");
}
void jz_hhll(int8_t variablePart){
    printf("JZ HHLL\n");
}
void jc_hhll(int8_t variablePart){
    printf("JC HHLL\n");
}
void jmp_rx0(int8_t variablePart){
    printf("JMP RX0\n");
}
void st_r0_rxn(int8_t variablePart){
    int8_t RXn = (variablePart & 0b00000011);
    printf("ST R0, RX%d\n",RXn);
}
void ld_r0_rxn(int8_t variablePart){
    int8_t RXn = (variablePart & 0b00000011);
    printf("LD R0, RX%d\n",RXn);
}
void st_rn_hhll(int8_t variablePart){
    printf("ST R%d, HHLL\n",variablePart);
}
void ld_rn_hhll(int8_t variablePart){
    printf("LD R%d, HHLL\n",variablePart);
}
void mv_rn_arg(int8_t variablePart){
    printf("mv R%d, arg#\n",variablePart);
}

int8_t getVariablePartLength(int8_t bitmask) {
    int8_t length = 0;
    while ((bitmask & 1) == 0) {
        length++;
        bitmask >>= 1;
    }
    return length;
}

int8_t decodeOpcode(int8_t opcode) {
    char* correspondingInstruction = "Unknown";
    int8_t variablePart = 0;
    for (int i = 0; i < NUM_INSTRUCTIONS; ++i) {
        if ((instructionSet[i].opcode & instructionSet[i].mask) == (opcode & instructionSet[i].mask)) {
            int variablePartLength = getVariablePartLength(instructionSet[i].mask);
            variablePart = (opcode & ((1 << variablePartLength) - 1));
            //instructionSet[i].operation(variablePart);
            return i;
        }
    }
}


