#include "assembly.h"
#include "microprocessor.h"
#include <stdio.h>

JointRegisterDict jointRegisterDict[NUM_JOINT_REGISTERS] = {
    {0,0,1},
    {1,2,3},
    {2,4,5},
    {3,6,7}
};

void ADD(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); Xin();
    SR(Rm), Rout(); Yin();
    addALU(); ALUout(); SR(Rn); Rin();
    AAout();PCin();
}

void DEC(int8_t Rn){
    SR(Rn); Rout(); Xin();
    decALU();ALUout(); SR(Rn); Rin();
    AAout();PCin();
}

void INC(int8_t Rn){
    SR(Rn); Rout(); Xin();
    incALU();ALUout(); SR(Rn); Rin();
    AAout();PCin();
}

void NOT(int8_t Rn){
    SR(Rn); Rout(); Xin();
    notALU();ALUout(); SR(Rn); Rin();
    AAout();PCin();
}

void AND(int8_t Rn, int8_t Rm){
    SR(Rn); Rout(); Xin();
    SR(Rm), Rout(); Yin();
    andALU(); ALUout(); SR(Rn); Rin();
    AAout();PCin();
}

void SUB(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); Xin();
    SR(Rm); Rout(); Yin();
    subALU(); ALUout(); SR(Rn); Rin();
    AAout();PCin();
}

void MULT(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); Xin();
    SR(Rm); Rout(); Yin();
    multALU(); ALUout(); SR(Rn); Rin();
    AAout();PCin();
}

void DIV(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); Xin();
    SR(Rm); Rout(); Yin();
    divALU(); ALUout(); SR(Rn); Rin();
    AAout();PCin();
}

void MV(int8_t Rn, int8_t Rm) {
    SR(Rm); Rout();
    SR(Rn); Rin();
    AAout();PCin();
}

void MV_Rn_arg(int8_t Rn){
    AAout();ALin();
    read();
    DLout();SR(Rn);Rin();
    AAout();PCin();
}

void SWP(int8_t Rn, int8_t Rm) {
    SR(Rn); Rout(); Xin();
    SR(Rm); Rout(); Yin();
    RepX(); ALUout(); SR(Rm); Rin();
    RepY(); ALUout(); SR(Rn); Rin();
    AAout();PCin();
}

void ST_Rn_HHLL(int8_t Rn){
    AAout();ALin();
    read();
    DLout();Xin();AAout();ALin();read();
    DLout();ALHin();
    RepX();ALUout();ALLin();
    SR(Rn);Rout();write();
    AAout();PCin();
}

void LD_RN_HHLL(int8_t Rn){
    AAout();ALin();
    read();
    DLout();Xin();AAout();ALin();read();
    DLout();ALHin();
    RepX();ALUout();ALLin();read();
    DLout();SR(Rn),Rin();
    AAout();PCin();
}

void getJointRegisters(int8_t* firstRegister, int8_t* secondRegister,int8_t num){
    for (int i = 0; i < NUM_JOINT_REGISTERS; i++)
    {   
        if(num == jointRegisterDict[i].jointRegisterNum){
            *firstRegister = jointRegisterDict[i].register1;
            *secondRegister = jointRegisterDict[i].register2;
        }
    }
}

void LD_R0_RXn(int8_t RXn){
    int8_t firstRegsiter;
    int8_t secondRegister;
    getJointRegisters(&firstRegsiter,&secondRegister,RXn);
    SR(firstRegsiter),Rout();ALLin();
    SR(secondRegister),Rout();ALHin();read();
    SR(0),DLout();Rin();
    AAout();PCin();
} 

void ST_R0_RXn(int8_t RXn){
    int8_t firstRegsiter;
    int8_t secondRegister;
    getJointRegisters(&firstRegsiter,&secondRegister,RXn);
    SR(firstRegsiter);Rout();ALLin();
    SR(secondRegister),Rout();ALHin();
    SR(0),Rout(),DLin();write();
    AAout();PCin();
}

void JMP_HHLL(){
    AAout();ALin();
    read();
    DLout();Xin();AAout();ALin();read();
    DLout();PCHin();
    RepX(); ALUout(); PCLin();
}

void JZ_HHLL(){
    AAout();ALin();
    read();
    if(getZeroFlag()){
        DLout();Xin();AAout();ALin();read();
        DLout();PCHin();
        RepX(); ALUout(); PCLin();
    }
    else{
        AAout();AAout();PCin();
    }
}

void JC_HHLL(){
    AAout();ALin();
    read();
    if(getCarryFlag()){
        DLout();Xin();AAout();ALin();read();
        DLout();PCHin();
        RepX(); ALUout(); PCLin();
    }
    else{
        AAout();AAout();PCin();
    }
}

void JMP_RX0(){
    SR(0); Rout(); PCLin();
    SR(1); Rout(); PCHin();
}
int16_t getHHLL_AS(){
    return getHHLL();
}