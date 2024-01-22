#include <stdio.h>
#include "lib.h"

int main() {
    microprocessor_t *process = getMicroProcessor();

    process->X = 5;
    process->Y = 2;
    addALU();
    ALUout();
    printf("DataBus : %d\n", process->dataBus);

    process->ram[1] = 12;
    process->AL = 1;
    read();
    printf("DataLatch : %d\n", process->DL);

    process->R[3] = 20;
    process->R[5] = 10;
    ADD(3, 5);
    printf("R3 after ADD R3, R5 : %d", process->R[3]);

    ALUout();
}