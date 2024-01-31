#include "control_unit.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Please specity the path to a program file\n");
        return -1;
    }
    char* pathname = argv[1];
    microprocessor_t* process = getMicroProcessor();
    int i;
    for (i = 0; i < 8; i++)
        process->R[i] = 0;
    memset(process->ram, 0, 1024);
    readFile(pathname);
    callControlUnit();
    for (i = 0; i < 1024; i++)
        printf(" %d ", process->ram[i]);
    printf("\n");
    for (i = 0; i < 8; i++)
        printf("\nR%d = %d", i, process->R[i]);
    printf("\n");
    return 0;
}