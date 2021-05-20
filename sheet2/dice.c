#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int dice(void) {
    time_t t;
    time(&t);
    srand((unsigned int)t);
    return rand() % 6 + 1;
}

int main(void) {
    int roll = dice();
    printf("roll: %d\n", roll);
    return roll;
}
