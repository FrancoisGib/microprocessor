#include "microprocessor.h"
#include"decoder.h"
#include<stdio.h>
#include<math.h>
static microprocessor_t microprocessor;

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
    microprocessor.PC = 0;
    microprocessor.PC |= (int16_t)(microprocessor.dataBus << 8);
}

void PCLin() {
    microprocessor.PC |= (int16_t)(microprocessor.dataBus);
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
    // printf("High Bit %02X\n",microprocessor.dataBus);
    microprocessor.AL = 0;
    // printf("AL Before H %hx\n",microprocessor.AL);
    microprocessor.AL |= (int16_t)(microprocessor.dataBus << 8);
    // printf("AL After H %d\n",microprocessor.AL);
}

void ALLin() {
    // printf("Low Bit %02X\n",microprocessor.dataBus);
    // printf("AL Before H %hx\n",microprocessor.AL);
    microprocessor.AL |= (int16_t)(microprocessor.dataBus);
    // printf("AL After L %d\n",microprocessor.AL);

}

void AAout() {
    microprocessor.addressBus = microprocessor.addressBus+1;
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

int chechForCarry(){
    int8_t num = max(microprocessor.X,microprocessor.Y);
    for (int i = 0; i < 8; i++)
    {
        if(num >= minimalBit[i] && )
    }
    
}
void updateFlags(int8_t result){
    if(result == 0){
        microprocessor.zeroFlag = 1;
        microprocessor.signFlag = 0;
        microprocessor.carryFlag = 0;
    }
    else if(result < 0){
       microprocessor.signFlag = 1;
       microprocessor.zeroFlag = 0;
       microprocessor.carryFlag = 0;
    }
    else if (result > 255)
    {
        return;
    }
    
}
microprocessor_t *getMicroProcessor(){
    return &microprocessor;
}

