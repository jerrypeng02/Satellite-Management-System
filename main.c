#include "constant.h"
#include "consoleDisplay.h"
#include "keyBoardConsole.h"
#include "powerSubsystem.h"
#include "satelliteComs.h"
#include "solarPanelControl.h"
#include "thrusterSubsystem.h"
#include "vehicleComms.h"
#include "warningAlarm.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>


const unsigned long MINOR_CYCLE_NUM_IN_MAJOR = 50;
const unsigned long MINOR_CYCLE_MICROS = 20000;
const unsigned long MAJOR_CYCLE_SEC = 5;

// Declare a TCB structure
struct TaskControlBlock {
    void(*taskPtr)(void*);
    void* taskDataPtr;
    struct TaskControlBlock* next;
    struct TaskControlBlock* prev;
};
typedef struct TaskControlBlock TCB;

void insert(TCB* node);

unsigned long taskCounter = 0;

// thruster control
unsigned int thrusterComm = 0;

// power management
unsigned int* batteryLevPtr = 0;
//TODO: point to 16 reading buffer
unsigned short batteryLev = 100;
unsigned short fuelLev = 100;
unsigned short powerCon = 0;
unsigned short powerGen = 0;

Bool solarPanelState = FALSE;
Bool solarPanelDeploy = FALSE;
Bool solarPanelRetract = FALSE;

// vehivle communications
char command = NULL;
char response = NULL;

// solar panel control
Bool dmsInc = FALSE;
Bool dmsDec = FALSE;

// warning alarm
Bool fuelLow = FALSE;
Bool batteryLow = FALSE;

// time pause function
//void pauseSec(int sec);

TCB* head;
TCB* tail;

// main
int main(void) {
    
    // Declare some TCBs
    
    // PowerSubsystemData
    TCB powerSubsystemTask;
    PowerSubsystemData powerSubsystemData;
    powerSubsystemData.batteryLevPtr = &batteryLevPtr;
    powerSubsystemData.solarPanelState = &solarPanelState;
    powerSubsystemData.batteryLev = &batteryLev;
    powerSubsystemData.powerCon = &powerCon;
    powerSubsystemData.powerGen = &powerGen;
    
    powerSubsystemTask.taskDataPtr = (void*)&powerSubsystemData;
    powerSubsystemTask.taskPtr = powerSubsystem;
    insert(&powerSubsystemTask);
    
    
    // SolarPanelControlData
    TCB solarPanelControlTask;
    SolarPanelControlData solarPanelControlData;
    solarPanelControlData.solarPanelState = &solarPanelState;
    solarPanelControlData.solarPanelDeploy = &solarPanelDeploy;
    solarPanelControlData.solarPanelRetract = &solarPanelRetract;
    solarPanelControlData.dmsInc = &dmsInc;
    solarPanelControlData.dmsDec = &dmsDec;
    
    solarPanelControlTask.taskDataPtr = (void*)&solarPanelControlData;
    solarPanelControlTask.taskPtr = solarPanelControl;
    insert(&solarPanelControlTask);

    
    // keyBoardConsoleData
    TCB keyBoardConsoleTask;
    KeyBoardConsoleData keyBoardConsoleData;
    keyBoardConsoleData.dmsInc = &dmsInc;
    keyBoardConsoleData.dmsDec = &dmsDec;
    
    keyBoardConsoleTask.taskDataPtr = (void*)&keyBoardConsoleData;
    keyBoardConsoleTask.taskPtr = keyBoardConsole;
    insert(&keyBoardConsoleTask);
    
    // ThrusterSubsystemData
    TCB thrusterSubsystemTask;
    ThrusterSubsystemData thrusterSubsystemData;
    thrusterSubsystemData.thrusterComm = &thrusterComm;
    thrusterSubsystemData.fuelLev = &fuelLev;
    
    thrusterSubsystemTask.taskDataPtr = (void*)&thrusterSubsystemData;
    thrusterSubsystemTask.taskPtr = thrusterSubsystem;
    insert(&thrusterSubsystemTask);
    
    // SatelliteComsData
    TCB satelliteComsTask;
    SatelliteComsData satelliteComsData;
    satelliteComsData.fuelLow = &fuelLow;
    satelliteComsData.batteryLow = &batteryLow;
    satelliteComsData.solarPanelState = &solarPanelState;
    satelliteComsData.batteryLev = &batteryLev;
    satelliteComsData.fuelLev = &fuelLev;
    satelliteComsData.powerCon = &powerCon;
    satelliteComsData.powerGen = &powerGen;
    satelliteComsData.thrusterComm = &thrusterComm;
    
    satelliteComsTask.taskDataPtr = (void*)&satelliteComsData;
    satelliteComsTask.taskPtr = satelliteComs;
    insert(&satelliteComsTask);
    
    // VehicleCommsData
    TCB vehicleCommsTask;
    VehicleCommsData vehicleCommsData;
    vehicleCommsData.command = &command;
    vehicleCommsData.response = &response;
    
    vehicleCommsTask.taskDataPtr = (void*)&vehicleCommsData;
    vehicleCommsTask.taskPtr = vehicleComms;
    insert(&vehicleCommsTask);
    
    // ConsoleDisplayData
    TCB consoleDisplayTask;
    ConsoleDisplayData consoleDisplayData;
    consoleDisplayData.fuelLow = &fuelLow;
    consoleDisplayData.batteryLow = &batteryLow;
    consoleDisplayData.solarPanelState = &solarPanelState;
    consoleDisplayData.batteryLev = &batteryLev;
    consoleDisplayData.fuelLev = &fuelLev;
    consoleDisplayData.powerCon = &powerCon;
    consoleDisplayData.powerGen = &powerGen;
    
    consoleDisplayTask.taskDataPtr = (void*)&consoleDisplayData;
    consoleDisplayTask.taskPtr = consoleDisplay;
    insert(&consoleDisplayTask);
    
    // WarningAlarmData
    TCB warningAlarmTask;
    WarningAlarmData warningAlarmData;
    warningAlarmData.fuelLow = &fuelLow;
    warningAlarmData.batteryLow = &batteryLow;
    warningAlarmData.batteryLev = &batteryLev;
    warningAlarmData.fuelLev = &fuelLev;
    
    warningAlarmTask.taskDataPtr = (void*)&warningAlarmData;
    warningAlarmTask.taskPtr = warningAlarm;
    insert(&warningAlarmTask);
    
    // schedule and dispatch the tasks
    
    time_t lastTime = time(NULL);
    
    TCB* tcbPtr;
    
    while (1) {
        tcbPtr = head;
        while (tcbPtr != NULL) {
            tcbPtr->taskPtr((tcbPtr->taskDataPtr));
            tcbPtr = tcbPtr -> next;
        }
        printf("%lu\n", taskCounter);
        usleep(MINOR_CYCLE_MICROS);
        taskCounter ++;
    }
    return EXIT_SUCCESS;
}

