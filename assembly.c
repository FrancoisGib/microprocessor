#include "assembly.h"

void JMP() {
    PCout(); ALin(); readSignal();
    DLout(); PCLin(); AAout(); ALin(); readSignal();
    DLout(); PCHin();
}

void JZ() {
    ZFout();
    microprocessor_t* micro = getMicroProcessor();
    if (micro->dataBus == 1)
        JMP();
}

void JC() {
    CFout();
    microprocessor_t* micro = getMicroProcessor();
    if (micro->dataBus == 1)
        JMP();
}

void JMP_RX0() {
    SR(0); Rout(); PCLin();
    SR(1); Rout(); PCHin();
}

void ST_R0_RXn(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); ALLin();
    SR(Rm); Rout(); ALHin();
    SR(0); Rout(); DLin(); writeSignal();
}

void LD_R0_RXn(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); ALLin();
    SR(Rm); Rout(); ALHin(); readSignal();
    SR(0); DLout(); Rin();
}

void ST(int8_t Rn) {
    readSignal();
    DLout(); Xin(); AAout(); ALin(); readSignal();
    AAout(); PCin();
    DLout(); ALHin();
    RepX(); ALUout(); ALLin();
    SR(Rn); Rout(); writeSignal();
}

void LD(int8_t Rn) {
    readSignal();
    DLout(); Xin(); AAout(); ALin(); readSignal();
    AAout(); PCin();
    DLout(); ALHin();
    RepX(); ALUout(); ALLin(); readSignal();
    DLout(); SR(Rn); Rin();
}

void MV_ARG(int8_t Rn) {
    readSignal(); AAout(); PCin();
    DLout(); SR(Rn); Rin();
}

void DEC(int8_t Rn) {
    SR(Rn); Rout(); Xin();
    decALU(); ALUout(); Rin();
}

void INC(int8_t Rn) {
    SR(Rn); Rout(); Xin();
    incALU(); ALUout(); Rin();
}

void ADD(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); Xin();
    SR(Rm); Rout(); Yin();
    addALU(); ALUout(); SR(Rn); Rin();
}

void SUB(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); Xin();
    SR(Rm); Rout(); Yin();
    subALU(); ALUout(); SR(Rn); Rin();
}

void AND(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); Xin();
    SR(Rm); Rout(); Yin();
    andALU(); ALUout(); SR(Rn); Rin();
}

void SWP(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); Xin();
    SR(Rm); Rout(); Yin();
    RepX(); ALUout(); SR(Rm); Rin();
    RepY(); ALUout(); SR(Rn); Rin();
}

void MV_Rn_Rm(int8_t Rn, int8_t Rm) {
    SR(Rm); Rout();
    SR(Rn); Rin();
}

void NOP() {}