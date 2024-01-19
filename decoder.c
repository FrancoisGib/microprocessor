#include <stdio.h>
#include <stdint.h>

#define NUM_INSTRUCTIONS 17

typedef struct {
    int8_t opcode;
    int8_t mask;
    void(*operation)(int8_t variablePart);
} Instruction;

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
    //Décalage vers le bit de poids faible sinon marche pas
    int8_t Rn = (variablePart & 0b11000000) >> 5;
    //Déjà dans le bit de poids faible
    int8_t Rm = variablePart & 0b00111111;
    printf("ADD R%d, R%d\n",Rn,Rm);
}
void sub_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b11000000) >> 5;
    int8_t Rm = variablePart & 0b00111111;
    printf("SUB R%d, R%d\n",Rn,Rm);
}
void and_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b11000000) >> 5;
    int8_t Rm = variablePart & 0b00111111;
    printf("AND R%d, R%d\n",Rn,Rm);
}
void swp_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b11000000) >> 5;
    int8_t Rm = variablePart & 0b00111111;
    printf("SWP R%d, R%d\n",Rn,Rm);
}
void mv_Rn_Rm(int8_t variablePart){
    int8_t Rn = (variablePart & 0b11100000) >> 5;
    int8_t Rm = variablePart & 0b00011111;
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
    int8_t Rn = (variablePart & 0b11000000) >> 6;
    printf("ST R%d, RXn\n",Rn);
}
void ld_r0_rxn(int8_t variablePart){
    int8_t Rn = (variablePart & 0b11000000) >> 6;
    printf("LD R%d, RXn\n",Rn);
}
void st_rn_hhll(int8_t variablePart){
    int8_t Rn = (variablePart & 0b11100000) >> 5;
    printf("ST R%d, HHLL\n",Rn);
}
void ld_rn_hhll(int8_t variablePart){
    int8_t Rn = (variablePart & 0b11100000) >> 5;
    printf("LD R%d, HHLL\n",Rn);
}
void mv_rn_arg(int8_t variablePart){
    int8_t Rn = (variablePart & 0b11100000) >> 5;
    printf("mv R%d, arg#\n",Rn);
}


static Instruction instructionSet[NUM_INSTRUCTIONS] = {
    {0b01110000,0b11111111,jmp_hhll},
    {0b01110001,0b11111111,jz_hhll},
    {0b01110010,0b11111111,jc_hhll},
    {0b01110011,0b11111111,jmp_rx0},
    {0b01110111,0b11111100,st_r0_rxn},
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

int main() {
    unsigned char opcode1 = 0b01011010; 
    unsigned char opcode2 = 0b01100011;
    unsigned char opcode3 = 0b10001110;

    decodeOpcode(&opcode1);
    decodeOpcode(&opcode2);
    decodeOpcode(&opcode3);

    return 0;
}
