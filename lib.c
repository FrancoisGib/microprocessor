#include "lib.h"

assembly_instructions instructions[17] = {
    {"jmp", 8, 2},
    {"jz", 8, 2},
    {"jc", 8, 2},
    {"jmp RX0", 8, 0},
    {"st R0", 6, 1},
    {"ld R0", 6, 1},
    {"st", 5, 3},
    {"ld", 5, 3},
    {"mv", 5, 2},
    {"dec", 5, 1},
    {"inc", 5, 1},
    {"not", 5, 1},
    {"add", 3, 2},
    {"sub", 3, 2},
    {"swp", 3, 2},
    {"mv", 2, 2}
};

int8_t hex_to_dec(char *hex) {
   return (int)strtol(hex, NULL, 16);
}

void readFile(char* path) {
    FILE* input = fopen(path, "r");
    FILE* output = fopen("output.txt", "w");
    char instruction_address[] = "    ";
    fread(instruction_address, 4, 1, input);
    while(strcmp(instruction_address, "FFFF")) {
        fseek(input, 3, SEEK_CUR);
        char first_byte[] = "  ";
        fread(first_byte, 2, 1, input);
        int8_t decoded_byte = hex_to_dec(first_byte);
        instructions_details* details = decodeInstructionArguments(decoded_byte, input);
        assembly_instructions instruction = instructions[details->opcode];
        //fwrite(instruction.desc, 1, strlen(instruction.desc), output);
        printf("%s", instruction.desc);
        for (int i = 0; i < details->nb_args; i++) {
            int8_t num = details->args[i];
            printf(" %d", num);
        }
        free(details->args);
        free(details);
        printf("\n\n");
        fseek(input, 1, SEEK_CUR);
        fread(instruction_address, 4, 1, input);
    }
    fclose(input);
    fclose(output);
}

instructions_details* decodeInstructionArguments(int8_t first_byte, FILE* input) {
    int8_t opcode = decodeOpcode(first_byte);
    instructions_details* details = (instructions_details*)malloc(sizeof(instructions_details));
    details->opcode = opcode;
    assembly_instructions instruction = instructions[opcode];
    if (instruction.nb_args > 0) {
        details->nb_args = instruction.nb_args;
        details->args = (int8_t*)malloc(sizeof(int8_t) * details->nb_args);
    }

    if (instruction.instruction_name_size == 8)
        decodeWhenInstructionNameLengthEqualsEight(details, input);
    else if (instruction.instruction_name_size == 5) 
        decodeWhenInstructionNameLengthEqualsFive(first_byte, details, input);
    else if (instruction.instruction_name_size == 6) 
        decodeWhenInstructionNameLengthEqualsSix(first_byte, details);
    else if (instruction.instruction_name_size == 2) 
        decodeWhenInstructionNameLengthEqualsTwo(first_byte, details);
    else if (instruction.instruction_name_size == 3) 
        decodeWhenInstructionNameLengthEqualsThree(first_byte, details);
    return details;
}

void decodeWhenInstructionNameLengthEqualsEight(instructions_details* details, FILE* input) {
    for (int i = 0; i < details->nb_args; i++) {
        fgetc(input);
        char buf[] = "  ";
        fread(buf, 2, 1, input);
        details->args[i] = hex_to_dec(buf);
    }
}

void decodeWhenInstructionNameLengthEqualsFive(int8_t first_byte, instructions_details* details, FILE* input) {
    details->args[0] = first_byte & 0b00000111;
    if (details->nb_args > 1) {
        for (int i = 1; i < details->nb_args; i++) {
            fgetc(input);
            char buf[] = "  ";
            fread(buf, 2, 1, input);
            details->args[i] = hex_to_dec(buf);
        }
    }
}

void decodeWhenInstructionNameLengthEqualsThree(int8_t first_byte, instructions_details* details) {
    details->args[0] = first_byte >> 3 & 0b00000011;
    details->args[1] = first_byte & 0b00000111;
}

void decodeWhenInstructionNameLengthEqualsTwo(int8_t first_byte, instructions_details* details) {
    printf("byte %d\n", first_byte);
    details->args[0] = first_byte >> 3 & 0b00000111;
    details->args[1] = first_byte & 0b00000111;
}

void decodeWhenInstructionNameLengthEqualsSix(int8_t first_byte, instructions_details* details) {
    details->args[0] = first_byte & 0b00000011;
}