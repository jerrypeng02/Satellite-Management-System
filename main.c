#include "constant.h"
#include "consoleDisplay.h"
#include "keyBoardConsole.h"
#include "powerSubsystem.h"
#include "imageCapture.h"
#include "transportDistance.h"
#include "satelliteComs.h"
#include "solarPanelControl.h"
#include "thrusterSubsystem.h"
#include "commandManagement.h"
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
const unsigned long MINOR_CYCLE_NUM_IN_MAJOR = 250;
const unsigned long MINOR_CYCLE_MS = 20;
const unsigned long MAJOR_CYCLE_SEC = 5;

void delay_ms(int);
unsigned long taskCounter = 0;
time_t lastTime;

/*************Interrupt service routines********************/
void isr0(void);
void isr1(void);
void isr2(void);
void isr3(void);

typedef void(*isr)(void);

void myHandler(int aSig);

unsigned int isrNum = 0; // isr selecter .. four used
void (*isrPtr)(); // isr pointer
isr isrArray[4]; // queue of isr handlers

/*************solarPanelControlVariable********************/


/*************PWM********************/
const int HEADER = 9;
const int PIN = 14;
const int PERIOD = 50000000;


/*************Global Variable********************/
// thruster control
unsigned int thrusterComm = 0;

// power management
unsigned int batteryLevel[16] = {0};
unsigned int* batteryLevPtr = NULL;

double batteryTemp1[16] = {0};
double* batteryTempPtr1 = NULL;

double batteryTemp2[16] = {0};
double* batteryTempPtr2 = NULL;

Bool batteryOverTemp = FALSE;
unsigned short batteryLev = 0;
unsigned short fuelLev = 100;
unsigned short powerCon = 0;
unsigned short powerGen = 0;

// image capture
unsigned int* imageFrequencyPtr = NULL;

Bool solarPanelState = FALSE;
Bool solarPanelDeploy = FALSE;
Bool solarPanelRetract = FALSE;

// vehicle communications
char vehicleCommand = NULL;
char response = NULL;

// solar panel control
Bool dmsInc = FALSE;
Bool dmsDec = FALSE;
unsigned short motorDriveSpeed = 20;

// warning alarm
Bool fuelLow = FALSE;
Bool batteryLow = FALSE;

// console display
Bool display = TRUE;

// transport distance
unsigned long transportDis = 0;

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

TCB commandManagementTask;
CommandManagementData commandManagementData;

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

void myHandler(int aSig)
{
    printf("in the handler isrnum %d\n", isrNum);
    isrArray[isrNum]();
    return;
}

