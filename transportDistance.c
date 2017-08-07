#include "transportDistance.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

const int pin[8] = {45, 26, 46, 65, 22, 27, 47, 23};
char pinFile[8][100];

unsigned long readData() {
    static time_t occurTime = 0;
    //7, 20,
    int freq[7];
    unsigned long totalFreq = 0;
    int i;

#ifdef BEAGLEBONE
    FILE *gpioC[8];    

    gpioC[0] = fopen(pinFile[0], "w");
    fseek(gpioC[0],0,SEEK_SET);
    fprintf(gpioC[0],"1");
    fflush(gpioC[0]);
    fclose(gpioC[0]);

    usleep(1000);
    
    for(i = 1; i < 8; i++) {
        gpioC[i] = fopen(pinFile[i], "r");
        fseek(gpioC[i],0,SEEK_SET);
        fscanf(gpioC[i], "%d", &freq[i-1]); // write analog value to buffer
        fclose(gpioC[i]); // close buffer
    }

    gpioC[0] = fopen(pinFile[0], "w");
    fseek(gpioC[0],0,SEEK_SET);
    fprintf(gpioC[0],"0");
    fflush(gpioC[0]);
    fclose(gpioC[0]);
    
    /*for(int i = 0; i < 8; i++) {
        fclose(gpioE[i]);
    }*/
 /// 10000 * pow(10, 9));
#else

    freq[0] = 1;
    freq[1] = 0;
    freq[2] = 0;
    freq[3] = 0;
    freq[4] = 0;
    freq[5] = 1;
    freq[6] = 1;

#endif
    for(i = 0; i < 7; i++) {
        totalFreq += (1 << i) * freq[i];
    }

    return totalFreq;
}

const unsigned long highLimit = 120;
const unsigned long lowLimit = 6;

void transportDistance(void* data) {
    
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;
    
    unsigned long* transportDis = ((TransportDistanceData*)data)->transportDis;
    unsigned long freq = 0;
    static int firstTime = 1;
    static unsigned long transbuff[8];
    static unsigned int buffCount = 0;
    
    freq = readData();
    //printf("%lu\n", freq);
    if(freq < highLimit && freq > lowLimit) {
        *transportDis = freq * 100 / 6;
    } else if(freq >= highLimit) {
        *transportDis = highLimit * 100 / 6;
    } else if(freq <= lowLimit) {
        *transportDis = lowLimit * 100 / 6;
    }

    if (firstTime) {
        transbuff[buffCount] = *transportDis;
        firstTime = 0;
    } else {
        if(*transportDis >= 1.1 * transbuff[buffCount]) {
            transbuff[buffCount] = *transportDis;
        }
    }
    buffCount = (buffCount + 1) % 8;
}

void enableGPIOforTransport() {
    int i;
    FILE* gpioE;
    for(i = 0; i < 8; i++) {
        gpioE = fopen("/sys/class/gpio/export", "w");
        fseek(gpioE,0,SEEK_SET);
        fprintf(gpioE, "%d", pin[i]);
        fflush(gpioE);
        fclose(gpioE);

        sprintf(pinFile[i], "/sys/class/gpio/gpio%d/value", pin[i]);
    }

    char temp[100];
    sprintf(temp, "/sys/class/gpio/gpio%d/direction", pin[0]);
    gpioE = fopen(temp, "w");
    fseek(gpioE, 0, SEEK_SET);
    fprintf(gpioE,"out");
    fflush(gpioE);
    fclose(gpioE);
}
