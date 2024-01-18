#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>

typedef struct {
    int32_t value;   // La valeur brute de l'instruction
    int8_t length;   // La longueur de l'instruction en bits
} Instruction;


void decodeInstruction(Instruction instruction) {
    // Traitez chaque segment de l'instruction ici en fonction de sa longueur
    for (int i = instruction.length - 8; i >= 0; i -= 8) {
        int8_t segment = (instruction.value >> i) & 0xFF;
        decode(segment);
    }
}


void decode(int8_t segment){
    char instructionBinary[9];
    for (int i = 7; i >= 0; i--) {
        instructionBinary[7 - i] = ((segment & (1 << i)) ? '1' : '0');
    }
    instructionBinary[8] = '\0';
    printf("Segment decoded: %s\n",instructionBinary);
}

int main(int argc, char const *argv[])
{   
    // Exemple d'utilisation avec une instruction de 24 bits
    Instruction instruction24;
    instruction24.value = 0x49;  // Remplacez cette valeur par votre instruction réelle
    instruction24.length = 8;

    // Décodage de l'instruction
    decodeInstruction(instruction24);
}
