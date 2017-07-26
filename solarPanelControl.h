#ifndef solarPanelControl_h
#define solarPanelControl_h

#include "constant.h"

// solar panel control function
void solarPanelControl(void* data);

//
struct SolarPanelControlDataStruct {
    Bool* solarPanelState;
    Bool* solarPanelDeploy;
    Bool* solarPanelRetract;
    Bool* dmsInc;
    Bool* dmsDec;
};
typedef struct SolarPanelControlDataStruct SolarPanelControlData;

#endif /* solarPanelControl_h */
