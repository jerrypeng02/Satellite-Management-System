#ifndef solarPanelControl_h
#define solarPanelControl_h

#include "constant.h"

// solar panel control function
void solarPanelControl(void* data);
void enablePWMforSolarPanelControl();
//
struct SolarPanelControlDataStruct {
    Bool* solarPanelState;
    Bool* solarPanelDeploy;
    Bool* solarPanelRetract;
    Bool* dmsInc;
    Bool* dmsDec;
    unsigned short* motorDriveSpeed;    
};
typedef struct SolarPanelControlDataStruct SolarPanelControlData;

#endif /* solarPanelControl_h */
