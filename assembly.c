#include "assembly.h"

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

void MULT(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); Xin();
    SR(Rm), Rout(); Yin();
    multALU(); ALUout(); SR(Rn); Rin();
}

void DIV(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); Xin();
    SR(Rm); Rout(); Yin();
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