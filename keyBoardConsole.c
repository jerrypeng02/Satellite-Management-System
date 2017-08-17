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

    Bool *solarPanelDeploy = ((KeyBoardConsoleData*)data)->solarPanelDeploy;
    Bool *solarPanelRetract = ((KeyBoardConsoleData*)data)->solarPanelRetract;
    unsigned short *motorDriveSpeed = ((KeyBoardConsoleData*)data)->motorDriveSpeed;

    if (*solarPanelRetract || *solarPanelDeploy) {
        if(earthCommand == 'i') {
            *motorDriveSpeed += 5;
            earthCommand = '\0';
        } else if(earthCommand == 'd') {
            *motorDriveSpeed -= 5;
            earthCommand = '\0';
        }
    }
}
