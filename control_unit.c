#include "control_unit.h"

control_unit_t control_unit;

assembly_instructions instructio[] = {
    {"jmp", 8, 0},
    {"jz", 8, 0},
    {"jc", 8, 0},
    {"jmp RX0", 8, 0},
    {"st", 6, 1},
    {"ld", 6, 1},
    {"st", 5, 1},
    {"ld", 5, 1},
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

control_unit_decoded_instruction* decodeInstruction(int8_t first_byte) {
    int8_t opcode = decodeOpcode(first_byte);
    //printf("\nfirst byte : 0x%03x", first_byte);
    control_unit_decoded_instruction* details = (control_unit_decoded_instruction*)malloc(sizeof(control_unit_decoded_instruction));
    details->opcode = opcode;
    assembly_instructions instruction = instructio[opcode];
    if (instruction.nb_args > 0) {
        details->nb_args = instruction.nb_args;
        details->args = (int8_t*)malloc(sizeof(int8_t) * details->nb_args);
    }
    details->args[0] = control_unit.bytes[0];
    if (instruction.instruction_name_size == 8)
        decodeEight(details);
    else if (instruction.instruction_name_size == 5) 
        decodeFive(details);
    else if (instruction.instruction_name_size == 6) 
        decodeSix(details);
    else if (instruction.instruction_name_size == 2) 
        decodeTwo(details);
    else if (instruction.instruction_name_size == 3) 
        decodeThree(details);
    return details;
}

void decodeEight(control_unit_decoded_instruction* details) {
    for (int i = 0; i < details->nb_args; i++)
        details->args[i] = control_unit.bytes[i + 1];
}

void decodeFive(control_unit_decoded_instruction* details) {
    details->args[0] = control_unit.bytes[0] & 0b00000111;
    if (details->nb_args > 1) {
        for (int i = 1; i < details->nb_args; i++) {
            details->args[i] = control_unit.bytes[i];
        }
    }
}

void decodeThree(control_unit_decoded_instruction* details) {
    details->args[0] = control_unit.bytes[0] >> 3 & 0b00000011;
    details->args[1] = control_unit.bytes[0] & 0b00000111;
}

void decodeTwo(control_unit_decoded_instruction* details) {
    details->args[0] = control_unit.bytes[0] >> 3 & 0b00000111;
    details->args[1] = control_unit.bytes[0] & 0b00000111;
}

void decodeSix(control_unit_decoded_instruction* details) {
    details->args[0] = control_unit.bytes[0] & 0b00000011;
}

control_unit_t* getControlUnit() {
    return &control_unit;
}

Instruction iSET[] = {
    {0b01110000,0b11111111,jmp_hhll, 1},
    {0b01110001,0b11111111,jz_hhll, 1},
    {0b01110010,0b11111111,jc_hhll, 1},
    {0b01110011,0b11111111,jmp_rx0, 1},
    {0b01111011,0b11111100,st_r0_rxn, 1},
    {0b01111111,0b11111100,ld_r0_rxn, 1},
    {0b01000111,0b11111000,st_rn_hhll, 1},
    {0b01001111,0b11111000,ld_rn_hhll, 1},
    {0b01010111,0b11111000,mv_rn_arg, 1},
    {0b01011111,0b11111000,dec_Rn, 1}, 
    {0b01100111,0b11111000,inc_Rn, 1},
    {0b01101111,0b11111000,not_Rn, 1},
    {0b10011111,0b11100000,add_Rn_Rm, 1},
    {0b10111111,0b11100000,sub_Rn_Rm, 1},
    {0b11011111,0b11100000,and_Rn_Rm, 1},
    {0b11111111,0b11100000,swp_Rn_Rm, 1},
    {0b00111111,0b11000000,mv_Rn_Rm, 1}
};

assembly_function functions[] = {control_jmp_hhll, control_jz, control_jc, control_jmp_rx0, control_st_r0_rxn, control_ld_r0_rxn, control_st_rn_hhll, control_ld_rn_hhll, control_mv_rn_arg, control_dec, control_inc, control_not, control_add, control_sub, control_and, control_swp, control_mv_Rn_Rm};

void callControlUnit() {
    microprocessor_t* microprocessor = getMicroProcessor();
    printf("0x%03x\n", microprocessor->PC);
    int i = 0;
    while (microprocessor->IR != -1) {
        printf("\nPC : 0x%03x", microprocessor->PC);
        readNextByte();
        control_unit.bytes[i] = microprocessor->IR;
        i++;
        int8_t first_byte = control_unit.bytes[0];
        int8_t code = decodeOpcode(first_byte);
        int8_t instruction_size = iSET[code].size;
        if (instruction_size == i) {
            printf("\nbyte : 0x%03x, 0x%03x, 0x%03x, instruc : %s", control_unit.bytes[0], control_unit.bytes[1], control_unit.bytes[2], instructio[code].desc);
            control_unit_decoded_instruction* instruction = decodeInstruction(control_unit.bytes[0]);
            launch_assembly_instruction(instruction);
            for (i = 0; i < 3; i++)
                control_unit.bytes[i] = 0;
            i = 0;
        }
    }
}

void control_dec(control_unit_decoded_instruction* params) {
    DEC(params->args[0]);
}

void control_inc(control_unit_decoded_instruction* params) {
    INC(params->args[0]);
}

void control_add(control_unit_decoded_instruction* params) {
    ADD(params->args[0], params->args[1]);
}

void control_sub(control_unit_decoded_instruction* params) {
    SUB(params->args[0], params->args[1]);
}

void control_and(control_unit_decoded_instruction* params) {
    AND(params->args[0], params->args[1]);
}

void control_swp(control_unit_decoded_instruction* params) {
    SWP(params->args[0], params->args[1]);
}

void control_mv_Rn_Rm(control_unit_decoded_instruction* params) {
    MV_Rn_Rm(params->args[0], params->args[1]);
}

void control_jmp_hhll(control_unit_decoded_instruction* params) {
    JMP();
}

void control_jz(control_unit_decoded_instruction* params) {
    microprocessor_t* micro = getMicroProcessor();
    if (micro->F[1] == 1)
        JMP();
    else {
        NOP();
    }
}

void control_jc(control_unit_decoded_instruction* params) {
    microprocessor_t* micro = getMicroProcessor();
    if (micro->F[0] == 1) {
        JMP();
    }
    else {
        NOP();
    }
}

void control_jmp_rx0(control_unit_decoded_instruction* params) {
    JMP_RX0();
}

void control_st_r0_rxn(control_unit_decoded_instruction* params) {
    ST_R0_RXn(params->args[0] * 2 + 1);
}

void control_ld_r0_rxn(control_unit_decoded_instruction* params) {
    LD_R0_RXn(params->args[0] * 2 + 1);
}

void control_st_rn_hhll(control_unit_decoded_instruction* params) {
    ST(params->args[0]);
}

void control_ld_rn_hhll(control_unit_decoded_instruction* params) {
    LD(params->args[0]);
}

void control_mv_rn_arg(control_unit_decoded_instruction* params) {
    MV_ARG(params->args[0]);
}

void control_not(control_unit_decoded_instruction* params) {
    NOT(params->args[0]);
}

void launch_assembly_instruction(control_unit_decoded_instruction* instruction) {
    assembly_function func = functions[instruction->opcode];
    (*func)(instruction);
}