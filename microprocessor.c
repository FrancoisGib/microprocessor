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

void readSignal() {
    microprocessor.DL = microprocessor.ram[microprocessor.AL];
}

void writeSignal() {
    microprocessor.ram[microprocessor.AL] = microprocessor.DL;
}

microprocessor_t* getMicroProcessor() {
    return &microprocessor;
}