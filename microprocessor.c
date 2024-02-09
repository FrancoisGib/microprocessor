#include "microprocessor.h"
#include"decoder.h"
#include<stdio.h>
#include<math.h>
static microprocessor_t microprocessor;

MinimalBit minimalBit[8] = {
    {0,1,1},
    {2,3,2},
    {4,7,3},
    {8,15,4},
    {16,31,5},
    {32,63,6},
    {64,127,7},
    {128,255,8}
};
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
    int8_t result = microprocessor.X + microprocessor.Y;
    updateFlags(result);
}

void subALU() {
    microprocessor.ALUcom = (microprocessor.X - microprocessor.Y);
    int8_t result = microprocessor.X - microprocessor.Y;
    updateFlags(result);
}

void multALU() {
    microprocessor.ALUcom = (microprocessor.X * microprocessor.Y);
    int8_t result = microprocessor.X * microprocessor.Y;
    updateFlags(result);
}

void divALU() {
    microprocessor.ALUcom = (microprocessor.X / microprocessor.Y);
    int8_t result = microprocessor.X / microprocessor.Y;
    updateFlags(result);
}

void incALU() {
    microprocessor.ALUcom = (microprocessor.X + 1);
    int8_t result = microprocessor.X + 1;
    updateFlags(result);
}

void notALU(){
    microprocessor.ALUcom = (~microprocessor.X);
}

void andALU(){
    microprocessor.ALUcom = (microprocessor.X & microprocessor.Y);
}

void decALU() {
    microprocessor.ALUcom = (microprocessor.X - 1);
    int8_t result = microprocessor.X - 1;
    updateFlags(result);
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

int chechForCarry(int8_t result){
    int8_t num = fmax(microprocessor.X,microprocessor.Y);
    int8_t result_minimal_Bit = getMinimalBit(result);
    int8_t num_minimal_Bit = getMinimalBit(num);
    if(result_minimal_Bit != -1 && num_minimal_Bit != -1){
        return result_minimal_Bit > num_minimal_Bit; 
    } 
    return -1;
}

int8_t getMinimalBit(int8_t number){
        for (int i = 0; i < 8; i++)
    {
        if(number >= minimalBit[i].start && number <= minimalBit[i].end){
            return minimalBit[i].nb_Bit;
        }
    }
    return -1;
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
    else if (chechForCarry(result))
    {
        microprocessor.carryFlag = 1;
        microprocessor.signFlag = 0;
        microprocessor.zeroFlag = 0;
    }
    
}

int8_t getCarryFlag(){
    return microprocessor.carryFlag;
}
int8_t getZeroFlag(){
    return microprocessor.zeroFlag;
}
int8_t getSignFlag(){
    return microprocessor.signFlag;
}
int16_t getHHLL(){
    return microprocessor.AL;
}

microprocessor_t *getMicroProcessor(){
    return &microprocessor;
}

