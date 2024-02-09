#include"microprocessor.h"
void launchInitPhase(microprocessor_t* proc);
void fetchInstruction(microprocessor_t* proc);
char* decodeInstruction(microprocessor_t* proc);
void displayRegisters(microprocessor_t* proc);
void startSimulation();