#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>

int randomInteger(int low, int high);

int seed = 1;

const int TASK_NUM = 5;
const int MINOR_CYCLE_NUM_IN_MAJOR = 50;
const int TASK_WAITING_TIME_MICROS = 50000;
const int MAJOR_CYCLE_SEC = 1;

int taskCounter = 0;

enum _Boolean { FALSE = 0, TRUE = 1 };

typedef enum _Boolean Bool;

// thruster control
unsigned int thrusterComm = 0;

// power management
unsigned int* batteryLevPtr = buf[16];
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

// power subsystem function
void powerSubsystem(void* data);

// solar panel control function
void solarPanelControl(void* data);

// keyboard console function
void keyBoardConsole(void* data);

// thruster subsystem function
void thrusterSubsystem(void* data);

// satellite communication function
void satelliteComs(void* data);

// vehicle communication function
void vehicleComms(void* data);

// console display function
void consoleDisplay(void* data);

// warning alarm function
void warningAlarm(void* data);

// time pause function
void pauseSec(int sec);

//
struct PowerSubsystemDataStruct {
    Bool* solarPanelState;
    Bool* solarPanelDeploy;
    Bool* solarPanelRetract;
    unsigned int* batteryLevPtr;
    unsigned short* batteryLev;
    unsigned short* powerCon;
    unsigned short* powerGen;
};
typedef struct PowerSubsystemDataStruct PowerSubsystemData;

//
struct SolarPanelControlDataStruct {
    Bool* solarPanelState;
    Bool* solarPanelDeploy;
    Bool* solarPanelRetract;
    Bool* dmsInc;
    Bool* dmsDec;
};
typedef struct SolarPanelControlDataStruct SolarPanelControlData;


//
struct KeyBoardConsoleDataStruct {
    Bool* dmsInc;
    Bool* dmsDec;
};
typedef struct KeyBoardConsoleDataStruct KeyBoardConsoleData;

//
struct ThrusterSubsystemDataStruct {
    unsigned int* thrusterComm;
    unsigned short* fuelLev;
};
typedef struct ThrusterSubsystemDataStruct ThrusterSubsystemData;

//
struct SatelliteComsDataStruct {
    Bool* fuelLow;
    Bool* batteryLow;
    Bool* solarPanelState;
    unsigned short* batteryLev;
    unsigned short* fuelLev;
    unsigned short* powerCon;
    unsigned short* powerGen;
    unsigned int* thrusterComm;
};
typedef struct SatelliteComsDataStruct SatelliteComsData;

//
struct VehicleCommsDataStruct {
    char* command;
    char* response;
};
typedef struct VehicleCommsDataStruct VehicleCommsData;

//
struct ConsoleDisplayDataStruct {
    Bool* fuelLow;
    Bool* batteryLow;
    Bool* solarPanelState;
    unsigned short* batteryLev;
    unsigned short* fuelLev;
    unsigned short* powerCon;
    unsigned short* powerGen;
};
typedef struct ConsoleDisplayDataStruct ConsoleDisplayData;

//
struct WarningAlarmDataStruct {
    Bool* fuelLow;
    Bool* batteryLow;
    unsigned short* batteryLev;
    unsigned short* fuelLev;
};
typedef struct WarningAlarmDataStruct WarningAlarmData;

// Declare a TCB structure
typedef struct {
    void(*taskPtr)(void*);
    void* taskDataPtr;
    struct TCB* next;
    struct TCB* prev;
}
TCB;

