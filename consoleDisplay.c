//
//  consoleDisplay.c
//  
//
//  Created by Ningyang Peng on 7/25/17.
//
//

#include "consoleDisplay.h"
#include "constant.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

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
