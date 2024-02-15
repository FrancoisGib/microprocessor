#include "control_unit.h"

control_unit_t control_unit;

assembly_function functions[] = {control_jmp_hhll, control_jz, control_jc, control_jmp_rx0, control_st_r0_rxn, control_ld_r0_rxn, control_st_rn_hhll, control_ld_rn_hhll, control_mv_rn_arg, control_dec, control_inc, control_not, control_add, control_sub, control_and, control_swp, control_mv_Rn_Rm};

int8_t* decodeInstruction(int8_t first_byte, instruction_informations instruction_info) {
    int8_t* instruction;
    if (instruction_info.code_size != 8) {
        int8_t nb_args;
        if (instruction_info.code_size < 4)
            nb_args = 2;
        else
            nb_args = 1;
        instruction = (int8_t*)malloc(sizeof(int8_t) * nb_args);
        for (int i = 0; i < nb_args; i++) {
            argument arg = instruction_info.arguments[i];
            instruction[i] = (first_byte >> arg.shift & arg.mask);
        }
    }
    return instruction;
}

void read_next_instruction(microprocessor_t* microprocessor) {
    readNextByte();
    instruction_informations* instructions = get_instruction_informations();
    int8_t code = decodeOpcode(microprocessor->IR);
    int8_t instruction_size = instructions[code].nb_bytes;
    int8_t* instruction = decodeInstruction(microprocessor->IR, instructions[code]);
    launch_assembly_instruction(instruction, code);
    memset(instruction, 0, 2);
}

void callControlUnit(int8_t debug) {
    microprocessor_t* microprocessor = getMicroProcessor();
    if (!debug) {
        while ((int16_t)microprocessor->ram[microprocessor->PC] != -1) {
            read_next_instruction(microprocessor);
        }
    }
    else
        callWithDebugger(microprocessor);
}

void callWithDebugger(microprocessor_t *microprocessor) {
    char buf[32];
    scanf("%s", buf);
    if (is_prefix(buf, "break")) {
        char n;
        sscanf(buf, "break %c", &n);
        char ok[2];
        ok[1] = '\0';
        ok[0] = n;
        printf("%d", (int)strtol(ok, NULL, 10));
    }
}

int is_prefix(char* str1, char* str2) {
    int8_t str1_len = strlen(str1);
    int8_t str2_len = strlen(str2);
    if (str1_len != str2_len) {
        return 0;
    }
    int8_t max_len = 1 * (str1_len > str2_len) + 1 * (str1_len <= str2_len);
    for (int8_t i = 0; i < max_len; i++) {
        if (str1[i] != str2[i]) {
            return 0;
        }
    }
    return 1;
}



void control_dec(int8_t* params) {
    DEC(params[0]);
}

void control_inc(int8_t* params) {
    INC(params[0]);
}

void control_add(int8_t* params) {
    ADD(params[0], params[1]);
}

void control_sub(int8_t* params) {
    SUB(params[0], params[1]);
}

void control_and(int8_t* params) {
    AND(params[0], params[1]);
}

void control_swp(int8_t* params) {
    SWP(params[0], params[1]);
}

void control_mv_Rn_Rm(int8_t* params) {
    MV_Rn_Rm(params[0], params[1]);
}

void control_jmp_hhll(int8_t* params) {
    JMP();
}

void control_jz(int8_t* params) {
    microprocessor_t* micro = getMicroProcessor();
    if (micro->F[1] == 1)
        JMP();
    else {
        readNextByte();
        readNextByte();
    }
}

void control_jc(int8_t* params) {
    microprocessor_t* micro = getMicroProcessor();
    if (micro->F[0] == 1) {
        JMP();
    }
    else {
        readNextByte();
        readNextByte();
    }
}

void control_jmp_rx0(int8_t* params) {
    JMP_RX0();
}

void control_st_r0_rxn(int8_t* params) {
    ST_R0_RXn(params[0] * 2);
}

void control_ld_r0_rxn(int8_t* params) {
    for (int i = 0; i < 2; i ++)
        LD_R0_RXn(params[0] * 2);
}

void control_st_rn_hhll(int8_t* params) {
    ST(params[0]);
}

void control_ld_rn_hhll(int8_t* params) {
    LD(params[0]);
}

void control_mv_rn_arg(int8_t* params) {
    MV_ARG(params[0]);
}

void control_not(int8_t* params) {
    NOT(params[0]);
}

void launch_assembly_instruction(int8_t* instruction, int8_t code) {
    assembly_function func = functions[code];
    (*func)(instruction);
}
