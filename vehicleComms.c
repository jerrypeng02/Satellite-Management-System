#include "vehicleComms.h"
#include "constant.h"

// vehicle communication function
void vehicleComms(void* data) {
    char command = *((VehicleCommsData*)data)->command;
    char response = *((VehicleCommsData*)data)->response;
    
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;
    
    
}
