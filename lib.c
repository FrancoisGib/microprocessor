#include <stdio.h>
#include <inttypes.h>
#include "lib.h"


FILE *output = fopen("output.s","w");

void fillMemory(uint8_t* RAM,int8_t* hasInstruction, size_t size){
    FILE *input = fopen("boucle.txt", "r");
    fillWithZero(RAM,size);
    if (input == NULL) {
        perror("Error opening file");
    }

    char line[256];
    int8_t isData = 0;
    while (fgets(line, sizeof(line), input) != NULL) {
        int16_t address;
        int8_t value1, value2, value3;
        if (strstr(line, "FFFF") != NULL) {
            break;
        }
        //pattern matching
        int result = sscanf(line, "%hx: %hhx %hhx %hhx", &address, &value1, &value2, &value3);
        if(strstr(line,"DATA") != NULL){
            isData = 1;
        }
        if (result >= 2) {
            if(isData){
                hasInstruction[address] = 2;
            }
            else{
                hasInstruction[address] = 1;
            }
            RAM[address] = value1;
            if (result >= 3) {
                RAM[address + 1] = value2;  
                hasInstruction[address+1] = 1;
            }
            if(result == 4){
                RAM[address + 2] = value3;
                hasInstruction[address+2] = 1;
            }
        }
    }
    fclose(input);
}

void fillWithZero(int8_t* hasInstruction, size_t size){
    for (size_t i = 0; i < size; i++)
    {
        hasInstruction[i] = 0;
    }
    
}

void displayInstructions(uint8_t* RAM,int8_t* hasInstruction,size_t size){
    printf("Instructions\n");
    printf("Address | Data\n");
    printf("-----------------\n");
    for (size_t i = 0; i < size; i++)
    {
        if(hasInstruction[i] == 1){
            printf("[%ld | %02X]  ",i,RAM[i]);
            if ((i + 2) % 3 == 0) {
                printf("\n");
            }
        }
    }
    printf("\n");
}



void displayData(uint8_t* RAM,int8_t* hasInstruction,size_t size){
    printf("Data\n");
    printf("-----------------\n");
    for (size_t i = 0; i < size; i++)
    {
        if(hasInstruction[i] == 2){
            printf("[%ld | %02X]  ",i,RAM[i]);
            if ((i + 1) % 2 == 0) {
                printf("\n");
            }
        }
    }
    printf("\n");
}
void displayMemory(uint8_t* RAM,int8_t* hasInstruction,size_t size){
    displayInstructions(RAM,hasInstruction,size);
    displayData(RAM,hasInstruction,size);
}