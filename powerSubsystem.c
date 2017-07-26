#include "powerSubsystem.h"
#include "constant.h"

unsigned int powerCount = 0;
Bool powerIncrease = TRUE;
// power subsystem function
void powerSubsystem(void* data) {
    
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;
    
    Bool solarPanelState = *((PowerSubsystemData*)data)->solarPanelState;
    Bool solarPanelDeploy = *((PowerSubsystemData*)data)->solarPanelDeploy;
    Bool solarPanelRetract = *((PowerSubsystemData*)data)->solarPanelRetract;
    unsigned int* batteryLevPtr = *((PowerSubsystemData*)data)->batteryLevPtr;
    unsigned short powerCon = *((PowerSubsystemData*)data)->powerCon;
    unsigned short powerGen = *((PowerSubsystemData*)data)->powerGen;
    
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
    
    // we change the condition of opening solar panel to when the battery level is less than 40
    if (!solarPanelState && *((PowerSubsystemData*)data)->batteryLev <= 36) {
        *((PowerSubsystemData*)data)->solarPanelState = TRUE;
    }
    
    powerCount ++;
}
