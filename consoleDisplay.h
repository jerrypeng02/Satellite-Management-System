#ifndef __CONSOLEDISPLAY_H__
#define __CONSOLEDISPLAY_H__

#include "constant.h"

// console display function
void consoleDisplay(void* data);

//
struct ConsoleDisplayDataStruct {
    Bool* fuelLow;
    Bool* batteryLow;
    Bool* solarPanelState;
    unsigned short* batteryLev;
    unsigned short* fuelLev;
    unsigned short* powerCon;
    unsigned short* powerGen;
    unsigned short* transportDis;
    unsigned int* batteryTemp;
};
typedef struct ConsoleDisplayDataStruct ConsoleDisplayData;


#endif /* consoleDisplay_h */

