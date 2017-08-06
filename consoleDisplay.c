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
    //printf("%c", earthCommand);
    if (earthCommand == 't') {
        mode = 1 - mode;
        earthCommand = 0;
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
        sprintf(output, "Solar panel state: %d\n"
                        "Battery level: %d\n"
                        "Fuel level: %d\n"
                        "Power consumption: %d\n"
                        "Battery temperature: %d\n"
                        "Transport distance: %d\n",
                *((ConsoleDisplayData*)data)->solarPanelState,
                *((ConsoleDisplayData*)data)->batteryLev,
                *((ConsoleDisplayData*)data)->fuelLev,
                *((ConsoleDisplayData*)data)->powerCon
                *((ConsoleDisplayData*)data)->batteryTemp,
                *((ConsoleDisplayData*)data)->transportDis);
    } else {
        sprintf(output, "Battery level is %s\n"
                        "Fuel level is %s\n",
                *((ConsoleDisplayData*)data)->batteryLow? "low" : "normal",
                *((ConsoleDisplayData*)data)->fuelLow? "low" : "normal");
    }
    earthOutput(output);
}
