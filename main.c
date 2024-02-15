#include "control_unit.h"
#include <unistd.h>

int main(int argc, char* argv[]) {
    int parse_assembly = 0;
    int debugger = 0;
    int opt;
    char* filename = argv[1];
    while((opt = getopt(argc, argv, "sd")) != -1) { 
        switch(opt) { 
            case 's': 
                parse_assembly = 1;
                break; 
            case 'd':
                debugger = 1;
        } 
    } 

    readFile(filename, parse_assembly);
    callControlUnit(debugger);
    microprocessor_t* process = getMicroProcessor();
    printf("\n-------------------\n Memory:\n\n");
    for (int i = 0; i < 1024; i++)
        printf("%02X ", (uint8_t)process->ram[i]);
    printf("\n\n-------------------\n Registers:\n\n");
    for (int i = 0; i < 8; i++)
        printf(" - R%d = %.2X\n", i, process->R[i]);
    printf("\n - PC = %.4X\n", (uint16_t)process->PC);
    printf(" - Data Latch = %.2X\n", (uint8_t)process->DL);
    printf(" - Data Bus = %.2X\n", (uint8_t)process->dataBus);
    printf(" - Address Latch = %.4X\n", (uint16_t)process->AL);
    printf(" - Address Bus = %.4X\n", (uint16_t)process->addressBus);
    printf(" - CS = %.2X\n", process->CS);
    printf(" - IR = %.2X\n", (uint8_t)process->IR);
    printf(" - X = %.2X\n", process->X);
    printf(" - Y = %.2X\n", process->Y);
    printf("\n-------------------\n Flags:\n\n");
    printf(" - FC = %d\n", process->F[0]);
    printf(" - FZ = %d\n", process->F[1]);
    return 0;
}