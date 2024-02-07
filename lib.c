#include "lib.h"

instructions_lengths instructions_informations[] = {
    {"jmp", 8, 3, 2, 0, {0, 255, 255}},
    {"jz", 8, 3, 2, 0, {0, 255, 255}},
    {"jc", 8, 3, 2, 0, {0, 255, 255}},
    {"jmp RX0", 8, 1, 0, 0, {0, 0, 0}},
    {"st", 6, 1, 1, 0, {3, 255, 255}},
    {"ld", 6, 1, 1, 0, {3, 255, 255}},
    {"st", 5, 3, 3, 0, {7, 255, 255}},
    {"ld", 5, 3, 3, 0, {7, 255, 255}},
    {"mv", 5, 2, 2, 0, {7, 255, 0}},
    {"dec", 5, 1, 1, 0, {7, 0, 0}},
    {"inc", 5, 1, 1, 0, {7, 0, 0}},
    {"not", 5, 1, 1, 0, {7, 0, 0}},
    {"add", 3, 1, 2, 3, {3, 7, 0}},
    {"sub", 3, 1, 2, 3, {3, 7, 0}},
    {"and", 3, 1, 2, 3, {3, 7, 0}},
    {"swp", 3, 1, 2, 3, {3, 7, 0}},
    {"mv", 2, 1, 2, 3, {7, 7, 0}}
};

instruction_description output_descriptions[] = {
    {8, "(%02x%02x)", 6, 2},
    {8, "RX0", 4, 0},
    {6, "R0, RX%d", 7, 1},
    {2, "R%d, R%d", 6, 2},
    {3, "R%d, R%d", 6, 2},
    {5, "R%d, (%02x%02x)", 10, 3},
    {5, "R%d, $%d", 6, 2},
    {5, "R%d", 2, 1}
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
    int8_t size = 0;
    while(readed_size > 0) {
        fseek(input, 3, SEEK_CUR);
        char first_byte[3];
        fread(first_byte, 2, 1, input);
        int8_t decoded_byte = hex_to_dec(first_byte);
        instructions_details* details = decodeInstructionArguments(decoded_byte, input, output);
        instructions_lengths instruction = instructions_informations[details->opcode];
        for (int i = 0; i < details->nb_bytes; i++)
            process->ram[hex_to_dec(instruction_address) + i] = details->bytes[i];
        size += details->nb_bytes;
        free(details->args);
        free(details->bytes);
        free(details);
        fseek(input, 1, SEEK_CUR);
        readed_size = fread(instruction_address, 4, 1, input);
        fwrite("\n", 1, 1, output);
    }
    int16_t last_address = process->PC + size;
    fclose(input);
    fclose(output);
    process->ram[last_address] = 0xFF;
    process->ram[last_address + 1] = 0xFF;
}

instructions_details* decodeInstructionArguments(int8_t first_byte, FILE* input, FILE* output) {
    int8_t opcode = decodeOpcode(first_byte);
    fwrite(instructions_informations[opcode].desc, 1, strlen(instructions_informations[opcode].desc), output);
    fwrite(" ", 1, 1, output);
    instructions_details* details = (instructions_details*)malloc(sizeof(instructions_details));
    details->opcode = opcode;
    instructions_lengths instruction = instructions_informations[opcode];
    details->nb_args = instruction.nb_args;
    details->nb_bytes = instruction.nb_bytes;
    if (instruction.nb_args > 0) {
        details->args = (int8_t*)malloc(details->nb_args);
        details->args[0] = first_byte >> instruction.first_shift & instruction.args_masks[0];
    }
    details->bytes = (int8_t*)malloc(details->nb_bytes);
    details->bytes[0] = first_byte;
        if (instruction.nb_args > 1) {
            if (instruction.nb_bytes == 1)
                details->args[1] = first_byte & instruction.args_masks[1];
            else {
                int i = 1;
                while (i < details->nb_bytes) {
                    fgetc(input);
                    char buf[3];
                    fread(buf, 2, 1, input);
                    int8_t byte = hex_to_dec(buf);
                    
                    int j = i;
                    if (instruction.instruction_name_size == 8)
                        j--;
                    details->args[j] = byte & instruction.args_masks[i];
                    details->bytes[i] = byte;
                    i++;
                }
            }
                 
        }
    writeInstructionInOutputFile(details, output);
    return details;
}

