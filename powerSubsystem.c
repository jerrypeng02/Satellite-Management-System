#include "powerSubsystem.h"
#include "constant.h"
#include "stdio.h"

unsigned int powerCount = 0;
Bool powerIncrease = TRUE;
// power subsystem function
void powerSubsystem(void* data) {

    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;

    Bool* solarPanelState = ((PowerSubsystemData*)data)->solarPanelState;
    Bool* solarPanelDeploy = ((PowerSubsystemData*)data)->solarPanelDeploy;
    Bool* solarPanelRetract = ((PowerSubsystemData*)data)->solarPanelRetract;
    unsigned int** batteryLevPtr = ((PowerSubsystemData*)data)->batteryLevPtr;
    unsigned short* powerCon = ((PowerSubsystemData*)data)->powerCon;
    unsigned short* powerGen = ((PowerSubsystemData*)data)->powerGen;

    unsigned short motorDrive = 0;

    if(powerIncrease) {
        if (*((PowerSubsystemData*)data)->powerCon <= 10){
            if (powerCount % 2 == 0) {
                *((PowerSubsystemData*)data)->powerCon += 2;
            } else {
                *((PowerSubsystemData*)data)->powerCon -= 1;
            }
        }
        if (*((PowerSubsystemData*)data)->powerCon > 10) {
            powerIncrease = FALSE;
        }
    } else {
        if (*((PowerSubsystemData*)data)->powerCon >= 5){
            if (powerCount % 2 == 0) {
                *((PowerSubsystemData*)data)->powerCon -= 2;
            } else {
                *((PowerSubsystemData*)data)->powerCon += 1;
            }
        }
        if (*((PowerSubsystemData*)data)->powerCon < 5) {
            powerIncrease = TRUE;
        }
    }

    if(solarPanelState){
        if(*((PowerSubsystemData*)data)->batteryLev > 95){
            *((PowerSubsystemData*)data)->solarPanelState = FALSE;
            *((PowerSubsystemData*)data)->powerGen = 0;
        }else{
            if (*((PowerSubsystemData*)data)->batteryLev <= 50){
                if (powerCount % 2 == 0){
                    *((PowerSubsystemData*)data)->powerGen += 2;
                } else {
                    *((PowerSubsystemData*)data)->powerGen += 1;
                }
            } else {
                if (powerCount % 2 == 0){
                    *((PowerSubsystemData*)data)->powerGen += 2;
                }
            }
        }
    }
/*
    if(!*((PowerSubsystemData*)data)->solarPanelState){
        if (*((PowerSubsystemData*)data)->batteryLev < *((PowerSubsystemData*)data)->powerCon * 3) {
            *((PowerSubsystemData*)data)->batteryLev = 0;
        } else {
            // we change the battery level formulate to batterylev = batterylev - powerCon
            *((PowerSubsystemData*)data)->batteryLev -= *((PowerSubsystemData*)data)->powerCon * 3;
        }
    }else{
        if (*((PowerSubsystemData*)data)->batteryLev < ((int) *((PowerSubsystemData*)data)->powerCon) - ((int)*((PowerSubsystemData*)data)->powerGen)) {
            *((PowerSubsystemData*)data)->batteryLev = 0;
        } else {
            *((PowerSubsystemData*)data)->batteryLev -= ((int) *((PowerSubsystemData*)data)->powerCon) - ((int)*((PowerSubsystemData*)data)->powerGen);
        }

        if (*((PowerSubsystemData*)data)->batteryLev > 100)
            *((PowerSubsystemData*)data)->batteryLev = 100;
    }
*/
    // we change the condition of opening solar panel to when the battery level is less than 40
    if (!solarPanelState && *((PowerSubsystemData*)data)->batteryLev <= 36) {
        *((PowerSubsystemData*)data)->solarPanelState = TRUE;
    }

    FILE *ain,*aval0,*aval1; // create some buffers


    int i,j; // working variables
    //int batteryLev;
    // enable the ADC ports
    ain = fopen("/sys/devices/bone_capemgr.9/slots", "w");
    fseek(ain,0,SEEK_SET);
    fprintf(ain,"cape-bone-iio");
    fflush(ain);
    while(1){

        for(i=0; i < 16; i++){

            aval0 = fopen("/sys/devices/ocp.3/helper.15/AIN0", "r");
            fseek(aval0,0,SEEK_SET); // go to beginning of buffer
            fscanf(aval0,"%d",&(*batteryLevPtr[i])); // write analog value to buffer
            fclose(aval0); // close buffer
            if(*batteryLevPtr[i] >1800){
                *batteryLevPtr[i] = 1800;
            }


            *batteryLevPtr[i] = *batteryLevPtr[i] * 20;



            // delay
            for(j = 0; j<1000000;j++);

        }
        // enable the ADC ports

        //enable the ADC ports
        //aval1 = fopen("/sys/devices/ocp.3/helper.15/AIN1","r");
        //fseek(aval1,0,SEEK_SET); // go to beginning of buffer
        //fscanf(aval1,"%d",&value1); // write analog value to buffer
        //fclose(aval1); // close buffer
        // display readings
        //printf("value0: %d value1: %d\n",value0,value1);
        // delay
        //for(i = 0; i<1000000;i++);

    }

    fclose(ain);




    powerCount ++;
}
