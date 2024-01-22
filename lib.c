#include "lib.h"

assembly_instructions instructions[17] = {
    {"jmp", 3},
    {"jz", 3},
    {"jc", 3},
    {"jmp", 1},
    {"st", 1},
    {"ld", 1},
    {"st", 3},
    {"ld", 2},
    {"mv", 1},
    {"dec", 1},
    {"inc", 1},
    {"not", 1},
    {"add", 2},
    {"sub", 1},
    {"swp", 1},
    {"mv", 1}
}

int8_t hex_to_dec(char* str) {
    return strtol(str, NULL, 16);
}

void readFile(char* path) {
    FILE* file = fopen(path, 'r');
    FILE* output = fopen("output.txt", 'r');
    char instruction[4];
    while (instruction != "FFFF") {
        fread(instruction, 4, 1, file);
        int8_t instruction_first_byte = hex_to_dec(instruction);
        int8_t opcode = decodeOpcode(instruction_first_byte);
        int8_t size = instructions[opcode].size;
        fread(NULL, 1, 3, file);
        int8_t arguments[size];
        char buf[2];
        for (int i = 0; i < size; i++) {
            fread(buf, 2, 1, file);
            arguments[i] = hex_to_dec(buf);
        }
        outputInstruction(opcode, arguments, file);
    }
}

void outputInstruction(int8_t opcode, int8_t* arguments, FILE* output) {
    int8_t size = instructions[opcode].size;
    char* desc = instructions[opcode].desc;
    fwrite(desc, strlen(desc), 1, output);
    arguments[0] = getVariablePart(opcode);
    for (int i = 1; i < size; i++) {
        arguments[i] = 
    }
}

void main() {
    microprocessor_t* process = getMicroProcessor();
    char* pathname = "file";
    readFile(pathname);
}