void writeInstructionInOutputFile(instructions_details* details, FILE* output) {
    int8_t description_index = -1;
    int i = -1;
    while (description_index == -1) {
        i++;
        if (output_descriptions[i].nb_args == details->nb_args && output_descriptions[i].operation_size == instructions_informations[details->opcode].instruction_name_size) 
            description_index = i;
    }
    instruction_description instruction = output_descriptions[i];
    int size = instruction.desc_size;
    const char* desc = instruction.description;
    char buf[size];
    memset(buf, 0, size);
    if (details->nb_args == 1)
        sprintf(buf, desc, details->args[0]);
    else if (details->nb_args == 2)
        sprintf(buf, desc, details->args[0], details->args[1]);
    else
        sprintf(buf, desc, details->args[0], details->args[1], details->args[2]);
    fwrite(buf, size, 1, output);
}

typedef struct {
    char* description;
    char* args;
    int8_t nb_bytes;
    int8_t nb_args;
    int8_t args_size[3];
} assembly_instruction_informations;


assembly_instruction_informations assembly_descriptions[] = {
    {"jmp", " (%c%c)", 3, 1, {0, 1, 1}},
    {"jz ", "(%c%c)", 3, 1, {0, 1, 1}},
    {"jc ", "(%c%c)", 3, 1, {0, 1, 1}},
    {"jmp", " RX0", 1, 0, {0, 0, 0}},
    {"st ", "R0, RX%c", 1, 1, {}},
    {"ld ", "R0, RX%c", 1, 1, 1},
    {"st ", "R%c, (%s)", 2, 3, 4},
    {"ld ", "R%c, (%s)", 2, 3, 4},
    {"mv ", "R%c, %s\n", 2, 2, 3},
    {"dec", " R%c", 1, 1, 1},
    {"inc", " R%c", 1, 1, 1},
    {"not", " R%c", 1, 1, 1},
    {"add", " R%c, R%c", 2, 1, 2},
    {"sub", " R%c, R%c", 2, 1, 2},
    {"and", " R%c, R%c", 2, 1, 2},
    {"swp", " R%c, R%c", 2, 2},
    {"mv ", "R%c, R%c", 2, 2}
};

typedef struct 
{
    char* desc;
    int8_t nb_bytes;
    int8_t first_arg_mask;
} hexadecimal_instructions;


hexadecimal_instructions hexadecimal_informations[] = {
    {"70", 3, 0}, 
    {"71", 3, 0}, 
    {"72", 3, 0}, 
    {"73", 1, 0}, 
    {"78", 1, 3}, 
    {"7C", 1, 3},
    {"40", 3, 7},
    {"48", 3, 7},
    {"50", 2, 7},
    {"58", 1, 0x1F},
    {"60", 1, 0x1F},
    {"68", 1, 0x1F},
    {"80", 1, 0x1F},
    {"A0", 1, 0x1F},
    {"C0", 1, 0x1F},
    {"E0", 1, 0x1F},
    {"00", 1, 0x3F}
};

int8_t my_str_cmp(char* str1, char* str2) {
    int8_t length = strlen(str1) > strlen(str2) ? strlen(str2) : strlen(str1);
    for (int i = 0; i < length; i++) {
        if (str1[i] != str2[i])
            return 0;
    }
    return 1;
}


void hexadecimal_to_assembly(char* path) {
    FILE* input = fopen(path, "r");
    FILE* output = fopen("ouput.txt", "w");
    char buf[10];
    int readed_size = 1;
    int8_t i, j = 0;
    while(readed_size > 0) {
        int8_t found = 0;
        while (!found && i < 9) {
            fread(buf + i, 1, 1, input);
            i++;
            while (my_str_cmp(buf, assembly_descriptions[j].description) != 0) 
                j++;
        }
        char arg1[5];
        char arg2[5];
        fscanf(input, assembly_descriptions[j].args, arg1, arg2);
        arg1[sizeof(arg1) - 1] = '\0';
        arg2[sizeof(arg2) -1] = '\0';
        printf("\n %s %s %s", assembly_descriptions[j].description, arg1, arg2);
        strcat(buf + i, arg1);
        strcat(buf + i + strlen(arg1), arg2);
        fwrite(buf, i + strlen(arg1) + strlen(arg2), 1, output);
        fwrite("\n", 1, 1, output);
        i = 0;
    }
    fclose(input);
    fclose(output);
}

void main() {
    hexadecimal_to_assembly("output.s");
}