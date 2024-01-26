#include "control_unit.h"

/*int main() {
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
}*/

int main() {
    microprocessor_t* process = getMicroProcessor();
    char* pathname = "prog.txt";
    int i;
    for (i = 0; i < 8; i++)
        process->R[i] = 0;
    memset(process->ram, 0, 1024);
    process->ram[0x0100] = 2;
    process->ram[0x0101] = 8;
    readFile(pathname);
    callControlUnit();
    for (i = 0; i < 1024; i++)
        printf(" %d ", process->ram[i]);
    for (i = 0; i < 8; i++)
        printf("\nR%d = %d", i, process->R[i]);
    printf("\nResultat : %d\n", process->ram[0x0102]);
}