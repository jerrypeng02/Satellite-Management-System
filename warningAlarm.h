#ifndef warningAlarm_h
#define warningAlarm_h

#include "constant.h"

// warning alarm function
void warningAlarm(void* data);

//
struct WarningAlarmDataStruct {
    Bool* fuelLow;
    Bool* batteryLow;
    unsigned short* batteryLev;
    unsigned short* fuelLev;
};
typedef struct WarningAlarmDataStruct WarningAlarmData;


#endif /* warningAlarm_h */
