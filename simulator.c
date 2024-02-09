#include "lib.h"
#include"decoder.h"
#include"microprocessor.h"


int findInstructionStart(int8_t* hasInstruction){
    for (int i = 0; i < 1024; i++){
        if(hasInstruction[i] != 0){
            return i;
        }
    }
    return -1;
}

int findInstructionEnd(int8_t* hasInstruction,int start){
    for(int i = start; i < 1024; i++){
        if(hasInstruction[i] != 1){
            return i;
        }
    }
}
void launchInitPhase(microprocessor_t* proc){
    fillMemory(proc->ram,proc->hasInstruction,1024);
    displayMemory(proc->ram,proc->hasInstruction,1024);
}

void fetchInstruction(microprocessor_t* proc){
    PCout();ALin();read();
    DLout();IRin();
}

char* decodeInstruction(microprocessor_t* proc){
    return decodeOpcode(&proc->IR);
}

void displayRegisters(microprocessor_t* proc){
    printf("Registers: ");
    for (size_t i = 0; i < 8; i++)
    {
        printf("[R%ld | %d]  ",i,proc->R[i]);
    }
    printf("\n");
}
void startSimulation(){
    microprocessor_t* proc = getMicroProcessor();
    launchInitPhase(proc);
    int start = findInstructionStart(proc->ram);
    int end = findInstructionEnd(proc->hasInstruction,start);

    if(start != -1){
        proc->PC = start;
        FILE *output = fopen("output.s","w");
        printf("-----------------\n");
        // while(proc->PC != end){
        //     fetchInstruction(proc);
        //     decodeInstruction(proc);
        //     displayRegisters(proc);
        //     // printf("PC After instruction: %d\n",proc->PC);            
        // }
        fetchInstruction(proc);
        char* str = decodeInstruction(proc);
        fwrite(str,1,strlen(str),output);
        free(str);
        displayRegisters(proc);
    }
}
