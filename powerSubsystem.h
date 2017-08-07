#ifndef powerSubsystem_h
#define powerSubsystem_h

#include "constant.h"

// power subsystem function
void powerSubsystem(void* data);

//
struct PowerSubsystemDataStruct {
    Bool* solarPanelState;
    Bool* solarPanelDeploy;
    Bool* solarPanelRetract;
    unsigned int** batteryLevPtr;
    unsigned short* batteryLev;
    unsigned short* powerCon;
    unsigned short* powerGen;
    double** batteryTempPtr1;
    double** batteryTempPtr2;
    Bool* batteryOverTemp;
};
typedef struct PowerSubsystemDataStruct PowerSubsystemData;


#endif /* powerSubsystem_h */
