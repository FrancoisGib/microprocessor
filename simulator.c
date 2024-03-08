#include "simulator.h"
#include <ctype.h>
#define MAX_COMMAND_LENGTH 100
#define MAX_BREAKPOINTS 10
int breakpointCounter = 0;
info_break info[10];
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

char* getRegisters(microprocessor_t* proc){
    char* result = malloc(100);
    result[0] = '\0';
    for (size_t i = 0; i < 8; i++)
    {
        char temp[100];
        sprintf(temp,"[R%ld | %d]  ",i,proc->R[i]);
        strcat(result,temp);
    }
    return result;
}
void displayAsmInstructions(microprocessor_t* proc,int start,int end,int instructionLength){
    proc->PC = start;
    int counter = 1;
    while(proc->PC != end){
        fetchInstruction(proc);
        char* str = decodeInstruction(proc);
        if(counter < instructionLength){
            printf("[%d]: %s\n",counter,str);
            counter++;
        }
        free(str);           
    }
}

//Ouais bon il ya un début à tout God Bless les malloc et bonne lecture
void launchInDebugMode(microprocessor_t* proc){
    int* instructionLength = fillMemory(proc->ram,proc->hasInstruction,1024);
    int capacity = 1;// capacité initial
    int size = 0;// nombre d'element à l'instant T
    int start = findInstructionStart(proc->ram);
    int end = findInstructionEnd(proc->hasInstruction,start);
    debuggerRegistry* registry = (debuggerRegistry*)malloc(sizeof(debuggerRegistry));
    if(start != -1){
        //On pointe vers la première addresse de la RAM
        proc->PC = start;
        printf("-----------------\n");
        while(proc->PC != end){
            fetchInstruction(proc);
            char* str = decodeInstruction(proc);
            if(size >= capacity){
                capacity*=2;
                //On augmente la capacité de lu registre, ici on ne perds pas les données vu que l'on utilise realloc
                debuggerRegistry* temp = realloc(registry,sizeof(debuggerRegistry) * capacity);
                if(temp == NULL){
                    fprintf(stderr, "Memory reallocation failed\n");
                    //On free les mémoires déja allouer
                    for (int i = 0; i < size; ++i) {
                        free(registry[i].instruction);
                        free(registry[i].register_);
                    }
                    free(registry);
                    return;
                }
                registry = temp;//maj du registre
            }
            registry[size].idx = size;
            registry[size].instruction = strdup(str);//askip il faut reallouer de la mémoire ici va comprendre
            char * register_ = getRegisters(proc);
            registry[size].register_ =  strdup(register_);
            free(str);
            free(register_);       
            //On increment le nombre l'élément
            size++;
        }
    }
    runDebugModeController(registry,size);
    //On fait free toute la memoire que l'on a allouer dynamiquement
    for (int i = 0; i < size; i++)
    {
        free(registry[i].instruction);
        free(registry[i].register_);
    }
    free(registry);
    free(instructionLength);
}

void handleRun(microprocessor_t* proc){
    while
}
void launchDebugModev2(microprocessor_t* proc){
    int* instructionLength = fillMemory(proc->ram,proc->hasInstruction,1024);
    int start = findInstructionStart(proc->ram);
    int end = findInstructionEnd(proc->hasInstruction,start);
    char cmd[MAX_COMMAND_LENGTH];
    if(start != -1){
        displayAsmInstructions(proc,start,end,*instructionLength);
        proc->PC = start;
        while(1){
            printf("> ");
            scanf(" %[^\n]", cmd);
            handleBreakPoint(cmd,proc,*instructionLength,start);
            handleRun(proc);
            if (strcmp(cmd, "quit") == 0) {
            printf("Exiting debugger.\n");
            break;
            }
        }
    }
    free(instructionLength);
}


void handleBreakPoint(const char* cmd,microprocessor_t* proc,int size,int start){
    if (strncmp(cmd, "break ", 6) == 0) {
        int breakpoint = atoi(cmd+ 6);
        if(breakpoint != 0){
            int16_t addr = start + (breakpoint -1);
            if(breakpoint < size){
                info[breakpointCounter].num  = breakpointCounter+1;
                info[breakpointCounter].addr = addr;
                info[breakpointCounter].line = breakpoint;
                printf("Breakpoint %d at 0x%hx: line %d\n",++breakpointCounter,addr,breakpoint);
            }
            else{
                printf("Invalid input: Breakpoint exceeds maximum allowed value\n");
            }
        }
        else{
            printf("Invalid input: Not an integer\n");
        }
    }
}

//affichage de du mode debugger
void runDebugModeController(debuggerRegistry* registry,int size){
    printf("Index | Instruction\n");
    printf("-----------------\n");
    char input[20];
    displayDebugMode(registry,size);
    while(1){
    printf("Enter the index you want to break at: \n");
        scanf("%s",input);
        int breakpoint = atoi(input);
        if(breakpoint != 0 || strcmp(input,"0") == 0){
            if(breakpoint < size){
                printf("%s\n",registry[breakpoint].register_);
            }
            else{
                printf("Invalid input: Breakpoint exceeds maximum allowed value\n");
            }
        }   
        else{
            printf("Invalid input: Not an integer\n");
        }
    }
}

void displayDebugMode(debuggerRegistry* registry, int size){
        for (int i = 0; i < size; i++)
    {
        printf("%d: %s",i,registry[i].instruction);
    }
}

void launchInNormalMode(microprocessor_t* proc){
    int* instructionLength = fillMemory(proc->ram,proc->hasInstruction,1024);
    displayMemory(proc->ram,proc->hasInstruction,1024);
    int start = findInstructionStart(proc->ram);
    int end = findInstructionEnd(proc->hasInstruction,start);
    if(start != -1){
        proc->PC = start;
        FILE *output = fopen("output.s","w");
        printf("-----------------\n");
        int counter = 0;
        while(proc->PC != end){
            fetchInstruction(proc);
            char* str = decodeInstruction(proc);
            if(counter < *instructionLength){
                fwrite(str,1,strlen(str),output);
                counter++;
            }
            free(str);           
        }
        displayRegisters(proc); 
        free(instructionLength);
    }
}

int askMode(){
    char response[10];
    printf("Do you want to launch your program in Debug mode? (yes/no): ");
    scanf("%s",response);

    for (int i = 0; response[i]; i++) {
        response[i] = tolower(response[i]);
    }

    if(strcmp(response, "yes") == 0 || strcmp(response,"y") == 0){
        return 1;
    }
    return 0;
}

void startSimulation(){
    microprocessor_t* proc = getMicroProcessor();
    int debugMode = askMode();
    if(debugMode){
        launchDebugModev2(proc);
    }
    else{
        launchInNormalMode(proc);
    }
}