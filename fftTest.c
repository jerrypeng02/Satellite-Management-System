#include "optfft.c"
#include <stdio.h>
#include <math.h>

#define PI 3.1415926535

int x[256], y[256];

void main() {
    int i;
    for (i = 0; i < 256; i ++) {
        x[i] = sin(PI * 2.0f / 256.0f * i * 20) * 900 + sin(PI * 2.0f / 256.0f * i * 2.5) * 100;
        y[i] = 0;
        printf("%d %d\n", x[i], y[i]);
    }
    optfft(x, y);
    for (i = 0; i < 256; i ++) {
        printf("%d %d\n", x[i], y[i]);
    }
}
