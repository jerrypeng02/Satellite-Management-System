#include "transportDistance.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

// 20 is the reset signal
// 45 is the known gate
// 26 to the end are 7 bits from 0 - 6
const int pin[9] = {20, 45, 26, 46, 65, 22, 27, 47, 23};
char pinFile[9][100];

unsigned long readData() {
    static int freq[7];
    unsigned long totalFreq = 0;
    int i;
    
#ifdef BEAGLEBONE
    FILE *gpioC[9];

    gpioC[0] = fopen(pinFile[0], "w");
    fseek(gpioC[0],0,SEEK_SET);
    fprintf(gpioC[0],"1");
    fflush(gpioC[0]);
    fclose(gpioC[0]);

    usleep(100000);

    gpioC[0] = fopen(pinFile[0], "w");
    fseek(gpioC[0],0,SEEK_SET);
    fprintf(gpioC[0],"0");
    fflush(gpioC[0]);
    fclose(gpioC[0]);

    gpioC[1] = fopen(pinFile[1], "w");
    fseek(gpioC[1],0,SEEK_SET);
    fprintf(gpioC[1],"1");
    fflush(gpioC[1]);
    fclose(gpioC[1]);

    usleep(90000); // should it be after the for loop?
    
    for(i = 2; i < 9; i++) {
        gpioC[i] = fopen(pinFile[i], "r");
        fseek(gpioC[i],0,SEEK_SET);
        fscanf(gpioC[i], "%d", &freq[i-2]); // write analog value to buffer
        fclose(gpioC[i]); // close buffer
    }

    for(i = 0; i < 7; i++) {
        //freq[i] = 0;
        printf("%d", freq[i]);
    }
    printf("\n");

    // gpioC[0] = fopen(pinFile[0], "w");
    // fseek(gpioC[0],0,SEEK_SET);
    // fprintf(gpioC[0],"0");
    // fflush(gpioC[0]);
    // fclose(gpioC[0]);

    gpioC[1] = fopen(pinFile[1], "w");
    fseek(gpioC[1],0,SEEK_SET);
    fprintf(gpioC[1],"0");
    fflush(gpioC[1]);
    fclose(gpioC[1]);

#else

    freq[0] = 1;
    freq[1] = 1;
    freq[2] = 0;
    freq[3] = 0;
    freq[4] = 0;
    freq[5] = 0;
    freq[6] = 0;

#endif

    for(i = 0; i < 7; i++) {
        totalFreq += (1 << i) * freq[i];
    }

    printf("%lu\n", totalFreq);

    return totalFreq;
}

const unsigned long highLimit = 100;
const unsigned long lowLimit = 0;

void transportDistance(void* data) {
    
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;
    
    unsigned long* transportDis = ((TransportDistanceData*)data)->transportDis;
    signed long freq = 0;
    static int firstTime = 1;
    static unsigned long transbuff[8];
    static unsigned long freqbuff[16];
    static unsigned int disbuffCount = 0;
    static unsigned int freqbuffCount = 0;
    
    freq = readData();

    freqbuff[freqbuffCount] = freq;

    //printf("%lu\n", freqbuff[freqbuffCount]);

    freqbuffCount = (freqbuffCount + 1) % 16;

    //printf("%lu\n", freq);

    if(freq < highLimit && freq > lowLimit) {
        *transportDis = freq * (-19) + 2000;
    } else if(freq >= highLimit) {
        *transportDis = highLimit * (-19) + 2000;
    } else if(freq == lowLimit) {
        *transportDis = lowLimit * (-19) + 2000;
    }

    if (firstTime) {
        transbuff[disbuffCount] = *transportDis;
        firstTime = 0;
    } else {
        if(*transportDis >= 0.9 * transbuff[disbuffCount]) {
            transbuff[disbuffCount] = *transportDis;
        }
    }

    //printf("%lu\n", transbuff[disbuffCount]);

    disbuffCount = (disbuffCount + 1) % 8;

    //printf("%lu\n", *transportDis);
}

void enableGPIOforTransport() {
    int i;
    FILE* gpioE;
    for(i = 0; i < 9; i++) {
        gpioE = fopen("/sys/class/gpio/export", "w");
        fseek(gpioE,0,SEEK_SET);
        fprintf(gpioE, "%d", pin[i]);
        fflush(gpioE);
        fclose(gpioE);

        sprintf(pinFile[i], "/sys/class/gpio/gpio%d/value", pin[i]);
    }

    for(i = 0; i < 2; i++) {
        char temp[100];
        sprintf(temp, "/sys/class/gpio/gpio%d/direction", pin[i]);
        gpioE = fopen(temp, "w");
        fseek(gpioE, 0, SEEK_SET);
        fprintf(gpioE,"out");
        fflush(gpioE);
        fclose(gpioE);
    }
}
