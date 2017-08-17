#include "consoleDisplay.h"
#include "satelliteComs.h"
#include "constant.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_SIZE 1024

// console display function
void consoleDisplay(void* data) {

    //static int mode = 0;
    //printf("%c", earthCommand);
    /*if (earthCommand == 't') {
        mode = 1 - mode;
        earthCommand = 0;
    }*/

    if (*((ConsoleDisplayData*)data)->batteryLev < 15)
        *((ConsoleDisplayData*)data)->batteryLow = TRUE;
    else
        *((ConsoleDisplayData*)data)->batteryLow = FALSE;
    if (*((ConsoleDisplayData*)data)->fuelLev < 15)
        *((ConsoleDisplayData*)data)->fuelLow = TRUE;
    else
        *((ConsoleDisplayData*)data)->fuelLow = FALSE;

    sprintf(getParamValuePtr("solarPanelState"), "%s", *((ConsoleDisplayData*)data)->solarPanelState ? "Deployed" : "Retracted");
    sprintf(getParamValuePtr("solarPanelRetract"), "%s", *((ConsoleDisplayData*)data)->solarPanelRetract ? "Yes" : "No");
    sprintf(getParamValuePtr("solarPanelDeploy"), "%s", *((ConsoleDisplayData*)data)->solarPanelDeploy ? "Yes" : "No");
    sprintf(getParamValuePtr("batteryLev"), "%d", *((ConsoleDisplayData*)data)->batteryLev);
    sprintf(getParamValuePtr("fuelLev"), "%d", *((ConsoleDisplayData*)data)->fuelLev);
    sprintf(getParamValuePtr("powerCon"), "%d", *((ConsoleDisplayData*)data)->powerCon);
    sprintf(getParamValuePtr("powerGen"), "%d", *((ConsoleDisplayData*)data)->powerGen);
    sprintf(getParamValuePtr("transportDis"), "%lu", *((ConsoleDisplayData*)data)->transportDis);
    sprintf(getParamValuePtr("batteryTemp1"), "%lf", **((ConsoleDisplayData*)data)->batteryTempPtr1);
    sprintf(getParamValuePtr("batteryTemp2"), "%lf", **((ConsoleDisplayData*)data)->batteryTempPtr2);
    sprintf(getParamValuePtr("imageData"), "%d", **((ConsoleDisplayData*)data)->imageFrequencyPtr);
    sprintf(getParamValuePtr("batteryLow"), "%s", *((ConsoleDisplayData*)data)->batteryLow ? "low" : "normal");
    sprintf(getParamValuePtr("fuelLow"), "%s", *((ConsoleDisplayData*)data)->fuelLow ? "low" : "normal");
    sprintf(getParamValuePtr("batteryOverTemp"), "%s", *((ConsoleDisplayData*)data)->batteryOverTemp ? "too high" : "normal");
}
