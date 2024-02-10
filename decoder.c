#include "decoder.h"
#include <stdio.h>
#include "assembly.h"
#include <string.h>
#include <stdlib.h>


Instruction instructionSet[NUM_INSTRUCTIONS] = {
    {0b01110000,0b11111111,jmp_hhll},
    {0b01110001,0b11111111,jz_hhll},
    {0b01110010,0b11111111,jc_hhll},
    {0b01110011,0b11111111,jmp_rx0},
    {0b01111011,0b11111100,st_r0_rxn},
    {0b01111111,0b11111100,ld_r0_rxn},
    {0b01000111,0b11111000,st_rn_hhll},
    {0b01001111,0b11111000,ld_rn_hhll},
    {0b01010111,0b11111000,mv_rn_arg},
    {0b01011111,0b11111000,dec_Rn}, 
    {0b01100111,0b11111000,inc_Rn},
    {0b01101111,0b11111000,not_Rn},
    {0b10011111,0b11100000,add_Rn_Rm},
    {0b10111111,0b11100000,sub_Rn_Rm},
    {0b11011111,0b11100000,and_Rn_Rm},
    {0b11111111,0b11100000,swp_Rn_Rm},
    {0b00111111,0b11000000,mv_Rn_Rm}
};

char* dec_Rn(int8_t Rn){
    DEC(Rn);
    printf("Instruction: DEC R%d\n",Rn);
    char* result = malloc(100);
    sprintf(result,"DEC R%d\n",Rn);
    return result;
}

char* inc_Rn(int8_t Rn){
    INC(Rn);
    printf("Instruction: INC R%d\n",Rn);
    char* result = malloc(100);
    sprintf(result,"INC R%d\n",Rn);
    return result;
}

char* not_Rn(int8_t Rn){
    NOT(Rn);
    printf("Instruction: NOT R%d\n",Rn);
    char* result = malloc(100);
    sprintf(result,"NOT R%d\n",Rn);
    return result;
}
char* add_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00011000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    ADD(Rn,Rm);
    printf("Instruction: ADD R%d, R%d\n",Rn,Rm);
    char* result = malloc(100);
    sprintf(result,"ADD R%d, R%d\n",Rn,Rm);
    return result;
}
char* sub_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00011000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    SUB(Rn,Rm);
    printf("Instruction: SUB R%d, R%d\n",Rn,Rm);
    char* result = malloc(100);
    sprintf(result,"SUB R%d, R%d\n",Rn,Rm);
    return result;
}
char* and_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00011000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    AND(Rn,Rm);
    printf("Instruction: AND R%d, R%d\n",Rn,Rm);
    char* result = malloc(100);
    sprintf(result,"AND R%d, R%d\n",Rn,Rm);
    return result;
}
char* swp_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00011000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    SWP(Rn,Rm);
    printf("Instruction: SWP R%d, R%d\n",Rn,Rm);
    char* result = malloc(100);
    sprintf(result,"SWP R%d, R%d\n",Rn,Rm);
    return result;
}
char* mv_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00111000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    MV(Rn,Rm);
    printf("Instruction: MV R%d, R%d\n",Rn,Rm);
    char* result = malloc(100);
    sprintf(result,"MV R%d, R%d\n",Rn,Rm);
    return result;
}
char* jmp_hhll(int8_t variablePart){
    JMP_HHLL();
    printf("Instruction: JMP %d\n",getHHLL_AS());
    char* result = malloc(100);
    sprintf(result,"JMP %d\n",getHHLL_AS());
    return result;
}
char* jz_hhll(int8_t variablePart){
    JZ_HHLL();
    printf("Instruction: JZ %d\n",getHHLL_AS());
    char* result = malloc(100);
    sprintf(result,"JZ %d\n",getHHLL_AS());
    return result;
}
char* jc_hhll(int8_t variablePart){
    JC_HHLL();
    printf("Instruction: JC %d\n",getHHLL_AS());
    char* result = malloc(100);
    sprintf(result,"JC %d\n",getHHLL_AS());
    return result;
}
char* jmp_rx0(int8_t variablePart){
    JMP_RX0();
    printf("Instruction: JMP RX0\n");
    char* result = malloc(100);
    strcpy(result,"JMP RX0\n");
    return result;
}
char* st_r0_rxn(int8_t variablePart){
    int8_t RXn = (variablePart & 0b00000011);
    ST_R0_RXn(RXn);
    printf("Instruction: ST R0, RX%d\n",RXn);
    char* result = malloc(100);
    sprintf(result,"ST R0, RX%d\n",RXn);
    return result;
}
char* ld_r0_rxn(int8_t variablePart){
    int8_t RXn = (variablePart & 0b00000011);
    LD_R0_RXn(RXn);
    printf("Instruction: LD R0, RX%d\n",RXn);
    char* result = malloc(100);
    sprintf(result,"LD R0, RX%d\n",RXn);
    return result;
}
char* st_rn_hhll(int8_t Rn){
    ST_Rn_HHLL(Rn);
    printf("Instruction: ST R%d, %d\n",Rn,getHHLL_AS());
    char* result = malloc(100);
    sprintf(result,"ST R%d, %d\n",Rn,getHHLL_AS());
    return result;
}
char* ld_rn_hhll(int8_t Rn){
    LD_RN_HHLL(Rn);
    printf("Instruction: LD R%d, %d\n",Rn,getHHLL_AS());
    char* result = malloc(100);
    sprintf(result,"LD R%d, %d\n",Rn,getHHLL_AS());
    return result;
}
char* mv_rn_arg(int8_t Rn){
    MV_Rn_arg(Rn);
    printf("Instruction: MV R%d, %d\n",Rn,getHHLL_AS());
    char* result = malloc(100);
    sprintf(result,"MV R%d, %d\n",Rn,getHHLL_AS());
    return result;
}

int getVariablePartLength(int8_t bitmask) {
    int length = 0;
    while ((bitmask & 1) == 0) {
        length++;
        bitmask >>= 1;
    }
    return length;
}

char* decodeOpcode(int8_t* opcode) {
    char* correspondingInstruction = "Unknown";
    int8_t variablePart = 0;
    for (int i = 0; i < NUM_INSTRUCTIONS; ++i) {
        if ((instructionSet[i].opcode & instructionSet[i].mask) == (*opcode & instructionSet[i].mask)){
            int variablePartLength = getVariablePartLength(instructionSet[i].mask);
            variablePart = (*opcode & ((1 << variablePartLength) - 1));
            return instructionSet[i].operation(variablePart);
        }
    }
}