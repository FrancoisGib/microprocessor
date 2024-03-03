#include "assembly.h"

#define microprocessor getMicroProcessor()
#define cycle_count getCycleCount()

void JMP(int8_t status) {
    readSignal();
    int8_t pcl = microprocessor->DL;
    DLout(); PCHin(); AAout(); ALin(); readSignal(); *cycle_count += 1;
    int8_t pch = microprocessor->DL;
    DLout(); PCLin(); *cycle_count += 1;
    if (status == 0)
        printf("JMP %.2X%.2X", pcl, pch);
    else if (status == 1)
        printf("JC %.2X%.2X", pcl, pch);
    else
        printf("JZ %.2X%.2X", pcl, pch);
}


void JMP_RX0() {
    SR(0); Rout(); PCLin(); *cycle_count += 1;
    SR(1); Rout(); PCHin(); *cycle_count += 1;
    printf("JMP %%RX0");
}

void ST_R0_RXn(int8_t n) {
    SR(n); Rout(); ALLin(); *cycle_count += 1;
    SR(n + 1); Rout(); ALHin(); *cycle_count += 1;
    SR(0); Rout(); DLin(); writeSignal(); *cycle_count += 1;
    printf("ST %%R0 %%RX%d", n);
}

void LD_R0_RXn(int8_t n) {
    SR(n); Rout(); ALLin(); *cycle_count += 1;
    SR(n + 1); Rout(); ALHin(); readSignal(); *cycle_count += 1;
    SR(0); DLout(); Rin();
    printf("LD %%R0 %%RX%d", n);
}

void ST(int8_t Rn) {
    readSignal();
    DLout(); Xin(); AAout(); ALin(); readSignal(); *cycle_count += 1;
    AAout(); PCin(); DLout(); ALHin(); *cycle_count += 1;
    RepX(); ALUout(); ALLin(); *cycle_count += 1;
    SR(Rn); Rout(); DLin(); writeSignal(); *cycle_count += 1;
    printf("ST %%R%d", Rn);
}

void LD(int8_t Rn) {
    readSignal();
    DLout(); Xin(); AAout(); ALin(); readSignal(); *cycle_count += 1;
    AAout(); PCin(); DLout(); ALHin(); *cycle_count += 1;
    RepX(); ALUout(); ALLin(); readSignal(); *cycle_count += 1;
    DLout(); SR(Rn); Rin(); *cycle_count += 1;
    printf("LD %%R%d", Rn);
}

void MV_ARG(int8_t Rn) {
    readSignal(); AAout(); PCin(); *cycle_count += 1;
    int8_t arg = microprocessor->DL;
    DLout(); SR(Rn); Rin(); *cycle_count += 1;
    printf("MV %%R%d, $%d", Rn, arg);
}

void DEC(int8_t Rn) {
    SR(Rn); Rout(); Xin(); *cycle_count += 1;
    decALU(); ALUout(); Rin(); *cycle_count += 1;
    printf("DEC %%R%d", Rn);
}

void INC(int8_t Rn) {
    SR(Rn); Rout(); Xin(); *cycle_count += 1;
    incALU(); ALUout(); Rin(); *cycle_count += 1;
    printf("INC %%R%d", Rn);
}

void ADD(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); Xin(); *cycle_count += 1;
    SR(Rm); Rout(); Yin(); *cycle_count += 1;
    addALU(); ALUout(); SR(Rn); Rin(); *cycle_count += 1;
    printf("ADD %%R%d, %%R%d", Rn, Rm);
}

void SUB(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); Xin(); *cycle_count += 1;
    SR(Rm); Rout(); Yin(); *cycle_count += 1;
    subALU(); ALUout(); SR(Rn); Rin(); *cycle_count += 1;
    printf("SUB %%R%d, %%R%d", Rn, Rm);
}

void AND(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); Xin(); *cycle_count += 1;
    SR(Rm); Rout(); Yin(); *cycle_count += 1;
    andALU(); ALUout(); SR(Rn); Rin(); *cycle_count += 1;
    printf("AND %%R%d, %%R%d", Rn, Rm);
}

void NOT(int8_t Rn) {
    SR(Rn); Rout(); Xin(); *cycle_count += 1;
    notALU(); ALUout(); SR(Rn); Rin(); *cycle_count += 1;
    printf("NOT %%R%d", Rn);
}

void SWP(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); Xin(); *cycle_count += 1;
    SR(Rm); Rout(); Yin(); *cycle_count += 1;
    RepX(); ALUout(); SR(Rm); Rin(); *cycle_count += 1;
    RepY(); ALUout(); SR(Rn); Rin(); *cycle_count += 1;
    printf("SWP %%R%d, %%R%d", Rn, Rm);
}

void MV_Rn_Rm(int8_t Rn, int8_t Rm) {
    SR(Rm); Rout(); *cycle_count += 1;
    SR(Rn); Rin(); *cycle_count += 1;
    printf("MV %%R%d, %%R%d", Rn, Rm);
}