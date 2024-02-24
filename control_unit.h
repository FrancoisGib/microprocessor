#include "decoder.h"

typedef struct {
    int8_t bytes[3];
} control_unit_t;

typedef void (*assembly_function)(int8_t* params);

void control_dec(int8_t* params);
void control_inc(int8_t* params);
void control_add(int8_t* params);
void control_sub(int8_t* params);
void control_and(int8_t* params);
void control_swp(int8_t* params);
void control_mv_Rn_Rm(int8_t* params);
void control_jmp_hhll(int8_t* params);
void control_jz(int8_t* params);
void control_jc(int8_t* params);
void control_jmp_rx0(int8_t* params);
void control_st_r0_rxn(int8_t* params);
void control_ld_r0_rxn(int8_t* params);
void control_st_rn_hhll(int8_t* params);
void control_ld_rn_hhll(int8_t* params);
void control_mv_rn_arg(int8_t* params);
void control_not(int8_t* params);
void launch_assembly_instruction(int8_t* instruction, int8_t code);
void readNextByte();
void callControlUnit(int8_t debug, int last_instruction_address);
void callWithDebugger(microprocessor_t *microprocessor, int last_instruction_address);
int is_prefix(char* str1, char* str2);
void print_registers(microprocessor_t* microprocessor);