#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "simulator.h"

int add_with_carry(int8_t a, int8_t b, uint8_t* result) {
    *result = a + b;

    // Check for carry
    return (*result > a) && (*result > b);
}

int main() {
    // startSimulation();
    int8_t a = 0b10;
    int8_t b = 0b01;
    int8_t result;

    if (add_with_carry(a, b, &result)) {
        printf("Carry occurred!\n");
    } else {
        printf("No carry.\n");
    }

    return 0;
}