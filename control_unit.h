#include "lib.h"

typedef struct {
    int8_t bytes[3];
} control_unit_t;

typedef struct {
    int8_t opcode;    
    int8_t nb_args;
    int8_t* args;
} control_unit_decoded_instruction;

typedef void (*assembly_function)(control_unit_decoded_instruction* params);

void readFile(char* path);
control_unit_decoded_instruction* decodeInstruction(int8_t first_byte);
void decodeEight(control_unit_decoded_instruction* details);
void decodeFive(control_unit_decoded_instruction* details);
void decodeThree(control_unit_decoded_instruction* details);
void decodeTwo(control_unit_decoded_instruction* details);
void decodeSix(control_unit_decoded_instruction* details);
control_unit_t* getControlUnit();
void control_dec(control_unit_decoded_instruction* params);
void control_inc(control_unit_decoded_instruction* params);
void control_add(control_unit_decoded_instruction* params);
void control_sub(control_unit_decoded_instruction* params);
void control_and(control_unit_decoded_instruction* params);
void control_swp(control_unit_decoded_instruction* params);
void control_mv_Rn_Rm(control_unit_decoded_instruction* params);
void control_jmp_hhll(control_unit_decoded_instruction* params);
void control_jz(control_unit_decoded_instruction* params);
void control_jc(control_unit_decoded_instruction* params);
void control_jmp_rx0(control_unit_decoded_instruction* params);
void control_st_r0_rxn(control_unit_decoded_instruction* params);
void control_ld_r0_rxn(control_unit_decoded_instruction* params);
void control_st_rn_hhll(control_unit_decoded_instruction* params);
void control_ld_rn_hhll(control_unit_decoded_instruction* params);
void control_mv_rn_arg(control_unit_decoded_instruction* params);
void control_not(control_unit_decoded_instruction* params);
void launch_assembly_instruction(control_unit_decoded_instruction* instruction);
void readNextByte();
void callControlUnit();