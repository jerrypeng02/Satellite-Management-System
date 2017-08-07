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

    unsigned short motorDrive = 0;


    if(*dmsInc){
        if(motorDrive <100){
            motorDrive += 5;
        }else{
            motorDrive = 100;
        }
    }
    if(*dmsDec){
        if(motorDrive > 0){
            motorDrive -= 5;
        }else{
            motorDrive = 0;
        }
    }
    if(*solarPanelDeploy){
        solarPanelProgress += motorDrive;
    }
    if(*solarPanelRetract){
        solarPanelProgress -= motorDrive;
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
        motorDrive = 0;
        *solarPanelDeploy = FALSE;
    }
    if(!*solarPanelState && *solarPanelRetract){
        motorDrive = 0;
        *solarPanelRetract = FALSE;
    }



    int pwm = bbb_enablePwm(HEADER, PIN);
    int pwmPeriod = bbb_setPwmPeriod(HEADER, PIN, PERIOD);
    int pwmDuty = bbb_setPwmDuty(HEADER, PIN, motorDrive * PERIOD / 100);



    /*
    Bool pwm;

    int counter = 0;

        if(counter <= motorDrive){
            pwm = TRUE;
        }else{
            pwm = FALSE;
        }

        if(dmsInc){
            motorDrive += 5;
        }
        if(dmsDec){
            motorDrive -= 5;
        }

        counter++;
        if(counter > 100){
            counter = 0;
        }

    solarPanelDeploy = &pwm;
    solarPanelRetract = &pwm;
<<<<<<< HEAD
    */


}

