#include "satelliteComs.h"
#include "constant.h"
#include "util.h"

// satellite communication function
void satelliteComs(void* data) {
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;
    
    // error structure name error
    *((SatelliteComsData*)data)->thrusterComm = randomInteger(0, 0xFFFF);
    //printf("command: %d", thrusterComm);
    
    Bool fuelLow = *((SatelliteComsData*)data)->fuelLow;
    Bool batteryLow = *((SatelliteComsData*)data)->batteryLow;
    Bool solarPanelState = *((SatelliteComsData*)data)->solarPanelState;
    unsigned short batteryLev = *((SatelliteComsData*)data)->batteryLev;
    unsigned short fuelLev = *((SatelliteComsData*)data)->fuelLev;
    unsigned short powerCon = *((SatelliteComsData*)data)->powerCon;
    unsigned short powerGen = *((SatelliteComsData*)data)->powerGen;
    
    // encode the status information from satellite to earth
    unsigned int result = (fuelLow << 3) | (batteryLow << 2) | (solarPanelState << 1)
    | (powerCon << 4) | (batteryLev << 8)
    | (fuelLev << 16) | (powerGen << 24);
    
}