/********** startup Function ********/
void startup() {

#ifdef BEAGLEBONE
    enableGPIOforTransport();
    enableGPIOforWarning();
    enablePWMforSolarPanelControl();
    enableADCforPowerSystem();
    usleep(600);
#endif

    taskCounter = 0;
    lastTime = time(NULL);

    // interupt services
    isrArray[0]= isr0;
    isrArray[1]= isr1;
    isrArray[2]= isr2;
    isrArray[3]= isr3;

    signal(SIGUSR1, myHandler);

    // thruster control
    thrusterComm = 0;

    // power management
    batteryLevPtr = batteryLevel;
    batteryTempPtr1 = batteryTemp1;
    batteryTempPtr2 = batteryTemp2;
    batteryOverTemp = FALSE;
    fuelLev = 100;
    powerCon = 0;
    powerGen = 0;

    solarPanelState = FALSE;
    solarPanelDeploy = FALSE;
    solarPanelRetract = FALSE;
    
    // image capture
    imageFrequencyPtr = NULL;

    // vehicle communications
    vehicleCommand = NULL;
    response = NULL;

    // solar panel control
    dmsInc = FALSE;
    dmsDec = FALSE;
    motorDriveSpeed = 20;

    // warning alarm
    fuelLow = FALSE;
    batteryLow = FALSE;

    // display
    display = TRUE;

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
    powerSubsystemData.batteryTempPtr1 = &batteryTempPtr1;
    powerSubsystemData.batteryTempPtr2 = &batteryTempPtr2;
    powerSubsystemData.batteryOverTemp = &batteryOverTemp;

    powerSubsystemTask.taskDataPtr = (void*)&powerSubsystemData;
    powerSubsystemTask.taskPtr = powerSubsystem;
    powerSubsystemTask.priority = 10;
    powerSubsystemTask.next = NULL;
    powerSubsystemTask.prev = NULL;
    insert(&powerSubsystemTask);


    // SolarPanelControlData
    solarPanelControlData.solarPanelState = &solarPanelState;
    solarPanelControlData.solarPanelDeploy = &solarPanelDeploy;
    solarPanelControlData.solarPanelRetract = &solarPanelRetract;
    solarPanelControlData.dmsInc = &dmsInc;
    solarPanelControlData.dmsDec = &dmsDec;
    solarPanelControlData.motorDriveSpeed = &motorDriveSpeed;

    solarPanelControlTask.taskDataPtr = (void*)&solarPanelControlData;
    solarPanelControlTask.taskPtr = solarPanelControl;
    solarPanelControlTask.priority = 8;
    solarPanelControlTask.next = NULL;
    solarPanelControlTask.prev = NULL;
    insert(&solarPanelControlTask);

    // keyBoardConsoleData
    keyBoardConsoleData.dmsInc = &dmsInc;
    keyBoardConsoleData.dmsDec = &dmsDec;
    keyBoardConsoleData.solarPanelDeploy = &solarPanelDeploy;
    keyBoardConsoleData.solarPanelRetract = &solarPanelRetract;
    keyBoardConsoleData.motorDriveSpeed = &motorDriveSpeed;

    keyBoardConsoleTask.taskDataPtr = (void*)&keyBoardConsoleData;
    keyBoardConsoleTask.taskPtr = keyBoardConsole;
    keyBoardConsoleTask.priority = 9;
    keyBoardConsoleTask.next = NULL;
    keyBoardConsoleTask.prev = NULL;
    //insert(&keyBoardConsoleTask);

    // ThrusterSubsystemData
    thrusterSubsystemData.thrusterComm = &thrusterComm;
    thrusterSubsystemData.fuelLev = &fuelLev;

    thrusterSubsystemTask.taskDataPtr = (void*)&thrusterSubsystemData;
    thrusterSubsystemTask.taskPtr = thrusterSubsystem;
    thrusterSubsystemTask.priority = 7;
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
    satelliteComsTask.priority = 11;
    satelliteComsTask.next = NULL;
    satelliteComsTask.prev = NULL;
    insert(&satelliteComsTask);

    commandManagementData.display = &display;
    commandManagementData.thrusterComm = &thrusterComm;
    commandManagementData.vehicleCommand = &vehicleCommand;
    commandManagementData.motorDriveSpeed = &motorDriveSpeed;

    commandManagementTask.taskDataPtr = (void*)&commandManagementData;
    commandManagementTask.taskPtr = commandManagement;
    commandManagementTask.priority = 6;
    commandManagementTask.next = NULL;
    commandManagementTask.prev = NULL;
    insert(&commandManagementTask);

    // VehicleCommsData
    vehicleCommsData.vehicleCommand = &vehicleCommand;
    vehicleCommsData.response = &response;

    vehicleCommsTask.taskDataPtr = (void*)&vehicleCommsData;
    vehicleCommsTask.taskPtr = vehicleComms;
    vehicleCommsTask.priority = 5;
    vehicleCommsTask.next = NULL;
    vehicleCommsTask.prev = NULL;
    insert(&vehicleCommsTask);
    
    // TransportDistanceData
    transportDistanceData.transportDis = &transportDis;
    
    transportDistanceTask.taskDataPtr = (void*)&transportDistanceData;
    transportDistanceTask.taskPtr = transportDistance;
    transportDistanceTask.priority = 4;
    transportDistanceTask.next = NULL;
    transportDistanceTask.prev = NULL;
    insert(&transportDistanceTask);
    
    // ImageCaptureData
    imageCaptureData.imageFrequencyPtr = &imageFrequencyPtr;
    
    imageCaptureTask.taskDataPtr = (void*)&imageCaptureData;
    imageCaptureTask.taskPtr = imageCapture;
    imageCaptureTask.priority = 3;
    imageCaptureTask.next = NULL;
    imageCaptureTask.prev = NULL;
    insert(&imageCaptureTask);

    // ConsoleDisplayData
    consoleDisplayData.display = &display;
    consoleDisplayData.fuelLow = &fuelLow;
    consoleDisplayData.batteryLow = &batteryLow;
    consoleDisplayData.solarPanelState = &solarPanelState;
    consoleDisplayData.solarPanelDeploy = &solarPanelDeploy;
    consoleDisplayData.solarPanelRetract = &solarPanelRetract;
    consoleDisplayData.batteryLev = &batteryLev;
    consoleDisplayData.fuelLev = &fuelLev;
    consoleDisplayData.powerCon = &powerCon;
    consoleDisplayData.powerGen = &powerGen;
    consoleDisplayData.transportDis = &transportDis;
    consoleDisplayData.imageFrequencyPtr = &imageFrequencyPtr;
    consoleDisplayData.batteryTempPtr1 = &batteryTempPtr1;
    consoleDisplayData.batteryTempPtr2 = &batteryTempPtr2;
    consoleDisplayData.batteryOverTemp = &batteryOverTemp;

    consoleDisplayTask.taskDataPtr = (void*)&consoleDisplayData;
    consoleDisplayTask.taskPtr = consoleDisplay;
    consoleDisplayTask.priority = 2;
    consoleDisplayTask.next = NULL;
    consoleDisplayTask.prev = NULL;
    insert(&consoleDisplayTask);

    // WarningAlarmData
    warningAlarmData.fuelLow = &fuelLow;
    warningAlarmData.batteryLow = &batteryLow;
    warningAlarmData.batteryLev = &batteryLev;
    warningAlarmData.batteryOverTemp = &batteryOverTemp;
    warningAlarmData.fuelLev = &fuelLev;

    warningAlarmTask.taskDataPtr = (void*)&warningAlarmData;
    warningAlarmTask.taskPtr = warningAlarm;
    warningAlarmTask.priority = 1;
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
        while (tcbPtr != NULL) {
            //printf("%d\n", tcbPtr->priority);
            tcbPtr->taskPtr((tcbPtr->taskDataPtr));
            tcbPtr = tcbPtr -> next;
        }
        //printf("\n%lu\n", taskCounter);
        delay_ms(MINOR_CYCLE_MS);
        taskCounter ++;
    }
    return EXIT_SUCCESS;
}

