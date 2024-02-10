#include"microprocessor.h"
#include "lib.h"
#include"decoder.h"

typedef struct {
    int idx;
    char* instruction;
    char* register_;
} debuggerRegistry;

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