#include "control_unit.h"
#include <assert.h>

microprocessor_t* reset() {
    microprocessor_t* microprocessor = getMicroProcessor();
    memset(microprocessor->ram, 0, 1024);
    microprocessor->addressBus = 0;
    microprocessor->AL = 0;
    microprocessor->ALUcom = 0;
    microprocessor->CS = 0;
    microprocessor->dataBus = 0;
    microprocessor->DL = 0;
    memset(microprocessor->F, 0, 2);
    microprocessor->IR = 0;
    microprocessor->PC = 0;
    memset(microprocessor->R, 0, 8);
    microprocessor->X = 0;
    microprocessor->Y = 0;
    return microprocessor;
}

void test_load_HHLL() {
    microprocessor_t* microprocessor = reset();
    microprocessor->ram[0x0100] = 1;
    assert(microprocessor->R[0] == 0);
    readFile("./tests/load_hhll.txt");
    callControlUnit();
    assert(microprocessor->R[0] == 1);
}

void test_store_HHLL() {
    microprocessor_t* microprocessor = reset();
    microprocessor->R[0] = 1;
    assert(microprocessor->ram[0x0100] == 0);
    readFile("./tests/store_hhll.txt");
    callControlUnit();
    assert(microprocessor->ram[0x0100] == 1);
}

void test_store_R0_RXn() {
    microprocessor_t* microprocessor = reset();
    microprocessor->R[2] = 0x01;
    microprocessor->R[3] = 0x00;
    microprocessor->R[0] = 1;
    assert(microprocessor->ram[0x0100] == 0);
    readFile("./tests/store_R0_RXn.txt");
    callControlUnit();
    assert(microprocessor->ram[0x0100] == 1);
}

void test_load_R0_RXn() {
    microprocessor_t* microprocessor = reset();
    microprocessor->R[2] = 0x01;
    microprocessor->R[3] = 0x00;
    microprocessor->ram[0x0100] = 1;
    assert(microprocessor->R[0] == 0);
    readFile("./tests/load_R0_RXn.txt");
    callControlUnit();
    assert(microprocessor->R[0] == 1);
}

void test_JZ() {
    microprocessor_t* microprocessor = reset();
    microprocessor->PC = 0x0200;
    microprocessor->F[1] = 1;
    readFile("./tests/jz.txt");
    callControlUnit();
    assert(microprocessor->PC == 0x0210);
}


int main(int argc, char *argv[]) {
    test_load_HHLL();
    test_store_HHLL();
    test_store_R0_RXn();
    test_load_R0_RXn();
    test_JZ();
}