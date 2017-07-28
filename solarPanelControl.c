#include "solarPanelControl.h"

// solar panel control function
void solarPanelControl(void* data) {
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;

    Bool* solarPanelState = ((SolarPanelControlData*)data)->solarPanelState;
    Bool* solarPanelDeploy = ((SolarPanelControlData*)data)->solarPanelDeploy;
    Bool* solarPanelRetract = ((SolarPanelControlData*)data)->solarPanelRetract;
    Bool* dmsInc = ((SolarPanelControlData*)data)->dmsInc;
    Bool* dmsDec = ((SolarPanelControlData*)data)->dmsDec;

    unsigned short moterDrive = 0;
    Bool pwm;

    int counter = 0;

        if(counter <= moterDrive){
            pwm = TRUE;
        }else{
            pwm = FALSE;
        }

        if(dmsInc){
            moterDrive += 5;
        }
        if(dmsDec){
            moterDrive -= 5;
        }

        counter++;
        if(counter > 100){
            counter = 0;
        }

    solarPanelDeploy = &pwm;
    solarPanelRetract = &pwm;

}

