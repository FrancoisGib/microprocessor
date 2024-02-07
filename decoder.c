#include "decoder.h"
#include "assembly.h"

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

int8_t dec_Rn(int8_t Rn){
    printf("Instruction: DEC R%d\n",Rn);
    DEC(Rn);
}
int8_t inc_Rn(int8_t Rn){
    printf("Instruction: INC R%d\n",Rn);
    INC(Rn);
}
int8_t not_Rn(int8_t Rn){
    printf("Instruction: NOT R%d\n",Rn);
    NOT(Rn);
}
int8_t add_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00011000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    printf("Instruction: ADD R%d, R%d\n",Rn,Rm);
    ADD(Rn,Rm);
}
int8_t sub_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00011000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    printf("Instruction: SUB R%d, R%d\n",Rn,Rm);
    SUB(Rn,Rm);
}
int8_t and_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00011000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    printf("Instruction: AND R%d, R%d\n",Rn,Rm);
    AND(Rn,Rm);
}
int8_t swp_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00011000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    printf("Instruction: SWP R%d, R%d\n",Rn,Rm);
    SWP(Rn,Rm);
}
int8_t mv_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b00111000) >> 3;
    int8_t Rm = variablePart & 0b00000111;
    printf("Instruction: MV R%d, R%d\n",Rn,Rm);
    MV(Rn,Rm);
}
int8_t jmp_hhll(int8_t variablePart){
    JMP_HHLL();
    printf("Instruction: JMP %d\n",getHHLL_AS());
}
int8_t jz_hhll(int8_t variablePart){
    JZ_HHLL();
    printf("Instruction: JZ %d\n",getHHLL_AS());
}
int8_t jc_hhll(int8_t variablePart){
    JC_HHLL();
    printf("Instruction: JC %d\n",getHHLL_AS());
}
int8_t jmp_rx0(int8_t variablePart){
    printf("Instruction: JMP RX0\n");
    JMP_RX0();
}
int8_t st_r0_rxn(int8_t variablePart){
    int8_t RXn = (variablePart & 0b00000011);
    printf("Instruction: ST R0, RX%d\n",RXn);
    ST_R0_RXn(RXn);
}
int8_t ld_r0_rxn(int8_t variablePart){
    int8_t RXn = (variablePart & 0b00000011);
    printf("Instruction: LD R0, RX%d\n",RXn);
    LD_R0_RXn(RXn);
}
int8_t st_rn_hhll(int8_t Rn){
    ST_Rn_HHLL(Rn);
    printf("Instruction: ST R%d, %d\n",Rn,getHHLL_AS());
}
int8_t ld_rn_hhll(int8_t Rn){
    LD_RN_HHLL(Rn);
    printf("Instruction: LD R%d, %d\n",Rn,getHHLL_AS());
}
int8_t mv_rn_arg(int8_t Rn){
    MV_Rn_arg(Rn);
    printf("MV R%d, %d\n",Rn,getHHLL_AS());
}


int getVariablePartLength(int8_t bitmask) {
    int length = 0;
    while ((bitmask & 1) == 0) {
        length++;
        bitmask >>= 1;
    }
    return length;
}

void decodeOpcode(int8_t* opcode) {
    char* correspondingInstruction = "Unknown";
    int8_t variablePart = 0;
    for (int i = 0; i < NUM_INSTRUCTIONS; ++i) {
        if ((instructionSet[i].opcode & instructionSet[i].mask) == (*opcode & instructionSet[i].mask)) {
            int variablePartLength = getVariablePartLength(instructionSet[i].mask);
            variablePart = (*opcode & ((1 << variablePartLength) - 1));
            instructionSet[i].operation(variablePart);
            break;
        }
    }
}