// main
int main(void) {
    
    // Declare some TCBs
    TCB powerSubsystemTask;
    TCB solarPanelControlTask;
    TCB keyBoardConsoleTask;
    TCB thursterSubsystemTask;
    TCB satelliteComsTask;
    TCB vehicleCommsTask;
    TCB consoleDisplayTask;
    TCB warningAlarmTask;
    TCB* head;
    
    // PowerSubsystemData
    PowerSubsystemData powerSubsystemData;
    powerSubsystemData.solarPanelState = &solarPanelState;
    powerSubsystemData.batteryLev = &batteryLev;
    powerSubsystemData.powerCon = &powerCon;
    powerSubsystemData.powerGen = &powerGen;
    
    powerSubsystemTask.taskDataPtr = (void*)&powerSubsystemData;
    powerSubsystemTask.taskPtr = powerSubsystem;
    
    // SolarPanelControlData
    SolarPanelControlData solarPanelControlData;
    solarPanelControlData.solarPanelState = &solarPanelState;
    solarPanelControlData.solarPanelDeploy = &solarPanelDeploy;
    solarPanelControlData.solarPanelRetract = &solarPanelRetract;
    solarPanelControlData.dmsInc = &dmsInc;
    solarPanelControlData.dmsDec = &dmsDec;
    
    solarPanelControlTask.taskDataPtr = (void*)SolarPanelControlData;
    solarPanelControlTask.taskPtr = solarPanelControl;
    
    // keyBoardConsoleData
    KeyBoardConsoleData keyBoardConsoleData;
    keyBoardConsoleData.dmsInc = &dmsInc;
    keyBoardConsoleData.dmsDec = &dmsDec;
    
    keyBoardConsoleTask.taskDataPtr = (void*)KeyBoardConsoleData;
    keyBoardConsoleTask.taskPtr = keyBoardConsole;
    
    // ThrusterSubsystemData
    ThrusterSubsystemData thrusterSubsystemData;
    thrusterSubsystemData.thrusterComm = &thrusterComm;
    thrusterSubsystemData.fuelLev = &fuelLev;
    
    thursterSubsystemTask.taskDataPtr = (void*)&thrusterSubsystemData;
    thursterSubsystemTask.taskPtr = thrusterSubsystem;
    
    // SatelliteComsData
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
    
    // VehicleCommsData
    VehicleCommsData vehicleCommsData;
    vehicleCommsData.command = &command;
    vehicleCommsData.response = &response;
    
    vehicleCommsTask.taskDataPtr = (void*)&vehicleCommsData;
    vehicleCommsTask.taskPtr = vehicleComms;
    
    // ConsoleDisplayData
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
    
    // WarningAlarmData
    WarningAlarmData warningAlarmData;
    warningAlarmData.fuelLow = &fuelLow;
    warningAlarmData.batteryLow = &batteryLow;
    warningAlarmData.batteryLev = &batteryLev;
    warningAlarmData.fuelLev = &fuelLev;
    
    warningAlarmTask.taskDataPtr = (void*)&warningAlarmData;
    warningAlarmTask.taskPtr = warningAlarm;
    
    // Initialize the task queue
    queue[0] = &powerSubsystemTask;
    queue[1] = &solarPanelControlTask;
    queue[2] = &keyBoardConsoleTask;
    queue[3] = &thursterSubsystemTask;
    queue[4] = &satelliteComsTask;
    queue[5] = &vehicleCommsTask;
    queue[6] = &consoleDisplayTask;
    queue[7] = &warningAlarmTask;
    
    
    
    // schedule and dispatch the tasks
    
    volatile int i = 0;
    time_t lastTime = time(NULL);
    
    Bool majorCycle = FALSE;
    
    while (1) {
        tcbPtr = queue[i];
        tcbPtr->taskPtr((tcbPtr->taskDataPtr));
        
        //usleep(TASK_WAITING_TIME_MICROS);
        printf("%d %d\n", taskCounter, i);
        if (i >= TASK_NUM - 1)
            taskCounter ++;
        i = (i + 1) % TASK_NUM;
    }
    return EXIT_SUCCESS;
}

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

// solar panel control function
void solarPanelControl(void* data) {
    Bool solarPanelState = *((SolarPanelControlData*)data)->solarPanelState;
    Bool solarPanelDeploy = *((SolarPanelControlData*)data)->solarPanelDeploy;
    Bool solarPanelRetract = *((SolarPanelControlData*)data)->solarPanelRetract;
    Bool dmsInc = *((SolarPanelControlData*)data)->dmsInc;
    Bool dmsDec = *((SolarPanelControlData*)data)->dmsDec;
    
}

// keyboard console function
void keyBoardConsole(void* data) {
    Bool dmsInc = *((KeyBoardConsoleData*)data)->dmsInc;
    Bool dmsDec = *((KeyBoardConsoleData*)data)->dmsDec;
    
}

