#include <stdio.h>
#include <stdint.h>
#include"microprocessor.h"
#include"decoder.h"

int main() {
    microprocessor_t *proc = getMicroProcessor();
    proc->ram[0] = 0b10011010;
    proc->ram[1] = 0b10111010;
    proc->ram[2] = 0b11111010;
    proc->R[3] = 69;
    proc->R[2] = 1;
    proc->addressBus = 0;
    PCin();
    PCout();ALin();
    printf("FIRST:%d\n",proc->IR);
    read();
    DLout();IRin();
    printf("SECOND:%d\n",proc->IR);
    decodeOpcode(&proc->IR);
    AAout();
    printf("%d",proc->addressBus);
    printf("TERMINER:%d\n",proc->R[3]);
    PCin();
    PCout();ALin();
    printf("FIRST:%d\n",proc->IR);
    read();
    DLout();IRin();
    printf("SECOND:%d\n",proc->IR);
    decodeOpcode(&proc->IR);
    AAout();
    printf("TERMINER:%d\n",proc->R[3]);
}