#ifndef satelliteComs_h
#define satelliteComs_h

#include "constant.h"

// satellite communication function
void satelliteComs(void* data);

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


#endif /* satelliteComs_h */
