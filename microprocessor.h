#include <stdint.h>
#include <stdio.h>

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
    ALUop ALUcom;
    int8_t F[2];
    int8_t ram[1024];
} microprocessor_t;

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
void subALU();
void incALU();
void decALU();
void andALU();
void notALU();

// IR signals
void IRin();

// Memory requests
void readSignal();
void writeSignal();

microprocessor_t* getMicroProcessor();