// Insert function
// Arguments: Pointer to TCB node
// Returns: void
// Function: Adds the TCB node to the end of the list and updates head and tail pointers
// This function assumes that head and tail pointers have already been created
// and are global and that the pointers contained in the TCB node have already been initialized to NULL
// This function also assumes that the “previous” and “next” pointers in the TCB node are called “prev”
// and “next” respectively
void insert(TCB* node) {
    if(NULL == head) { // If the head pointer is pointing to nothing
        head = node; // set the head and tail pointers to point to this node
        tail = node;
    }
    else { // otherwise, head is not NULL, add the node to the end of the list
        tail -> next = node;
        node -> prev = tail; // note that the tail pointer is still pointing
        // to the prior last node at this point
        tail = node; // update the tail pointer
    }
    return;
}

//#include <stdio.h>
//#include <time.h>
//void pauseSec(int sec); // software timer
//int main()
//{
//    FILE *ain,*aval0,*aval1; // create some buffers
//    int value0,value1,i; // working variables
//    // enable the ADC ports
//    ain = fopen("/sys/devices/bone_capemgr.9/slots", "w");
//    fseek(ain,0,SEEK_SET);
//    fprintf(ain,"cape-bone-iio");
//    fflush(ain);
//    while(1)
//    {
//        // enable the ADC ports
//        aval = fopen("/sys/devices/ocp.3/helper.15/AIN0", "r");
//        fseek(aval0,0,SEEK_SET); // go to beginning of buffer
//        fscanf(aval0,"%d",&value0); // write analog value to buffer
//        fclose(aval0); // close buffer
//        // delay
//        for(i = 0; i<1000000;i++);
//        // enable the ADC ports
//        aval1 = fopen("/sys/devices/ocp.3/helper.15/AIN1","r");
//        fseek(aval1,0,SEEK_SET); // go to beginning of buffer
//        fscanf(aval1,"%d",&value1); // write analog value to buffer
//        fclose(aval1); // close buffer
//        // display readings
//        printf("value0: %d value1: %d\n",value0,value1);
//        // delay
//        for(i = 0; i<1000000;i++);
//    }
//    fclose(ain);
//    return 0;
//}
//
//// delay function
//void pauseSec(int sec)
//{
//    time_t now,later;
//    now = time(NULL);
//    later = time(NULL);
//    while((later - now) < (double) sec)
//        later = time(NULL);
//}

// Test Error we didn't run every task in major cycle
/*if (i == 0) {
 time_t currentTime = time(NULL);
 //printf("%ld\n", (currentTime - lastTime));
 if (currentTime - lastTime >= MAJOR_CYCLE_SEC) {
 majorCycle = TRUE;
 lastTime = currentTime;
 } else {
 majorCycle = FALSE;
 }
 }*/

