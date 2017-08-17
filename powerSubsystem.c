#include "powerSubsystem.h"
#include "constant.h"
#include <stdio.h>
#include <time.h>
#include <signal.h>

unsigned int powerCount = 0;
Bool powerIncrease = TRUE;
// power subsystem function

void controlPower(PowerSubsystemData* data) {

    Bool* solarPanelState = data->solarPanelState;
    Bool* solarPanelDeploy = data->solarPanelDeploy;
    Bool* solarPanelRetract = data->solarPanelRetract;
    unsigned int** batteryLevPtr = data->batteryLevPtr;
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
            isrNum = 2;
            signal(SIGUSR1);
            *solarPanelRetract = TRUE;
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


    if (!solarPanelState && *batteryLev <= 50) {
        *solarPanelDeploy = TRUE;
        isrNum = 2;
        signal(SIGUSR1);
    }
    *solarPanelDeploy = TRUE;
}


void readBatteryTemp(PowerSubsystemData* data) {

    Bool* batteryOverTemp = data->batteryOverTemp;
    double** batteryTempPtr1 = data->batteryTempPtr1;
    double** batteryTempPtr2 = data->batteryTempPtr2;

    if (*batteryTempPtr1 == &batteryTemp1[15]) {
        *batteryTempPtr1 = batteryTemp1;
    } else {
        *batteryTempPtr1 = *batteryTempPtr1 + 1;
    }

    if (*batteryTempPtr2 == &batteryTemp2[15]) {
        *batteryTempPtr2 = batteryTemp2;
    } else {
        *batteryTempPtr2 = *batteryTempPtr2 + 1;
    }

#ifdef BEAGLEBONE

    FILE *ain,*aval0,*aval1;
    int temp;

    ain = fopen("/sys/devices/bone_capemgr.9/slots", "w");
    fseek(ain,0,SEEK_SET);
    fprintf(ain,"cape-bone-iio");
    fflush(ain);

    usleep(500);

    aval0 = fopen("/sys/devices/ocp.3/helper.15/AIN1", "r");
    fseek(aval0, 0, SEEK_SET); // go to beginning of buffer
    fscanf(aval0, "%d", &temp); // write analog value to buffer
    fclose(aval0); // close buffer

    **batteryTempPtr1 = ((double) temp) / 100.0;
    **batteryTempPtr1 = **batteryTempPtr1 * 32 + 33;

    aval1 = fopen("/sys/devices/ocp.3/helper.15/AIN2", "r");
    fseek(aval1, 0, SEEK_SET); // go to beginning of buffer
    fscanf(aval1, "%d", &temp); // write analog value to buffer
    fclose(aval1); // close buffer

    fclose(ain);

    **batteryTempPtr2 = ((double) temp) / 100.0;
    **batteryTempPtr2 = **batteryTempPtr2 * 32 + 33;

    if(powerCount > 0) {
        if(**batteryTempPtr1 > *(batteryTemp1 + ((*batteryTempPtr1 - batteryTemp1) + 15) % 16) * 1.2){
            *batteryOverTemp = TRUE;
        }else if(**batteryTempPtr2 > *(batteryTemp2 + ((*batteryTempPtr2 - batteryTemp2) + 15) % 16) * 1.2){
            *batteryOverTemp = TRUE;
        }else{
            *batteryOverTemp = FALSE;
        }
    }



#else
    **batteryTempPtr1 = 50;
    **batteryTempPtr2 = 50;
    *batteryOverTemp = FALSE;
#endif
}



void readBatteryLevel(PowerSubsystemData* data) {

    unsigned int** batteryLevPtr = data->batteryLevPtr;
    unsigned short* batteryLev = data->batteryLev;
    unsigned short* powerCon = data->powerCon;
    unsigned short* powerGen = data->powerGen;
    Bool* solarPanelState = data->solarPanelState;

    volatile int i, j;
    FILE *ain,*aval0,*aval1;
#ifdef BEAGLEBONE

    ain = fopen("/sys/devices/bone_capemgr.9/slots", "w");
    fseek(ain,0,SEEK_SET);
    fprintf(ain,"cape-bone-iio");
    fflush(ain);

    usleep(600);

    aval0 = fopen("/sys/devices/ocp.3/helper.15/AIN0", "r");
    fseek(aval0, 0, SEEK_SET); // go to beginning of buffer
    fscanf(aval0, "%d", (*batteryLevPtr + powerCount % 16)); // write analog value to buffer
    fclose(aval0); // close buffer

    fclose(ain);

    printf("1 %d\n", *(*batteryLevPtr + powerCount % 16));

    *batteryLev = *(*batteryLevPtr + powerCount % 16) * 100 / 1800;
    *(*batteryLevPtr + powerCount % 16) *= 20;

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
    readBatteryTemp((PowerSubsystemData*) data);
    controlPower((PowerSubsystemData*) data);

    powerCount ++;


}




