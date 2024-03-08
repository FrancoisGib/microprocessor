#include "control_unit.h"

control_unit_t control_unit;
#define microprocessor getMicroProcessor()

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

void read_next_instruction() {
    readNextByte();
    instruction_informations* instructions = get_instruction_informations();
    int8_t code = decodeOpcode(microprocessor->IR);
    int8_t instruction_size = instructions[code].nb_bytes;
    int8_t* instruction = decodeInstruction(microprocessor->IR, instructions[code]);
    launch_assembly_instruction(instruction, code);
}

void callControlUnit(int8_t debug, int last_instruction_address) {
    if (!debug) {
        while ((int16_t)microprocessor->ram[microprocessor->PC] != -1) {
            read_next_instruction();
            printf("\n");
        }
    }
    else
        callWithDebugger(last_instruction_address);
}

void addElement(fifo_t* fifo, void* value, int8_t size) {
    node_t* new_node = create_node(value, size);
    if (fifo->root == NULL)
        fifo->root = new_node;
    else {
        node_t* node = fifo->root;
        while (node->next != NULL)
            node = node->next;
        node->next = new_node;
    }
}

void* next(fifo_t* fifo) {
    node_t* root = fifo->root;
    fifo->root = fifo->root->next;
    void* ptr = root->ptr;
    free(root);
    return ptr;
}

int is_empty(fifo_t* fifo) {
    return fifo->root == NULL;
}

fifo_t* init_fifo() {
    fifo_t* fifo = (fifo_t*)malloc(sizeof(fifo_t));
    fifo->root = NULL;
    return fifo;
}

node_t* create_node(void* value, int8_t size) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    new_node->ptr = malloc(size);
    new_node->next = NULL;
    memcpy(new_node->ptr, value, size);
    return new_node;
}

void callWithDebugger(int16_t last_instruction_address) {
    char buf[32];
    fifo_t* fifo = init_fifo();
    int end = 0;
    int8_t step = 0;
    while (!end) {
        int i = 0;
        while ((buf[i] = getchar()) != '\n')
            i++;
        if (strstr(buf, "break")) {
            step = 0;
            int16_t n;
            sscanf(&buf[6], "%04X", (int*)&n);
            printf("\nNew break point at %04X\n", n);
            addElement(fifo, &n, sizeof(int16_t));
        }
        else if (strstr(buf, "run")) {
            int16_t break_point;
            if (is_empty(fifo))
                break_point = last_instruction_address;
            else {
                void* break_ptr = next(fifo);
                break_point = *(int16_t*)break_ptr;
                free(break_ptr);
            }
            while (break_point > microprocessor->PC) {
                read_next_instruction();
                printf("\n");
            } 
        }
        else if (strstr(buf, "step")) {
            step = 1;
            read_next_instruction();
        }
        else if (strstr(buf, "info-registers")) {
            step = 0;
            print_registers();
        }
        else if (strstr(buf, "info-ram")) {
            step = 0;
            print_ram();
        }
        else if (strstr(buf, "end"))
            end = 1;
        else if (step == 1 && strstr(buf, "\n"))
            read_next_instruction();
        else {
            step = 0;
            printf("Invalid command, please retry\n");
        }
        if (last_instruction_address <= microprocessor->PC)
            end = 1;
        memset(buf, 0, i);
    }
    free(fifo);
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
    JMP(0);
}

void control_jz(int8_t* params) {
    microprocessor_t* micro = getMicroProcessor();
    if (micro->FZ == 1)
        JMP(2);
    else {
        readNextByte();
        readNextByte();
    }
}

void control_jc(int8_t* params) {
    microprocessor_t* micro = getMicroProcessor();
    if (micro->FC == 1)
        JMP(1);
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
