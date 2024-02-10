#ifndef MICROPROCESSOR_H
#define MICROPROCESSOR_H

#define RAMSIZE 1024
#include <stdint.h>


typedef int8_t (*ALUop)(int8_t param);

typedef struct {
    int8_t R[8];
    int8_t CS;
    int16_t PC;
    int8_t DL;
    int16_t AL;
    int8_t dataBus;
    int16_t addressBus;
    int8_t IR;
    int8_t X;
    int8_t Y;
    int8_t zeroFlag;
    int8_t carryFlag;
    ALUop ALUcom;
    uint8_t ram[RAMSIZE];
    int8_t hasInstruction[RAMSIZE];
    int8_t controlUnit[3];
} microprocessor_t;

typedef struct{
    int8_t start;
    int8_t end;
    int8_t nb_Bit;
} MinimalBit;


// Function to get the singleton instance
microprocessor_t* getMicroProcessor();

// R registers signals
void SR(int i);
void Rin();
void Rout();

// PC signals
void PCin();
void PCout();
void PCHin();
void PCLin();

// DLatch signals
void DLout();
void DLin();

// AddressLatch signals
void ALin();
void ALHin();
void ALLin();
void AAout();

// ALU Signals
void Xin();
void Yin();
void RepX();
void RepY();
void ALUout();

// Micro-instructions for ALU
void addALU();
void andALU();
void subALU();
void multALU();
void divALU();
void incALU();
void decALU();
void notALU();

// IR signals
void IRin();

// Memory requests
void read();
void write();

//Flags Update
void updateFlags(int8_t result);
int8_t getMinimalBit(int8_t number);
int chechForCarry(int8_t result);
int8_t getCarryFlag();
int8_t getZeroFlag();
int16_t getHHLL();

#endif // MICROPROCESSOR_H