Bool inQueue(TCB* node) {
    TCB* current = head;
    while (current != NULL) {
        if (current == node)
            return TRUE;
        current = current->next;
    }
    return FALSE;
}

// Insert function
// Arguments: Pointer to TCB node
// Returns: void
// Function: Adds the TCB node and sort queue by priority
void insert(TCB* node) {
    if (inQueue(node))
        return;

    TCB* current = tail;
    while (current != NULL && current->priority < node->priority) {
        current = current->prev;
    }

    if (current != NULL) {
        node->prev = current;
        if (current->next != NULL) {
            node->next = current->next;
            current->next->prev = node;
        }
        current->next = node;
    } else {
        node->prev = NULL;
        node->next = head;
        if (head != NULL)
            head->prev = node;
        head = node;
    }

    if(tail == current) {
        tail = node;
        node->next = NULL;
    }
    return;
}

// Delete function
// Arguments: Pointer to TCB node
void delete(TCB* node) {
    if (!inQueue(node))
        return;

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

// lift off
void isr0() {
    insert(&transportDistanceTask);
}

// dock
void isr1() {
    delete(&transportDistanceTask);
}

// image scan
void isr2() {
    insert(&solarPanelControlTask);
    insert(&keyBoardConsoleTask);
}

// send image data
void isr3() {
    delete(&solarPanelControlTask);
    delete(&keyBoardConsoleTask);
}