// thruster subsystem function
void thrusterSubsystem(void* data) {
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;
    
    unsigned short left = 0;
    unsigned short right = 0;
    unsigned short up = 0;
    unsigned short down = 0;
    
    unsigned int thrusterComm = *((ThrusterSubsystemData*)data)->thrusterComm;
    unsigned short fuelLev = *((ThrusterSubsystemData*)data)->fuelLev;
    
    unsigned int duration = thrusterComm >> 8;
    unsigned int magnititude = (thrusterComm & 0xFF) >> 4;
    unsigned int thruster =  thrusterComm & 0xF;
    
    //printf("command: %d", *((ThrusterSubsystemData*)data)->thrusterComm);
    //printf("duration: %d, mag: %d, thruster: %d\n", duration, magnititude, thruster);
    
    left = thruster & 0x1;
    right = (thruster >> 1) & 0x1;
    up = (thruster >> 2) & 0x1;
    down = (thruster >> 3) & 0x1;
    
    // send thruster signal
    if (duration > 0) {
        // we decide fuel level formulate to be
        if (*((ThrusterSubsystemData*)data)->fuelLev >= magnititude / 3) {
            *((ThrusterSubsystemData*)data)->fuelLev -= magnititude / 3;
        } else {
            *((ThrusterSubsystemData*)data)->fuelLev = 0;
        }
        if (duration >= 5)
            duration -= 5;
        else
            duration = 0;
    }
    
    *((ThrusterSubsystemData*)data)->thrusterComm = (duration << 8) | (magnititude << 4) | thruster;
    //printf("command: %d", *((ThrusterSubsystemData*)data)->thrusterComm);
}

// satellite communication function
void satelliteComs(void* data) {
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;
    
    // error structure name error
    *((SatelliteComsData*)data)->thrusterComm = randomInteger(0, 0xFFFF);
    //printf("command: %d", thrusterComm);
    
    Bool fuelLow = *((SatelliteComsData*)data)->fuelLow;
    Bool batteryLow = *((SatelliteComsData*)data)->batteryLow;
    Bool solarPanelState = *((SatelliteComsData*)data)->solarPanelState;
    unsigned short batteryLev = *((SatelliteComsData*)data)->batteryLev;
    unsigned short fuelLev = *((SatelliteComsData*)data)->fuelLev;
    unsigned short powerCon = *((SatelliteComsData*)data)->powerCon;
    unsigned short powerGen = *((SatelliteComsData*)data)->powerGen;
    
    // encode the status information from satellite to earth
    unsigned int result = (fuelLow << 3) | (batteryLow << 2) | (solarPanelState << 1)
    | (powerCon << 4) | (batteryLev << 8)
    | (fuelLev << 16) | (powerGen << 24);
    
}

// vehicle communication function
void vehicleComms(void* data) {
    char command = *((VehicleCommsData*)data)->command;
    char response = *((VehicleCommsData*)data)->response;
    
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;
    
    
}

// console display function
void consoleDisplay(void* data) {
    
    static int fdr = -1;
    static int fdw = -1;
    static int mode = 0;
    
    if (fdr == -1)
        fdr = open("/dev/ttys001", O_RDONLY | O_NONBLOCK);
    if (fdw == -1)
        fdw = open("/dev/ttys001", O_WRONLY);
    
    
    char userInput[1];
    ssize_t result = read(fdr, userInput, 1);
    if (result > 0) {
        if (userInput[0] == 't') {
            mode = 1 - mode;
        }
    }
    
    if (*((ConsoleDisplayData*)data)->batteryLev < 15)
        *((ConsoleDisplayData*)data)->batteryLow = TRUE;
    else
        *((ConsoleDisplayData*)data)->batteryLow = FALSE;
    if (*((ConsoleDisplayData*)data)->fuelLev < 15)
        *((ConsoleDisplayData*)data)->fuelLow = TRUE;
    else
        *((ConsoleDisplayData*)data)->fuelLow = FALSE;
    
    if (mode == 0) {
        dprintf(fdw, "Solar Panel State: %d\n", *((ConsoleDisplayData*)data)->solarPanelState);
        dprintf(fdw, "Battery Level: %d\n", *((ConsoleDisplayData*)data)->batteryLev);
        dprintf(fdw, "Fuel Level: %d\n", *((ConsoleDisplayData*)data)->fuelLev);
        dprintf(fdw, "Power consumption: %d\n", *((ConsoleDisplayData*)data)->powerCon);
    } else {
        if (*((ConsoleDisplayData*)data)->batteryLow) {
            dprintf(fdw, "Battery Level is Low\n");
        } else {
            dprintf(fdw, "Battery Level is Normal\n");
        }
        if (*((ConsoleDisplayData*)data)->fuelLow) {
            dprintf(fdw, "Fuel Level is Low\n");
        }else {
            dprintf(fdw, "Fuel Level is Normal\n");
        }
    }
}

