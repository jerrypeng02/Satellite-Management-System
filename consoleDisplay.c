#include "consoleDisplay.h"
#include "satelliteComs.h"
#include "constant.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_SIZE 1024

// console display function
void consoleDisplay(void* data) {

    static int mode = 0;
    if (earthCommend == 't') {
        mode = 1 - mode;
    }

    if (*((ConsoleDisplayData*)data)->batteryLev < 15)
        *((ConsoleDisplayData*)data)->batteryLow = TRUE;
    else
        *((ConsoleDisplayData*)data)->batteryLow = FALSE;
    if (*((ConsoleDisplayData*)data)->fuelLev < 15)
        *((ConsoleDisplayData*)data)->fuelLow = TRUE;
    else
        *((ConsoleDisplayData*)data)->fuelLow = FALSE;

    char output[MAX_SIZE];
    if (mode == 0) {
        sprintf(output, "Solar Panel State: %d\n", *((ConsoleDisplayData*)data)->solarPanelState);
        sprintf(output, "Battery Level: %d\n", *((ConsoleDisplayData*)data)->batteryLev);
        sprintf(output, "Fuel Level: %d\n", *((ConsoleDisplayData*)data)->fuelLev);
        sprintf(output, "Power consumption: %d\n", *((ConsoleDisplayData*)data)->powerCon);
    } else {
        if (*((ConsoleDisplayData*)data)->batteryLow) {
            sprintf(output, "Battery Level is Low\n");
        } else {
            sprintf(output, "Battery Level is Normal\n");
        }
        if (*((ConsoleDisplayData*)data)->fuelLow) {
            sprintf(output, "Fuel Level is Low\n");
        }else {
            sprintf(output, "Fuel Level is Normal\n");
        }
    }
    earthOutput(output);
}
