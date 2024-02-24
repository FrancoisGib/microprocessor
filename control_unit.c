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

void callControlUnit(int8_t debug, int last_instruction_address) {
    microprocessor_t* microprocessor = getMicroProcessor();
    if (!debug) {
        while ((int16_t)microprocessor->ram[microprocessor->PC] != -1) {
            read_next_instruction(microprocessor);
        }
    }
    else
        callWithDebugger(microprocessor, last_instruction_address);
}

typedef struct node_s node_t;

struct node_s {
    int ptr;
    node_t* next;
};


typedef struct {
    node_t* root;
} fifo_t;

void addElement(fifo_t* fifo, int ptr) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    new_node->ptr = ptr;
    if (fifo->root == NULL)
        fifo->root = new_node;
    else {
        node_t* node = fifo->root;
        while (node->next != NULL)
            node = node->next;
        node->next = new_node;
    }
}

void next(fifo_t* fifo) {
    node_t* root = fifo->root;
    fifo->root = fifo->root->next;
    free(root);
}

void callWithDebugger(microprocessor_t *microprocessor, int last_instruction_address) {
    char buf[32];
    printf("\n%4X", microprocessor->PC);
    fifo_t* fifo = (fifo_t*)malloc(sizeof(fifo_t));
    fifo->root = NULL;
    int end = 0;
    while (!end) {
        int i = 0;
        while ((buf[i] = getchar()) != '\n')
            i++;
        if (is_prefix(buf, "break")) {
            int n;
            sscanf(&buf[6], "%04X", &n);
            printf("\nNew break point at %04X\n", n);
            addElement(fifo, n);
        }
        else if (is_prefix(buf, "run")) {
            int16_t break_point;
            if (fifo->root != NULL) {
                break_point = fifo->root->ptr;
                next(fifo);
            }
            else
                break_point = last_instruction_address;
            while (microprocessor->PC != break_point) 
                read_next_instruction(microprocessor);
        }
        else if (is_prefix(buf, "info-registers"))
            print_registers(microprocessor);
        else if (is_prefix(buf, "end"))
            end = 1;
        else 
            printf("Invalid command, please retry\n");
        if (last_instruction_address == microprocessor->PC)
            end = 1;
        memset(buf, 0, i);
    }
    free(fifo);
}

void print_registers(microprocessor_t* microprocessor) {
    printf("\n-------------------\n Registers:\n\n");
    for (int i = 0; i < 8; i++)
        printf(" - R%d = %.2X\n", i, microprocessor->R[i]);
    printf("\n - PC = %.4X\n", (uint16_t)microprocessor->PC);
    printf(" - Data Latch = %.2X\n", (uint8_t)microprocessor->DL);
    printf(" - Data Bus = %.2X\n", (uint8_t)microprocessor->dataBus);
    printf(" - Address Latch = %.4X\n", (uint16_t)microprocessor->AL);
    printf(" - Address Bus = %.4X\n", (uint16_t)microprocessor->addressBus);
    printf(" - CS = %.2X\n", microprocessor->CS);
    printf(" - IR = %.2X\n", (uint8_t)microprocessor->IR);
    printf(" - X = %.2X\n", microprocessor->X);
    printf(" - Y = %.2X\n", microprocessor->Y);
    printf("\n-------------------\n Flags:\n\n");
    printf(" - FC = %d\n", microprocessor->F[0]);
    printf(" - FZ = %d\n\n", microprocessor->F[1]);
}

int is_prefix(char* str1, char* str2) {
    int8_t str1_len = strlen(str1);
    int8_t str2_len = strlen(str2);
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
