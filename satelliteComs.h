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


// satellite communication function
void satelliteComs(void* data);

char* getParamValuePtr(const char* name);
char* getEarthCommand();
char* getEarthPayload();
void clearEarthCommand();

#endif /* satelliteComs_h */
