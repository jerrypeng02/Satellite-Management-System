#include "solarPanelControl.h"

// solar panel control function
void solarPanelControl(void* data) {
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;
    
    Bool solarPanelState = *((SolarPanelControlData*)data)->solarPanelState;
    Bool solarPanelDeploy = *((SolarPanelControlData*)data)->solarPanelDeploy;
    Bool solarPanelRetract = *((SolarPanelControlData*)data)->solarPanelRetract;
    Bool dmsInc = *((SolarPanelControlData*)data)->dmsInc;
    Bool dmsDec = *((SolarPanelControlData*)data)->dmsDec;
    
    
}
