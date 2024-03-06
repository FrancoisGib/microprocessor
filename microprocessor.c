#include "microprocessor.h"

microprocessor_t microprocessor;
int cycle_count = 0;
#define READ_WRITE_CYCLE 3

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

void PCLin() {
    *((int8_t*)&microprocessor.PC) = microprocessor.dataBus;
}

void PCHin() {
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
    microprocessor.ALUcom = microprocessor.X + microprocessor.Y;
    if (microprocessor.ALUcom > 127)
        microprocessor.FC = 1;
    else
        microprocessor.FC = 0;
}

void subALU() {
    microprocessor.ALUcom = microprocessor.X - microprocessor.Y;
    if (microprocessor.ALUcom < -127)
        microprocessor.FC = 1;
    else
        microprocessor.FC = 0;
}

void incALU() {
    microprocessor.ALUcom = microprocessor.X + 1;
    if (microprocessor.ALUcom == 0) {
        microprocessor.FZ = 1;
    }
    else
        microprocessor.FZ = 0;
}

void decALU() {
    microprocessor.ALUcom = microprocessor.X - 1;
    if (microprocessor.ALUcom == 0) {
        microprocessor.FZ = 1;
    }
    else
        microprocessor.FZ = 0;
}

void notALU() {
    microprocessor.ALUcom = ~microprocessor.X;
}

void RepX() {
    microprocessor.ALUcom = microprocessor.X;
}

void RepY() {
    microprocessor.ALUcom = microprocessor.Y;
}

void andALU() {
    microprocessor.ALUcom = microprocessor.X & microprocessor.Y;
    if (microprocessor.ALUcom == 0)
        microprocessor.FZ = 1;
    else
        microprocessor.FZ = 0;
}

// IR signals

void IRin() {
    microprocessor.IR = microprocessor.dataBus;
}


// Memory requests

void readSignal() {
    microprocessor.DL = microprocessor.ram[microprocessor.AL];
    cycle_count += READ_WRITE_CYCLE;
}

void writeSignal() {
    microprocessor.ram[microprocessor.AL] = microprocessor.DL;
    cycle_count += READ_WRITE_CYCLE;
}

void readNextByte() {
    PCout(); ALin(); readSignal(); cycle_count += 1;
    AAout(), PCin(); cycle_count += 1;
    DLout(); IRin(); ALin(); cycle_count += 1;
}


microprocessor_t* getMicroProcessor() {
    return &microprocessor;
}

int* getCycleCount() {
    return &cycle_count;
}

void print_ram() {
    printf("\n-------------------\n Memory:\n\n");
    for (int i = 0; i < 1024; i++)
        printf("%02X ", (uint8_t)microprocessor.ram[i]);
    printf("\n");
}

void print_registers() {
    printf("\n\n-------------------\n Registers:\n\n");
    for (int i = 0; i < 8; i++)
        printf(" - R%d = %d\n", i, microprocessor.R[i]);
    printf("\n - PC = %.4X\n", (uint16_t)microprocessor.PC);
    printf(" - Data Latch = %.2X\n", (uint8_t)microprocessor.DL);
    printf(" - Data Bus = %.2X\n", (uint8_t)microprocessor.dataBus);
    printf(" - Address Latch = %.4X\n", (uint16_t)microprocessor.AL);
    printf(" - Address Bus = %.4X\n", (uint16_t)microprocessor.addressBus);
    printf(" - CS = %.2X\n", microprocessor.CS);
    printf(" - IR = %.2X\n", (uint8_t)microprocessor.IR);
    printf(" - X = %.2X\n", microprocessor.X);
    printf(" - Y = %.2X\n", microprocessor.Y);
    printf("\n-------------------\n Flags:\n\n");
    printf(" - FC = %d\n", microprocessor.FC);
    printf(" - FZ = %d\n", microprocessor.FZ);
}