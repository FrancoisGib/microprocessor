#include "decoder.h"

instruction_informations instructions_info[NB_INSTRUCTIONS] = {
    {0b01110000,0, 8, 3, "jmp %.2X%.2X", 8, 2, {{1, 255}, {2, 255}}}, 
    {0b01110001,0, 8, 3, "jz %.2X%.2X", 7, 2, {{1, 255}, {2, 255}}},
    {0b01110010,0, 8, 3, "jc %.2X%.2X", 7, 2, {{1, 255}, {2, 255}}},
    {0b01110011,0, 8, 1, "jmp %%RX0", 8, 0},
    {0b01111000,3, 6, 1, "st %%R0, %%RX%d", 12, 1, {{0, 3}}},
    {0b01111100,3, 6, 1, "ld %%R0, %%RX%d", 12, 1, {{0, 3}}},
    {0b01000000,7, 5, 3, "st %%R%d, %.2X%.2X", 12, 3, {{0, 7}, {1, 255}, {2, 255}}},
    {0b01001000,7, 5, 3, "ld %%R%d, %.2X%.2X", 12, 3, {{0, 7}, {1, 255}, {2, 255}}},
    {0b01010000,7, 5, 2, "mv %%R%d, $%d", 10, 2, {{0, 7}, {1, 255}}},
    {0b01011000,7, 5, 1, "dec %%R%d", 7, 1, {{0, 7}}}, 
    {0b01100000,7, 5, 1, "inc %%R%d", 7, 1, {{0, 7}}},
    {0b01101000,7, 5, 1, "not %%R%d", 7, 1, {{0, 7}}},
    {0b10000000,0b00011111, 3, 1, "add %%R%d, %%R%d", 12, 2, {{0, 3, 3}, {0, 7}}},
    {0b10100000,0b00011111, 3, 1, "sub %%R%d, %%R%d", 12, 2, {{0, 3, 3}, {0, 7}}},
    {0b11000000,0b00011111, 3, 1, "and %%R%d, %%R%d", 12, 2, {{0, 3, 3}, {0, 7}}},
    {0b11100000,0b00011111, 3, 1, "swp %%R%d, %%R%d", 12, 2, {{0, 3, 3}, {0, 7}}},
    {0,0b00111111, 2, 1, "mv %%R%d, %%R%d", 11, 2, {{0, 7, 3}, {0, 7}}}
};

instruction_informations* get_instruction_informations() {
    return instructions_info;
}

int8_t decodeOpcode(int8_t byte) {
    for (int i = 0; i < NB_INSTRUCTIONS; ++i) {
        if ((byte & ~instructions_info[i].mask) == instructions_info[i].code)
            return i;
    }
    return -1;
}

int16_t hex_to_dec(char *hex) {
    return strtol(hex, NULL, 16);
}

void test_all_instructions() {
    char* instructions[] = {"70", "71", "72", "73", "78", "7C", "40", "48", "50", "58", "60", "68", "80", "A0", "C0", "E0", "00"};
    for (int i = 0; i < 17; i++) {
        int8_t opcode = decodeOpcode(hex_to_dec(instructions[i]));
        assert(opcode == i);
    }
}

int16_t readFile(char* path, int8_t write, FILE* output) {
    microprocessor_t* process = getMicroProcessor();
    int8_t last_instruction_size;
    int16_t last_intruction_address;
    FILE* input = fopen(path, "r");
    if (input == NULL) {
        perror("Error opening input file ");
        exit(0);
    }
    size_t read_bytes = 1;
    char memory_buffer[5];
    char buf[3];
    read_bytes = fread(memory_buffer, 4, 1, input);
    instruction_s instruction; 
    instruction.memory_address = hex_to_dec(memory_buffer);
    process->PC = instruction.memory_address;
    while (strcmp(memory_buffer, "FFFF") != 0) {
        fseek(input, 3, SEEK_CUR); // pass ":  "
        read_bytes = fread(buf, 2, 1, input);
        buf[2] = '\0';
        int8_t first_byte = hex_to_dec(buf);
        int8_t opcode = decodeOpcode(first_byte);
        instruction.opcode = opcode;
        instruction_informations instruction_info = instructions_info[opcode];
        last_instruction_size = instruction_info.nb_bytes;
        last_intruction_address = instruction.memory_address;
        if (instruction_info.nb_args > 0)
            instruction.args = (int8_t*)malloc(8 * instruction_info.nb_args);
        instruction.bytes = (int8_t*)malloc(8 * instruction_info.nb_bytes);
        instruction.bytes[0] = first_byte;
        int8_t i;

        fseek(input, 1, SEEK_CUR); // pass " "
        process->ram[last_intruction_address] = instruction.bytes[0];
        for (i = 1; i < instruction_info.nb_bytes; i++) {
            read_bytes = fread(buf, 2, 1, input);
            instruction.bytes[i] = hex_to_dec(buf);
            fseek(input, 1, SEEK_CUR); // pass " "
            process->ram[last_intruction_address + i] = instruction.bytes[i];
        }
        for (i = 0; i < instruction_info.nb_args; i++) {
            int8_t byte_number = instruction_info.arguments[i].byte_number;
            instruction.args[i] = (instruction.bytes[byte_number] >> instruction_info.arguments[i].shift) & instruction_info.arguments[i].mask;
        }
        char instruction_buffer[instruction_info.string_size + 2];
        switch (instruction_info.nb_args) {
            case 0:
                strcpy(instruction_buffer, instruction_info.assembly_string);
                break;
            case 1:
                sprintf(instruction_buffer, instruction_info.assembly_string, instruction.args[0]);
                break;
            case 2:
                sprintf(instruction_buffer, instruction_info.assembly_string, instruction.args[0], instruction.args[1]);
            case 3:
                sprintf(instruction_buffer, instruction_info.assembly_string, instruction.args[0], instruction.args[1], instruction.args[2]);
                break;
            default:
                printf("Error in the file\n");
                exit(0);
        }
        if (instruction_info.nb_args > 0)
            free(instruction.args);
        free(instruction.bytes);
        instruction_buffer[instruction_info.string_size] = '\n';
        if (write)
            fwrite(instruction_buffer, instruction_info.string_size + 1, 1, output);
        read_bytes = fread(memory_buffer, 4, 1, input);
        instruction.memory_address = hex_to_dec(memory_buffer);
    }
    memset(&process->ram[last_intruction_address + last_instruction_size], 0xFF, 2);
    fseek(input, 0, SEEK_SET); // in case we read the file again
    fclose(input);
    return last_intruction_address + last_instruction_size;
}