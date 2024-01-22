#include "decoder.h"

Instruction instructionSet[NUM_INSTRUCTIONS] = {
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

int8_t dec_Rn(int8_t variablePart){
    printf("DEC R%d\n",variablePart);
}
int8_t inc_Rn(int8_t variablePart){
    printf("INC R%d\n",variablePart);
}
int8_t not_Rn(int8_t variablePart){
    printf("NOT R%d\n",variablePart);
}
int8_t add_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00011000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    printf("ADD R%d, R%d\n",Rn,Rm);
}
int8_t sub_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00011000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    printf("SUB R%d, R%d\n",Rn,Rm);
}
int8_t and_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00011000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    printf("AND R%d, R%d\n",Rn,Rm);
}
int8_t swp_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00011000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    printf("SWP R%d, R%d\n",Rn,Rm);
}
int8_t mv_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00111000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    printf("MV R%d, R%d\n",Rn,Rm);
}
int8_t jmp_hhll(int8_t variablePart){
    printf("JMP HHLL\n");
}
int8_t jz_hhll(int8_t variablePart){
    printf("JZ HHLL\n");
}
int8_t jc_hhll(int8_t variablePart){
    printf("JC HHLL\n");
}
int8_t jmp_rx0(int8_t variablePart){
    printf("JMP RX0\n");
}
int8_t st_r0_rxn(int8_t variablePart){
    int8_t RXn = (variablePart & 0b00000011);
    printf("ST R0, RX%d\n",RXn);
}
int8_t ld_r0_rxn(int8_t variablePart){
    int8_t RXn = (variablePart & 0b00000011);
    printf("LD R0, RX%d\n",RXn);
}
int8_t st_rn_hhll(int8_t variablePart){
    printf("ST R%d, HHLL\n",variablePart);
}
int8_t ld_rn_hhll(int8_t variablePart){
    printf("LD R%d, HHLL\n",variablePart);
}
int8_t mv_rn_arg(int8_t variablePart){
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
            instructionSet[i].operation(variablePart);
            return i;
        }
    }
}


