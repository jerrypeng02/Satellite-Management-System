#ifndef satelliteComs_h
#define satelliteComs_h

#include "constant.h"

//
struct SatelliteComsDataStruct {
    Bool* fuelLow;
    Bool* batteryLow;
    Bool* solarPanelState;
    unsigned short* batteryLev;
    unsigned short* fuelLev;
    unsigned short* powerCon;
    unsigned short* powerGen;
    unsigned int* thrusterComm;
};
typedef struct SatelliteComsDataStruct SatelliteComsData;

extern char earthCommend;

// satellite communication function
void satelliteComs(void* data);

// output to earth
void earthOutput(char* output);


#endif /* satelliteComs_h */
