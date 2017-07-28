#include "solarPanelControl.h"
#include "constant.h"
#include "stdio.h"
#include "bbb.h"

// solar panel control function
unsigned short solarPanelProgress = 0;

void solarPanelControl(void* data) {
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;

    Bool* solarPanelState = ((SolarPanelControlData*)data)->solarPanelState;
    Bool* solarPanelDeploy = ((SolarPanelControlData*)data)->solarPanelDeploy;
    Bool* solarPanelRetract = ((SolarPanelControlData*)data)->solarPanelRetract;
    Bool* dmsInc = ((SolarPanelControlData*)data)->dmsInc;
    Bool* dmsDec = ((SolarPanelControlData*)data)->dmsDec;

    unsigned short moterDrive = 0;


    if(*dmsInc){
        if(moterDrive <100){
            moterDrive += 5;
        }else{
            moterDrive = 100;
        }
    }
    if(*dmsDec){
        if(moterDrive > 0){
            moterDrive -= 5;
        }else{
            moterDrive = 0;
        }
    }
    if(*solarPanelDeploy){
        solarPanelProgress += moterDrive;
    }
    if(*solarPanelRetract){
        solarPanelProgress -= moterDrive;
    }

    if(solarPanelProgress >= 100){
        *solarPanelState = TRUE;
        solarPanelProgress = 100;
    }
    if(solarPanelProgress <= 0){
        *solarPanelState = FALSE;
        solarPanelProgress = 0;
    }

    if(*solarPanelState && *solarPanelDeploy){
        moterDrive = 0;
        *solarPanelDeploy = FALSE;
    }
    if(!*solarPanelState && *solarPanelRetract){
        moterDrive = 0;
        *solarPanelRetract = FALSE;
    }



    int pwm = bbb_enablePwm(HEADER, PIN);
    int pwmPeriod = bbb_setPwmPeriod(HEADER, PIN, PERIOD);
    int pwmDuty = bbb_setPwmDuty(HEADER, PIN, moterDrive/100 * PERIOD);



    /*
    Bool pwm;

    int counter = 0;

    while(1){
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
        // delay
        for(j = 0; j<1000000;j++);
    }

    solarPanelDeploy = &pwm;
    solarPanelRetract = &pwm;
    */







}

