#include "control_unit.h"
#include <unistd.h>

int main(int argc, char* argv[]) {
    int parse_assembly = 0;
    int debugger = 0;
    fifo_t* program_names = init_fifo();
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            parse_assembly = 1;
        }
        else if (strcmp(argv[i], "-d") == 0) {
            debugger = 1;
        }
        else {
            addElement(program_names, argv[i], sizeof(argv[i]));
        }
    }
    FILE* output;
    if (parse_assembly)
        output = fopen("output.txt", "w");
    if (program_names->size == 0)
        exit(0);
    while (!is_empty(program_names)) {
        void* ptr = next(program_names);
        char* filename = (char*)ptr;
        printf("file %s\n", filename);
        int last_instruction_address = readFile(filename, parse_assembly, output);
        callControlUnit(debugger, last_instruction_address);
        free(ptr);
        fwrite("\n", 1, 1, output);
    }
    free(program_names);
    if (parse_assembly)
        fclose(output);
        
    int i;
    microprocessor_t* process = getMicroProcessor();
    printf("\n-------------------\n Memory:\n\n");
    for (i = 0; i < 1024; i++)
        printf("%02X ", (uint8_t)process->ram[i]);
    printf("\n\n-------------------\n Registers:\n\n");
    for (i = 0; i < 8; i++)
        printf(" - R%d = %d\n", i, process->R[i]);
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
    printf(" - FC = %d\n", process->FC);
    printf(" - FZ = %d\n", process->FZ);
    return 0;
}