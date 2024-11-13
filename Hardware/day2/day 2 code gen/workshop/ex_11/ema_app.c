#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "calcEMA.h"

int main() {
    double data[100];
    double ema[100];

    srand(time(NULL));
    for (int i=0; i < 100; i++) {
        data[i] = (double)rand()/RAND_MAX;
        printf("%.2f,", data[i]);
    }
    printf("\n");
    calcEMA(data, 8, ema);
    for (int i=0; i < 100; i++) {
        printf("%.2f,", ema[i]);
    }
    printf("\n");
}