#include "control_unit.h"
#include <assert.h>

microprocessor_t* reset() {
    microprocessor_t* microprocessor = getMicroProcessor();
    memset(microprocessor, 0, sizeof(microprocessor_t));
    return microprocessor;
}

void test_load_HHLL() {
    microprocessor_t* microprocessor = reset();
    readFile("./tests/load_hhll.txt", 0, NULL);
    microprocessor->ram[0x0100] = 1;
    assert(microprocessor->R[0] == 0);
    int16_t PC = microprocessor->PC;
    int8_t instruction_size = 3;
    callControlUnit(0, 0);
    assert(microprocessor->R[0] == 1);
    assert(microprocessor->PC == PC + instruction_size);
}

void test_store_HHLL() {
    microprocessor_t* microprocessor = reset();
    microprocessor->R[0] = 1;
    readFile("./tests/store_hhll.txt", 0, NULL);
    assert(microprocessor->ram[0x0100] == 0);
    int16_t PC = microprocessor->PC;
    int8_t instruction_size = 3;
    callControlUnit(0, 0);
    assert(microprocessor->ram[0x0100] == 1);
    assert(microprocessor->PC == PC + instruction_size);
}

void test_load_R0_RXn() {
    microprocessor_t* microprocessor = reset();
    microprocessor->R[2] = 0x01;
    microprocessor->R[3] = 0x00;
    microprocessor->ram[0x0100] = 1;
    readFile("./tests/load_R0_RXn.txt", 0, NULL);
    assert(microprocessor->R[0] == 0);
    int16_t PC = microprocessor->PC;
    int8_t instruction_size = 1;
    callControlUnit(0, 0);
    assert(microprocessor->R[0] == 1);
    assert(microprocessor->PC == PC + instruction_size);
}

void test_store_R0_RXn() {
    microprocessor_t* microprocessor = reset();
    microprocessor->R[2] = 0x01;
    microprocessor->R[3] = 0x00;
    microprocessor->R[0] = 1;
    readFile("./tests/store_R0_RXn.txt", 0, NULL);
    assert(microprocessor->ram[0x0100] == 0);
    int16_t PC = microprocessor->PC;
    int8_t instruction_size = 1;
    callControlUnit(0, 0);
    assert(microprocessor->ram[0x0100] == 1);
    assert(microprocessor->PC == PC + instruction_size);
}

void test_JMP() {
    microprocessor_t* microprocessor = reset();
    readFile("./tests/jmp.txt", 0, NULL);
    callControlUnit(0, 0);
    printf("\nPC %04X\n", microprocessor->PC);
    assert(microprocessor->PC == 0x0010);
}

void test_JZ() {
    microprocessor_t* microprocessor = reset();
    microprocessor->FZ = 1;
    readFile("./tests/jz1.txt", 0, NULL);
    callControlUnit(0, 0);
    assert(microprocessor->PC == 0x0010);
    microprocessor = reset();
    readFile("./tests/jz0.txt", 0, NULL);
    callControlUnit(0, 0);
    assert(microprocessor->PC == 0x0203);
}

void test_JC() {
    microprocessor_t* microprocessor = reset();
    microprocessor->FC = 1;
    readFile("./tests/jc1.txt", 0, NULL);
    callControlUnit(0, 0);
    assert(microprocessor->PC == 0x0010);
    microprocessor = reset();
    readFile("./tests/jc0.txt", 0, NULL);
    callControlUnit(0, 0);
    assert(microprocessor->PC == 0x0203);
}


int main(int argc, char *argv[]) {
    test_load_HHLL();
    test_store_HHLL();
    test_load_R0_RXn();
    test_store_R0_RXn();
    test_JMP();
    test_JZ();
    test_JC();
    printf("All tests passed !\n");
}