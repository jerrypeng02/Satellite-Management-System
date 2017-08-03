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

/*************TaskQueue related********************/
// Declare a TCB structure
struct TaskControlBlock {
    void(*taskPtr)(void*);
    void* taskDataPtr;
    int priority;
    struct TaskControlBlock* next;
    struct TaskControlBlock* prev;
};
typedef struct TaskControlBlock TCB;

void insert(TCB* node);
void delete(TCB* node);

TCB* head = NULL;
TCB* tail = NULL;

/*************Time related********************/
const unsigned long MINOR_CYCLE_NUM_IN_MAJOR = 50;
const unsigned long MINOR_CYCLE_MS = 20;
const unsigned long MAJOR_CYCLE_SEC = 5;

void delay_ms(int);
unsigned long taskCounter = 0;
time_t lastTime;

/*************solarPanelControlVariable********************/


/*************PWM********************/
const int HEADER = 9;
const int PIN = 14;
const int PERIOD = 500000000;


/*************Global Variable********************/
// thruster control
unsigned int thrusterComm = 0;


unsigned int buffer[16];
// power management
unsigned int* batteryLevPtr = 0;
//TODO: point to 16 reading buffer
unsigned short batteryLev = 100;
unsigned short fuelLev = 100;
unsigned short powerCon = 0;
unsigned short powerGen = 0;

// image capture
unsigned int* imageDataRawPtr = 0;
unsigned int* imageDataPtr = 0;

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


// Declare some TCBs
TCB powerSubsystemTask;
PowerSubsystemData powerSubsystemData;

TCB solarPanelControlTask;
SolarPanelControlData solarPanelControlData;

TCB keyBoardConsoleTask;
KeyBoardConsoleData keyBoardConsoleData;

TCB thrusterSubsystemTask;
ThrusterSubsystemData thrusterSubsystemData;

TCB satelliteComsTask;
SatelliteComsData satelliteComsData;

TCB vehicleCommsTask;
VehicleCommsData vehicleCommsData;

TCB transportDistanceTask;
TransportDistanceData transportDistanceData;

TCB imageCaptureTask;
ImageCaptureData imageCaptureData;

TCB consoleDisplayTask;
ConsoleDisplayData consoleDisplayData;

TCB warningAlarmTask;
WarningAlarmData warningAlarmData;

// time pause function
//void pauseSec(int sec);

