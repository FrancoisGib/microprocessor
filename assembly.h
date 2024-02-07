#include <stdint.h>

#define NUM_JOINT_REGISTERS 4
void ADD(int8_t Rn, int8_t Rm);
void SUB(int8_t Rn, int8_t Rm);
void MULT(int8_t Rn, int8_t Rm);
void DIV(int8_t Rn, int8_t Rm);
void MV(int8_t Rn, int8_t Rm);
void SWP(int8_t Rn, int8_t Rm);
void ST_Rn_HHLL(int8_t Rn);
void LD_RN_HHLL(int8_t Rn);
void JZ_HHLL();
void JC_HHLL();
void JMP_RX0();
void JMP_HHLL();
void ST_R0_RXn(int8_t RXn);
void LD_R0_RXn(int8_t RXn);
void MV_Rn_arg(int8_t Rn);
void DEC(int8_t Rn);
void INC(int8_t Rn);
void NOT(int8_t Rn);
void AND(int8_t Rn, int8_t Rm);

typedef struct
{
    int8_t jointRegisterNum;
    int8_t register1;
    int8_t register2;
} JointRegisterDict;

void getJointRegisters(int8_t* firstRegister, int8_t* secondRegister,int8_t num);
int16_t getHHLL_AS();