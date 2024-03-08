#include "decoder.h"

typedef struct {
    int8_t bytes[3];
} control_unit_t;

typedef void (*assembly_function)(int8_t* params);

typedef struct node_s node_t;

struct node_s {
    void* ptr;
    node_t* next;
};

typedef struct {
    node_t* root;
} fifo_t;

void addElement(fifo_t* fifo, void* value, int8_t size);
void* next(fifo_t* fifo);
int is_empty(fifo_t* fifo);
fifo_t* init_fifo();
node_t* create_node(void* value, int8_t size);

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
void callWithDebugger(int16_t last_instruction_address);