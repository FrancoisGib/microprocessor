#include "control_unit.h"
#include <unistd.h>

#define microprocessor getMicroProcessor()
#define cycle_count getCycleCount()

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
            addElement(program_names, argv[i], strlen(argv[i]));
        }
    }
    FILE* output;
    if (parse_assembly)
        output = fopen("program.ms", "w");
    if (is_empty(program_names))
        exit(0);
    while (!is_empty(program_names)) {
        void* ptr = next(program_names);
        char* filename = (char*)ptr;
        int16_t last_instruction_address = readFile(filename, parse_assembly, output);
        callControlUnit(debugger, last_instruction_address);
        free(ptr);
        if (parse_assembly)
            fwrite("\n", 1, 1, output);
    }
    free(program_names);
    if (parse_assembly)
        fclose(output);
    print_ram();
    print_registers();
    printf("\nThe program has been executed in %d cycles\n", *cycle_count);
    return 0;
}