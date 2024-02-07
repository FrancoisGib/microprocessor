#include <stdio.h>
#include <inttypes.h>
#include "lib.h"

void fillMemory(uint8_t* RAM,int8_t* hasInstruction, size_t size){
    FILE *input = fopen("boucle.txt", "r");
    FILE *output = fopen("output.s","w");
    fillWithZero(RAM,size);
    if (input == NULL) {
        perror("Error opening file");
    }

    char line[256];
    int8_t isData = 0;
    while (fgets(line, sizeof(line), input) != NULL) {
        int16_t value1;
        int8_t value2, value3, value4;
        if (strstr(line, "FFFF") != NULL) {
            break;
        }
        //pattern matching
        int result = sscanf(line, "%hx: %hhx %hhx %hhx", &value1, &value2, &value3, &value4);
        if(strstr(line,"DATA") != NULL){
            isData = 1;
        }
        if (result >= 2) {
            if(isData){
                hasInstruction[value1] = 2;
            }
            else{
                hasInstruction[value1] = 1;
            }
            RAM[value1] = value2;
            if (result == 3) {
                RAM[value1 + 1] = value3;  
                hasInstruction[value1+1] = 1;
            }
            if(result == 4){
                RAM[value1 + 1] = value3;
                RAM[value1 + 2] = value4;
                hasInstruction[value1+1] = 1;
                hasInstruction[value1+2] = 1;
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
            if ((i + 1) % 2 == 0) {
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