#ifndef __CONSOLEDISPLAY_H__
#define __CONSOLEDISPLAY_H__

#include "constant.h"

// console display function
void consoleDisplay(void* data);

//
struct ConsoleDisplayDataStruct {
    Bool* display;
    Bool* fuelLow;
    Bool* batteryLow;
    Bool* solarPanelState;
    Bool* solarPanelRetract;
    Bool* solarPanelDeploy;
    unsigned short* batteryLev;
    unsigned short* fuelLev;
    unsigned short* powerCon;
    unsigned short* powerGen;
    unsigned long* transportDis;
    unsigned int** imageFrequencyPtr;
    double** batteryTempPtr1;
    double** batteryTempPtr2;
    Bool* batteryOverTemp;
};
typedef struct ConsoleDisplayDataStruct ConsoleDisplayData;


#endif /* consoleDisplay_h */

