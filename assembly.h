#include "microprocessor.h"

void JMP();
void JZ();
void JC();
void JMP_RX0();
void ST_R0_RXn(int8_t Rn, int8_t Rm);
void LD_R0_RXn(int8_t Rn, int8_t Rm);
void ST(int8_t Rn);
void LD(int8_t Rn);
void MV_ARG(int8_t Rn);
void DEC(int8_t Rn);
void INC(int8_t Rn);
void ADD(int8_t Rn, int8_t Rm);
void SUB(int8_t Rn, int8_t Rm);
void AND(int8_t Rn, int8_t Rm);
void SWP(int8_t Rn, int8_t Rm);
void MV_Rn_Rm(int8_t Rn, int8_t Rm);