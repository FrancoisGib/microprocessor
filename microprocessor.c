#include <stdint.h>
#include <stdio.h>

typedef int8_t (*operator)(int8_t param);

typedef struct {
    int8_t R[8];
    int16_t PC;
    int8_t DL;
    int16_t AL;
    int8_t IR;
    int8_t X;
    int8_t Y;
    int8_t dataBus;
    int16_t addressBus;
    int8_t CS;
    int16_t ram[1024];
    operator ALUcom;
    int8_t* ALUoutRegister;
} microprocessor_t;

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

void DLint() {
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

void RepX() {
    microprocessor.ALUoutRegister = &microprocessor.X;
}

void RepY() {
    microprocessor.ALUoutRegister = &microprocessor.Y;
}

void ALUout() {
    *microprocessor.ALUoutRegister = microprocessor.ALUcom;
    microprocessor.ALUoutRegister = &microprocessor.dataBus;
}

// Operators for ALU

void ADD() {
    microprocessor.ALUcom = (microprocessor.X + microprocessor.Y);
}

void SUB() {
    microprocessor.ALUcom = (microprocessor.X - microprocessor.Y);
}

void MULT() {
    microprocessor.ALUcom = (microprocessor.X * microprocessor.Y);
}

void DIV() {
    microprocessor.ALUcom = (microprocessor.X / microprocessor.Y);
}


// IR signals

void IRin() {
    microprocessor.IR = microprocessor.dataBus;
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
    microprocessor.X = 5;
    microprocessor.Y = 2;
    microprocessor.ALUoutRegister = &microprocessor.dataBus;
    MULT();
    ALUout();
    printf("%d", microprocessor.dataBus);
}