#define DEBUG

#ifndef DEBUG
#define OUTPUT_LED1 "/sys/class/leds/beaglebone:green:usr1/brightness"
#else
#define OUTPUT_LED1 "led1.txt"
#endif

#ifndef DEBUG
#define OUTPUT_LED2 "/sys/class/leds/beaglebone:green:usr2/brightness"
#else
#define OUTPUT_LED2 "led2.txt"
#endif

#ifndef DEBUG
#define OUTPUT_LED3 "/sys/class/leds/beaglebone:green:usr3/brightness"
#else
#define OUTPUT_LED3 "led3.txt"
#endif
// warning alarm function
void warningAlarm(void* data) {
    static time_t start = 0;
    if (start == 0)
        start = time(NULL);
    //(WarningAlarmData*)data
    int flag = 0;
    
    FILE *led;
    
    int batterLev = *((WarningAlarmData*)data)->batteryLev;
    int fuelLev = *((WarningAlarmData*)data)->fuelLev;
    
    time_t t = time(NULL);
    t = t - start;
    
    if (t % 2) {
        if (batteryLev <= 10) {
            flag |= 0x2;
        }
        
        if (fuelLev <= 10) {
            flag |= 0x1;
        }
    }
    
    if ((t / 2) % 2) {
        if (batteryLev <= 50 && batteryLev > 10) {
            flag |= 0x2;
        }
        
        if (fuelLev <= 50 && fuelLev > 10) {
            flag |= 0x1;
        }
    }
    
    if (batteryLev > 20 && fuelLev > 20) {
        flag |= 0x4;
    }
    
    // error: LED is always on
    if (flag & 0x1) {
        led = fopen(OUTPUT_LED1, "w");
        fprintf(led, "%d", 1);
        fflush(led);
        fclose(led);
    } else {
        led = fopen(OUTPUT_LED1, "w");
        fprintf(led, "%d", 0);
        fflush(led);
        fclose(led);
    }
    
    if (flag & 0x2) {
        led = fopen(OUTPUT_LED2, "w");
        fprintf(led, "%d", 1);
        fflush(led);
        fclose(led);
    } else {
        led = fopen(OUTPUT_LED2, "w");
        fprintf(led, "%d", 0);
        fflush(led);
        fclose(led);
    }
    
    if (flag & 0x4) {
        led = fopen(OUTPUT_LED3, "w");
        fprintf(led, "%d", 1);
        fflush(led);
        fclose(led);
    } else {
        led = fopen(OUTPUT_LED3, "w");
        fprintf(led, "%d", 0);
        fflush(led);
        fclose(led);
    }
}

int randomInteger(int low, int high)
{
    double randNum = 0.0;
    int multiplier = 2743;
    int addOn = 5923;
    double max = INT_MAX + 1.0;
    
    int retVal = 0;
    
    if (low > high)
        retVal = randomInteger(high, low);
    else
    {
        seed = seed*multiplier + addOn;
        randNum = seed;
        
        if (randNum <0)
        {
            randNum = randNum + max;
        }
        
        randNum = randNum/max;
        
        retVal =  ((int)((high-low+1)*randNum))+low;
    }
    
    return retVal;
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
    if(NULL = = head) { // If the head pointer is pointing to nothing
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

