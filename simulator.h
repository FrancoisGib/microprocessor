#include"microprocessor.h"
#include "lib.h"
#include"decoder.h"

typedef struct {
    int idx;
    char* instruction;
    char* register_;
} debuggerRegistry;

typedef struct {
    int num;
    int16_t addr;
    int line;
} info_break;

typedef struct node_s node_t;

struct node_s {
    void* ptr;
    node_t* next;
};

typedef struct {
    node_t* root;
} lifo_t;

void launchInitPhase(microprocessor_t* proc);
void fetchInstruction(microprocessor_t* proc);
char* decodeInstruction(microprocessor_t* proc);
void displayRegisters(microprocessor_t* proc);
void startSimulation();
void launchInDebugMode(microprocessor_t* proc);
void launchInNormalMode(microprocessor_t* proc);
int askMode();
void runDebugModeController(debuggerRegistry* registry,int size);
void displayDebugMode(debuggerRegistry* registry, int size);
void displayAsmInstructions(microprocessor_t* proc,int start,int end,int instructionLength);
void handleBreakPoint(const char* cmd,microprocessor_t* proc,int size,int start);
void handleRun(microprocessor_t* proc);