/********** startup Function ********/
void startup() {
    taskCounter = 0;
    lastTime = time(NULL);

    // thruster control
    thrusterComm = 0;

    // power management
    batteryLevPtr = buffer;
    batteryLev = 100;
    fuelLev = 100;
    powerCon = 0;
    powerGen = 0;

    solarPanelState = FALSE;
    solarPanelDeploy = FALSE;
    solarPanelRetract = FALSE;
    
    // image capture
    imageDataRawPtr = 0;
    imageDataPtr = 0;

    // vehivle communications
    command = NULL;
    response = NULL;

    // solar panel control
    dmsInc = FALSE;
    dmsDec = FALSE;

    // warning alarm
    fuelLow = FALSE;
    batteryLow = FALSE;

    // clear TCB
    head = tail = NULL;

    // PowerSubsystemData
    powerSubsystemData.batteryLevPtr = &batteryLevPtr;
    powerSubsystemData.solarPanelDeploy = &solarPanelDeploy;
    powerSubsystemData.solarPanelRetract = &solarPanelRetract;
    powerSubsystemData.solarPanelState = &solarPanelState;
    powerSubsystemData.batteryLev = &batteryLev;
    powerSubsystemData.powerCon = &powerCon;
    powerSubsystemData.powerGen = &powerGen;

    powerSubsystemTask.taskDataPtr = (void*)&powerSubsystemData;
    powerSubsystemTask.taskPtr = powerSubsystem;
    powerSubsystemTask.next = NULL;
    powerSubsystemTask.prev = NULL;
    insert(&powerSubsystemTask);


    // SolarPanelControlData
    solarPanelControlData.solarPanelState = &solarPanelState;
    solarPanelControlData.solarPanelDeploy = &solarPanelDeploy;
    solarPanelControlData.solarPanelRetract = &solarPanelRetract;
    solarPanelControlData.dmsInc = &dmsInc;
    solarPanelControlData.dmsDec = &dmsDec;

    solarPanelControlTask.taskDataPtr = (void*)&solarPanelControlData;
    solarPanelControlTask.taskPtr = solarPanelControl;
    solarPanelControlTask.next = NULL;
    solarPanelControlTask.prev = NULL;
    insert(&solarPanelControlTask);


    // keyBoardConsoleData
    keyBoardConsoleData.dmsInc = &dmsInc;
    keyBoardConsoleData.dmsDec = &dmsDec;

    keyBoardConsoleTask.taskDataPtr = (void*)&keyBoardConsoleData;
    keyBoardConsoleTask.taskPtr = keyBoardConsole;
    keyBoardConsoleTask.next = NULL;
    keyBoardConsoleTask.prev = NULL;
    insert(&keyBoardConsoleTask);

    // ThrusterSubsystemData
    thrusterSubsystemData.thrusterComm = &thrusterComm;
    thrusterSubsystemData.fuelLev = &fuelLev;

    thrusterSubsystemTask.taskDataPtr = (void*)&thrusterSubsystemData;
    thrusterSubsystemTask.taskPtr = thrusterSubsystem;
    thrusterSubsystemTask.next = NULL;
    thrusterSubsystemTask.prev = NULL;
    insert(&thrusterSubsystemTask);

    // SatelliteComsData
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
    satelliteComsTask.next = NULL;
    satelliteComsTask.prev = NULL;
    insert(&satelliteComsTask);

    // VehicleCommsData
    vehicleCommsData.command = &command;
    vehicleCommsData.response = &response;

    vehicleCommsTask.taskDataPtr = (void*)&vehicleCommsData;
    vehicleCommsTask.taskPtr = vehicleComms;
    vehicleCommsTask.next = NULL;
    vehicleCommsTask.prev = NULL;
    insert(&vehicleCommsTask);
    
    // TransportDistanceData
    transportDistanceTask. = &;
    transportDistanceTask. = &;
    
    transportDistanceTask.taskDataPtr = (void*)&transportDistanceData;
    transportDistanceTask.taskPtr = transportDistance;
    transportDistanceTask.next = NULL;
    transportDistanceTask.prev = NULL;
    insert(&transportDistanceTask);
    
    // ImageCaptureData
    imageCaptureTask.command = &command;
    imageCaptureTask.response = &response;
    
    imageCaptureTask.taskDataPtr = (void*)&imageCaptureData;
    imageCaptureTask.taskPtr = imageCapture;
    imageCaptureTask.next = NULL;
    imageCaptureTask.prev = NULL;
    insert(&imageCaptureTask);

    // ConsoleDisplayData
    consoleDisplayData.fuelLow = &fuelLow;
    consoleDisplayData.batteryLow = &batteryLow;
    consoleDisplayData.solarPanelState = &solarPanelState;
    consoleDisplayData.batteryLev = &batteryLev;
    consoleDisplayData.fuelLev = &fuelLev;
    consoleDisplayData.powerCon = &powerCon;
    consoleDisplayData.powerGen = &powerGen;
    consoleDisplayData.batteryTemp = &batteryTemp;
    consoleDisplayData.transportDis = &transportDis;

    consoleDisplayTask.taskDataPtr = (void*)&consoleDisplayData;
    consoleDisplayTask.taskPtr = consoleDisplay;
    consoleDisplayTask.next = NULL;
    consoleDisplayTask.prev = NULL;
    insert(&consoleDisplayTask);

    // WarningAlarmData
    warningAlarmData.fuelLow = &fuelLow;
    warningAlarmData.batteryLow = &batteryLow;
    warningAlarmData.batteryLev = &batteryLev;
    warningAlarmData.fuelLev = &fuelLev;
    warningAlarmData.BOT = &BOT;

    warningAlarmTask.taskDataPtr = (void*)&warningAlarmData;
    warningAlarmTask.taskPtr = warningAlarm;
    warningAlarmTask.next = NULL;
    warningAlarmTask.prev = NULL;
    insert(&warningAlarmTask);
}

// main
int main(void) {
    // schedule and dispatch the tasks
    TCB* tcbPtr;

    startup();

    static int append = 0;

    while (1) {
        tcbPtr = head;
        int t = 0;
        if ((solarPanelState && !solarPanelDeploy) || 
                (!solarPanelState && !solarPanelRetract)) {
            if (append==1) {
                printf("Appending solar and keyboard\n");
                insert(&solarPanelControlTask);
                insert(&keyBoardConsoleTask);
                append = 0;
            }
        } else if (!append) { 
            printf("Removing solar and keyboard\n");
            remove(&solarPanelControlTask);
            remove(&keyBoardConsoleTask);
            append = 1;
        }  
        while (tcbPtr != NULL) {
            //printf("%d\n", t ++);
            tcbPtr->taskPtr((tcbPtr->taskDataPtr));
            tcbPtr = tcbPtr -> next;
        }
        //printf("\n%lu\n", taskCounter);
        delay_ms(MINOR_CYCLE_MS);
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

// Delete function
// Arguments: Pointer to TCB node
void delete(TCB* node) {
    /* If node to be deleted is head node */
    if(head == node)
        head = node->next;

    if(tail == node)
        tail = node->prev;

    /* Change next only if node to be deleted is NOT the last node */
    if(node->next != NULL)
        node->next->prev = node->prev;

    /* Change prev only if node to be deleted is NOT the first node */
    if(node->prev != NULL)
        node->prev->next = node->next;

    /* Finally, free the memory occupied by del*/
    //free(node);
    return;
}

void delay_ms(int time_in_ms) {
    usleep(time_in_ms * 1000);
}
