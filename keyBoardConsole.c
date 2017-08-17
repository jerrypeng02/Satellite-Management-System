#include "keyBoardConsole.h"
#include "satelliteComs.h"
#include "solarPanelControl.h"
#include "constant.h"

#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// keyboard console function
void keyBoardConsole(void* data) {
    // 2 sec
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR * 2 / 5 != 0)
        return;

    Bool* dmsInc = ((KeyBoardConsoleData*)data)->dmsInc;
    Bool* dmsDec = ((KeyBoardConsoleData*)data)->dmsDec;
    Bool solarPanelDeploy = *((KeyBoardConsoleData*)data)->solarPanelDeploy;
    Bool solarPanelRetract = *((KeyBoardConsoleData*)data)->solarPanelRetract;

    if (solarPanelRetract || solarPanelDeploy) {
        if(earthCommand == 'i') {
            *dmsInc = TRUE;
        } else {
            *dmsInc = FALSE;
        }
        if(earthCommand == 'd') {
            *dmsDec = TRUE;
        } else {
            *dmsDec = FALSE;
        }
    }
}
