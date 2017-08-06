#include "transportDistance.h"
#include <time.h>

double readData() {
    static time_t occurTime = 0;
    
    int pin[8] = {45, 26, 46, 65, 22, 27, 47, 23}; //7, 20,
    int freq[7];
    int totalFreq;
    FILE *gpioE[8] *gpioC[8];
    
    for(int i = 0; i < 8; i++) {
        gpioE[i] = fopen("/sys/class/gpio/export", "w");
        fseek(gpioE[i],0,SEEK_SET);
        fprintf(gpioE[i], "%d", pin[i]);
        fflush(gpioE[i]);
    }
    
    gpioC[0] = fopen("/sys/class/gpio/gpio%d/direction", pin[0], "w");
    fseek(gpioC[i],0,SEEK_SET);
    fprintf(gpioC[0],"out")
    fflush(gpioC[0]);
    fclose(gpioC[0]);
    
    time_t currTime = time(NULL);
    currTime = currTime - occurTime;
    
    while(currTime < 10000) {
        gpioC[0] = fopen("/sys/class/gpio/gpio%d/value", pin[0], "w");
        fseek(gpioC[0],0,SEEK_SET);
        fprintf(gpioC[0],"1")
        fflush(gpioC[0]);
        fclose(gpioC[0]);
        
        for(int i = 1; i < 8; i++) {
            gpioC[i] = fopen("/sys/class/gpio/gpio%d/value", pin[i], "r");
            fseek(gpioC[i],0,SEEK_SET);
            fscanf(gpioC[i], "%d", &freq[i-1]); // write analog value to buffer
            fclose(gpioC[i]); // close buffer
        }
    }
        
    for(int i = 0; i < 7; i++) {
        totalFreq += (double) pow(2, freq[i]));
    }
    
    for(int i = 0; i < 8; i++) {
        fclose(gpioE[i]);
    }

    totalFreq = (unsigned int) (totalFreq / 10000 * pow(10, 9));
    
    return totalFreq;


}

void transportDistance(void* data) {
    
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;
    
    unsigned long* transportDis = data->transportDis
    unsigned int freq = 0;
    unsigned int transbuff[8];
    unsigned int buffCount[3] = 000;
    
    freq = readData();
    if(freq < highLimit && freq > lowLimit) {
        transportDis = freq * 100 / 6;
    } else if(freq >= highLimit) {
        transportDis = highLimit * 100 / 6;
    } else if(freq <= lowLimit) {
        transportDis = lowLimit * 100 / 6;
    }
    
    transbuff[buffCount] = lowLimit;
    period = readData();
    
    if(freq == 0.1 * transbuff[buffCount]) {
        buffCount++;
        transbuff[buffCount] = period;
    }
}
