#include "microprocessor.h"

microprocessor_t microprocessor;

// R registers signals

void SR(int i) {
    microprocessor.CS = i;
}

void Rin() {
    microprocessor.R[microprocessor.CS] = microprocessor.dataBus;
}

void Rout() {
    microprocessor.dataBus = microprocessor.R[microprocessor.CS];
}


// PC signals

void PCin() {
    microprocessor.PC = microprocessor.addressBus;
}

void PCout() {
    microprocessor.addressBus = microprocessor.PC;
}

void PCHin() {
    *((int8_t*)&microprocessor.PC) = microprocessor.dataBus;
}

void PCLin() {
    *((int8_t*)&microprocessor.PC + 1) = microprocessor.dataBus;
}


// DLatch signals

void DLout() {
    microprocessor.dataBus = microprocessor.DL;
}

void DLin() {
    microprocessor.DL = microprocessor.dataBus;
}


// AddressLatch signals

void ALin() {
    microprocessor.AL = microprocessor.addressBus;
}

void ALHin() {
    *((int8_t*)&microprocessor.AL) = microprocessor.dataBus;
}

void ALLin() {
    *((int8_t*)&microprocessor.AL + 1) = microprocessor.dataBus;
}

void AAout() {
    microprocessor.addressBus = microprocessor.AL + 1;
}


// ALU Signals

void Xin() {
    microprocessor.X = microprocessor.dataBus;
}

void Yin() {
    microprocessor.Y = microprocessor.dataBus;
}

void ALUout() {
    microprocessor.dataBus = microprocessor.ALUcom;
}

// Micro-instructions for ALU

void addALU() {
    microprocessor.ALUcom = (microprocessor.X + microprocessor.Y);
}

void subALU() {
    microprocessor.ALUcom = (microprocessor.X - microprocessor.Y);
}

void multALU() {
    microprocessor.ALUcom = (microprocessor.X * microprocessor.Y);
}

void divALU() {
    microprocessor.ALUcom = (microprocessor.X / microprocessor.Y);
}

void incALU() {
    microprocessor.ALUcom = (microprocessor.X + 1);
}

void decALU() {
    microprocessor.ALUcom = (microprocessor.X - 1);
}

void RepX() {
    microprocessor.ALUcom = microprocessor.X;
}

void RepY() {
    microprocessor.ALUcom = microprocessor.Y;
}


// IR signals

void IRin() {
    microprocessor.IR = microprocessor.dataBus;
}


// Memory requests

void read() {
    microprocessor.DL = microprocessor.ram[microprocessor.AL];
}

void write() {
    microprocessor.ram[microprocessor.AL] = microprocessor.DL;
}



// Assembly instructions

void ADD(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(), Xin();
    SR(Rm), Rout(), Yin();
    addALU(); ALUout(); SR(Rn); Rin();
}

void SUB(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(), Xin();
    SR(Rm), Rout(), Yin();
    subALU(); ALUout(); SR(Rn); Rin();
}

void MULT(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(), Xin();
    SR(Rm), Rout(), Yin();
    multALU(); ALUout(); SR(Rn); Rin();
}

void DIV(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(), Xin();
    SR(Rm), Rout(), Yin();
    divALU(); ALUout(); SR(Rn); Rin();
}

void MV(int8_t Rn, int8_t Rm) {
    SR(Rm); Rout();
    SR(Rn); Rin();
}

void SWP(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); Xin();
    SR(Rm); Rout(); Yin();
    RepX(); ALUout(); SR(Rm); Rin();
    RepY(); ALUout(); SR(Rn); Rin();
}

/*int main() {
    microprocessor.PC = 16385;
    microprocessor.dataBus = 2;
    PCLin();
    PCHin();
    int8_t v = *((int8_t*)&microprocessor.PC);
    printf("ptr: %p value: %d\n", (&microprocessor.PC), v);
    printf("ptr: %p value: %d", &microprocessor.PC + 1, *((int8_t*)&microprocessor.PC + 1));
}*/



int main() {
    init();
    microprocessor.X = 5;
    microprocessor.Y = 2;
    multALU();
    ALUout();
    printf("DataBus : %d\n", microprocessor.dataBus);

    microprocessor.ram[1] = 12;
    microprocessor.AL = 1;
    read();
    printf("DataLatch : %d\n", microprocessor.DL);

    microprocessor.R[3] = 20;
    microprocessor.R[5] = 10;
    ADD(3, 5);
    printf("R3 after ADD R3, R5 : %d", microprocessor.R[3]);
}