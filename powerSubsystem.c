#include "powerSubsystem.h"
#include "constant.h"
#include <stdio.h>
#include <time.h>

unsigned int powerCount = 0;
Bool powerIncrease = TRUE;
// power subsystem function

void controlPower(PowerSubsystemData* data) {

    Bool* solarPanelState = data->solarPanelState;
    Bool* solarPanelDeploy = data->solarPanelDeploy;
    Bool* solarPanelRetract = data->solarPanelRetract;
    unsigned short* batteryLev = data->batteryLev;
    unsigned short* powerCon = data->powerCon;
    unsigned short* powerGen = data->powerGen;

    unsigned short motorDrive = 0;

    if(powerIncrease) {
        if (*powerCon <= 10){
            if (powerCount % 2 == 0) {
                *powerCon += 2;
            } else {
                *powerCon -= 1;
            }
        }
        if (*powerCon > 10) {
            powerIncrease = FALSE;
        }
    } else {
        if (*powerCon >= 5){
            if (powerCount % 2 == 0) {
                *powerCon -= 2;
            } else {
                *powerCon += 1;
            }
        }
        if (*powerCon < 5) {
            powerIncrease = TRUE;
        }
    }

    if(solarPanelState){
        if(*batteryLev > 95){
            *solarPanelState = FALSE;
            *powerGen = 0;
        }else{
            if (*batteryLev <= 50){
                if (powerCount % 2 == 0){
                    *powerGen += 2;
                } else {
                    *powerGen += 1;
                }
            } else {
                if (powerCount % 2 == 0){
                    *powerGen += 2;
                }
            }
        }
    }

    /*
     if(!*solarPanelState){
     if (*batteryLev < *powerCon * 3) {
     *batteryLev = 0;
     } else {
     // we change the battery level formulate to batterylev = batterylev - powerCon
     *batteryLev -= *powerCon * 3;
     }
     }else{
     if (*batteryLev < ((int) *powerCon) - ((int)*powerGen)) {
     *batteryLev = 0;
     } else {
     *batteryLev -= ((int) *powerCon) - ((int)*powerGen);
     }

     if (*batteryLev > 100)
     *batteryLev = 100;
     }
     */


    // we change the condition of opening solar panel to when the battery level is less than 40
<<<<<<< HEAD
    if (!solarPanelState && *batteryLevPtr <= 36) {
        *solarPanelDeploy = TRUE;
=======
    if (!solarPanelState && *batteryLev <= 36) {
        *solarPanelState = TRUE;
>>>>>>> 9e90fea3b40e21100ed1efa4e51c958e5278cbb2
    }
}


void readBatteryLevel(PowerSubsystemData* data) {

    unsigned int** batteryLevPtr = data->batteryLevPtr;
    unsigned short* batteryLev = data->batteryLev;
    unsigned short* powerCon = data->powerCon;
    unsigned short* powerGen = data->powerGen;
    Bool* solarPanelState = data->solarPanelState;

    FILE *ain,*aval0;
    volatile int i, j;

#ifdef BEAGLEBONE

    ain = fopen("/sys/devices/bone_capemgr.9/slots", "w");
    fseek(ain,0,SEEK_SET);
    fprintf(ain,"cape-bone-iio");
    fflush(ain);

    usleep(600);

    for(i = 0; i < 16; i++){

        aval0 = fopen("/sys/devices/ocp.3/helper.15/AIN0", "r");
        fseek(aval0, 0, SEEK_SET); // go to beginning of buffer
        fscanf(aval0, "%d", &(*batteryLevPtr[i])); // write analog value to buffer
        fclose(aval0); // close buffer

        // delay
        for(j = 0; j<1000;j++);
    }

    fclose(ain);

    int sum;
    for(i = 0; i < 16; i++){
        sum = sum + *batteryLevPtr[i];
    }
    sum /= 16;

    *batteryLev = sum / 1800 * 100;

#else
    if(!*solarPanelState){
        if (*batteryLev < *powerCon * 3) {
            *batteryLev = 0;
        } else {
            // we change the battery level formulate to batterylev = batterylev - powerCon
            *batteryLev -= *powerCon * 3;
        }
    }else{
        if (*batteryLev < ((int) *powerCon) - ((int)*powerGen)) {
            *batteryLev = 0;
        } else {
            *batteryLev -= ((int) *powerCon) - ((int)*powerGen);
        }

        if (*batteryLev > 100)
            *batteryLev = 100;
    }

#endif
}


void powerSubsystem(void* data) {

    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;

    readBatteryLevel((PowerSubsystemData*) data);
    controlPower((PowerSubsystemData*) data);

    powerCount ++;
}

