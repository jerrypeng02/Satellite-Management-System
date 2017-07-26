//
//  solarPanelControl.c
//  
//
//  Created by Ningyang Peng on 7/25/17.
//
//

#include "solarPanelControl.h"

// solar panel control function
void solarPanelControl(void* data) {
    Bool solarPanelState = *((SolarPanelControlData*)data)->solarPanelState;
    Bool solarPanelDeploy = *((SolarPanelControlData*)data)->solarPanelDeploy;
    Bool solarPanelRetract = *((SolarPanelControlData*)data)->solarPanelRetract;
    Bool dmsInc = *((SolarPanelControlData*)data)->dmsInc;
    Bool dmsDec = *((SolarPanelControlData*)data)->dmsDec;
    
}
