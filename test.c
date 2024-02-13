#include "control_unit.h"
#include <assert.h>

microprocessor_t* reset() {
    microprocessor_t* microprocessor = getMicroProcessor();
    memset(microprocessor->ram, 0, 1024);
    memset(microprocessor->R, 0, 8);
    return microprocessor;
}

void test_load_HHLL() {
    microprocessor_t* microprocessor = reset();
    readFile("./tests/load_hhll.txt", 0);
    microprocessor->ram[0x0100] = 1;
    assert(microprocessor->R[0] == 0);
    int16_t PC = microprocessor->PC;
    int8_t instruction_size = 3;
    callControlUnit();
    assert(microprocessor->R[0] == 1);
    assert(microprocessor->PC == PC + instruction_size);
}

void test_store_HHLL() {
    microprocessor_t* microprocessor = reset();
    microprocessor->R[0] = 1;
    readFile("./tests/store_hhll.txt", 0);
    assert(microprocessor->ram[0x0100] == 0);
    int16_t PC = microprocessor->PC;
    int8_t instruction_size = 3;
    callControlUnit();
    assert(microprocessor->ram[0x0100] == 1);
    assert(microprocessor->PC == PC + instruction_size);
}

void test_load_R0_RXn() {
    microprocessor_t* microprocessor = reset();
    microprocessor->R[2] = 0x01;
    microprocessor->R[3] = 0x00;
    microprocessor->ram[0x0100] = 1;
    readFile("./tests/load_R0_RXn.txt", 0);
    assert(microprocessor->R[0] == 0);
    int16_t PC = microprocessor->PC;
    int8_t instruction_size = 1;
    callControlUnit();
    assert(microprocessor->R[0] == 1);
    assert(microprocessor->PC == PC + instruction_size);
}

void test_store_R0_RXn() {
    microprocessor_t* microprocessor = reset();
    microprocessor->R[2] = 0x01;
    microprocessor->R[3] = 0x00;
    microprocessor->R[0] = 1;
    readFile("./tests/store_R0_RXn.txt", 0);
    assert(microprocessor->ram[0x0100] == 0);
    int16_t PC = microprocessor->PC;
    int8_t instruction_size = 1;
    callControlUnit();
    assert(microprocessor->ram[0x0100] == 1);
    assert(microprocessor->PC == PC + instruction_size);
}

void test_JMP() {
    microprocessor_t* microprocessor = reset();
    readFile("./tests/jmp.txt", 0);
    callControlUnit();
    assert(microprocessor->PC == 0x0010);
}

void test_JZ() {
    microprocessor_t* microprocessor = reset();
    microprocessor->F[1] = 1;
    readFile("./tests/jz.txt", 0);
    callControlUnit();
    assert(microprocessor->PC == 0x0010);
}

void test_JC() {
    microprocessor_t* microprocessor = reset();
    microprocessor->F[0] = 1;
    readFile("./tests/jc.txt", 0);
    callControlUnit();
    assert(microprocessor->PC == 0x0010);
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