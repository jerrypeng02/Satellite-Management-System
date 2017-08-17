#include "imageCapture.h"
#include "optfft.h"
#include "constant.h"

#include <stdio.h>

int x[256], y[256];
unsigned int imageFrequencyData[16];

void imageCapture(void* data) {
    
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;

    unsigned int** imageFrequencyPtr = ((ImageCaptureData*) data) -> imageFrequencyPtr;
    unsigned int frequency;
    volatile int i;

#ifdef BEAGLEBONE

    FILE *ain,*aval0;

    unsigned int maxF;
    int maxA;

    ain = fopen("/sys/devices/bone_capemgr.9/slots", "w");
    fseek(ain,0,SEEK_SET);
    fprintf(ain,"cape-bone-iio");
    fflush(ain);

    usleep(600);

    FILE* ftemp = fopen("data.txt", "w");

    for(i = 0; i < 256; i++){
        aval0 = fopen("/sys/devices/ocp.3/helper.15/AIN3", "r");
        fseek(aval0, 0, SEEK_SET); // go to beginning of buffer
        fscanf(aval0, "%d", &x[i]); // write analog value to buffer
        fclose(aval0); // close buffer

        fprintf(ftemp, "%d ", x[i]);

        x[i] = x[i] * 64 / 1800 - 32;
        y[i] = 0;

        // 1,000,000 / 35 / 256 = 111 us
        usleep(130);
    }
    fclose(ain);

    fprintf(ftemp, "\n", x[i]);
    fclose(ftemp);

    maxF = optfft(x, y);

    /*
    for(i = 0; i < 256; i++){
        if (maxA < y[i]) {
            maxA = y[i];
            maxF = 256 - i;
        }
    }*/

    frequency = 7500 * maxF / 256; 

#else
    frequency = 1000;
#endif
    if (*imageFrequencyPtr == NULL)
        *imageFrequencyPtr = imageFrequencyData;

    if (*imageFrequencyPtr == &imageFrequencyData[15]) {
        *imageFrequencyPtr = imageFrequencyData;
    } else {
        *imageFrequencyPtr = *imageFrequencyPtr + 1;
    }

    **imageFrequencyPtr = frequency;
}
