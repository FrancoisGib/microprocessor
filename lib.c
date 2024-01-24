#include "lib.h"


assembly_instructions instructions[17] = {
    {"jmp", 8, 2},
    {"jz", 8, 2},
    {"jc", 8, 2},
    {"jmp RX0", 8, 0},
    {"st", 6, 1},
    {"ld", 6, 1},
    {"st", 5, 3},
    {"ld", 5, 3},
    {"mv", 5, 2},
    {"dec", 5, 1},
    {"inc", 5, 1},
    {"not", 5, 1},
    {"add", 3, 2},
    {"sub", 3, 2},
    {"and", 3, 2},
    {"swp", 3, 2},
    {"mv", 2, 2}
};

int16_t hex_to_dec(char *hex) {
    return strtol(hex, NULL, 16);
}

void readFile(char* path) {
    microprocessor_t* process = getMicroProcessor();
    FILE* input = fopen(path, "r");
    FILE* output = fopen("output.s", "w");
    char instruction_address[5];
    size_t readed_size = fread(instruction_address, 4, 1, input);
    process->PC = hex_to_dec(instruction_address);
    int8_t size = -4;
    while(readed_size > 0) {
        fseek(input, 3, SEEK_CUR);
        char first_byte[3];
        fread(first_byte, 2, 1, input);
        int8_t decoded_byte = hex_to_dec(first_byte);
        instructions_details* details = decodeInstructionArguments(decoded_byte, input, output);
        assembly_instructions instruction = instructions[details->opcode];
        for (int i = 0; i < details->nb_bytes; i++) {
            process->ram[hex_to_dec(instruction_address) + i] = details->bytes[i];
        }
        free(details->args);
        free(details->bytes);
        free(details);
        fseek(input, 1, SEEK_CUR);
        readed_size = fread(instruction_address, 4, 1, input);
        fwrite("\n", 1, 1, output);
        size += details->nb_bytes;
    }
    fclose(input);
    fclose(output);
    process->ram[size + process->PC] = 0xFF;
    process->ram[size + process->PC + 1] = 0xFF;
}

instructions_details* decodeInstructionArguments(int8_t first_byte, FILE* input, FILE* output) {
    int8_t opcode = decodeOpcode(first_byte);
    fwrite(instructions[opcode].desc, 1, strlen(instructions[opcode].desc), output);
    fwrite(" ", 1, 1, output);
    instructions_details* details = (instructions_details*)malloc(sizeof(instructions_details));
    details->opcode = opcode;
    assembly_instructions instruction = instructions[opcode];
    if (instruction.nb_args > 0) {
        details->nb_args = instruction.nb_args;
        details->args = (int8_t*)malloc(sizeof(int8_t) * details->nb_args);
    }
    if (instruction.instruction_name_size == 8)
        decodeWhenInstructionNameLengthEqualsEight(first_byte, details, input, output);
    else if (instruction.instruction_name_size == 5) 
        decodeWhenInstructionNameLengthEqualsFive(first_byte, details, input, output);
    else if (instruction.instruction_name_size == 6) 
        decodeWhenInstructionNameLengthEqualsSix(first_byte, details, output);
    else if (instruction.instruction_name_size == 2) 
        decodeWhenInstructionNameLengthEqualsTwo(first_byte, details, output);
    else if (instruction.instruction_name_size == 3) 
        decodeWhenInstructionNameLengthEqualsThree(first_byte, details, output);
    return details;
}

void decodeWhenInstructionNameLengthEqualsEight(int8_t first_byte, instructions_details* details, FILE* input, FILE* output) {
    details->nb_bytes = 3;
    details->bytes = (int8_t*)malloc(8 * 3);
    fwrite("(", 1, 1, output);
    for (int i = 0; i < details->nb_args; i++) {
        fgetc(input);
        char buf[3];
        fread(buf, 2, 1, input);
        fwrite(buf, 2, 1, output);
        details->args[i] = hex_to_dec(buf);
        details->bytes[i] = details->args[i];
    }
    fwrite(")", 1, 1, output);
}

void decodeWhenInstructionNameLengthEqualsFive(int8_t first_byte, instructions_details* details, FILE* input, FILE* output) {
    details->args[0] = first_byte & 0b00000111;
    char arg[4];
    sprintf(arg, "R%d ", details->args[0]);
    if (details->nb_args > 1)
        arg[2] = ',';
    fwrite(arg, 3, 1, output);
    if (details->nb_args > 1) {
        if (details->nb_args == 2) {
            details->nb_bytes = 2;
            details->bytes = (int8_t*)malloc(16);
            fwrite(" $", 2, 1, output);
        }
        else if (details->nb_args == 3) {
            details->nb_bytes = 3;
            details->bytes = (int8_t*)malloc(24);
            fwrite(" (", 2, 1, output);
        }
        for (int i = 1; i < details->nb_args; i++) {
            char c = fgetc(input);
            char buf[3];
            fread(buf, 2, 1, input);
            if (details->nb_args == 3)
                fwrite(buf, 2, 1, output);
            else
                fwrite(buf + 1, 1, 1, output);
            details->args[i] = hex_to_dec(buf);
            details->bytes[i] = details->args[i];
        }
        if (details->nb_args == 3)
            fwrite(")", 1, 1, output);
    }
    else {
        details->nb_bytes = 1;
        details->bytes = (int8_t*)malloc(8);
    }
    details->bytes[0] = first_byte;
}

void writeRegisters(int8_t first_arg, int8_t second_arg, FILE* output) {
    char buf[7];
    sprintf(buf, "R%d, R%d", first_arg, second_arg);
    fwrite(buf, 6, 1, output);
}

void mallocAndWriteFirstByte(int8_t first_byte, instructions_details* details) {
    details->bytes = (int8_t*)malloc(8);
    details->bytes[0] = first_byte;
}

void decodeWhenInstructionNameLengthEqualsThree(int8_t first_byte, instructions_details* details, FILE* output) {
    details->args[0] = first_byte >> 3 & 0b00000011;
    details->args[1] = first_byte & 0b00000111;
    details->nb_bytes = 1;
    writeRegisters(details->args[0], details->args[1], output);
    mallocAndWriteFirstByte(first_byte, details);
}

void decodeWhenInstructionNameLengthEqualsTwo(int8_t first_byte, instructions_details* details, FILE* output) {
    details->nb_bytes = 1;
    details->args[0] = first_byte >> 3 & 0b00000111;
    details->args[1] = first_byte & 0b00000111;
    writeRegisters(details->args[0], details->args[1], output);
    mallocAndWriteFirstByte(first_byte, details);
}

void decodeWhenInstructionNameLengthEqualsSix(int8_t first_byte, instructions_details* details, FILE* output) {
    details->nb_bytes = 3;
    details->args[0] = first_byte & 0b00000011;
    char buf[8] = "R0, RX ";
    sprintf(buf + 6, "%d", details->args[0]);
    fwrite(buf, 7, 1, output);
    mallocAndWriteFirstByte(first_byte, details);
}