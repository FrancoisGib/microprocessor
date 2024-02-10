#include"microprocessor.h"
#include "lib.h"
#include"decoder.h"

void launchInitPhase(microprocessor_t* proc);
void fetchInstruction(microprocessor_t* proc);
char* decodeInstruction(microprocessor_t* proc);
void displayRegisters(microprocessor_t* proc);
void startSimulation();
void launchInDebugMode(microprocessor_t* proc);
void launchInNormalMode(microprocessor_t* proc);
int askMode();

typedef struct {
    int idx;
    char* instruction;
    char* register_;
} debuggerRegistry ;