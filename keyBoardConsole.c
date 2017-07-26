#include "keyBoardConsole.h"
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

    Bool dmsInc = *((KeyBoardConsoleData*)data)->dmsInc;
    Bool dmsDec = *((KeyBoardConsoleData*)data)->dmsDec;
    Bool solarPanelDeploy = *((SolarPanelControlData*)data)->solarPanelDeploy;
    Bool solarPanelRetract = *((SolarPanelControlData*)data)->solarPanelRetract;

    static int fdr = -1;

    if (fdr == -1)
        fdr = open("/dev/ttys000", O_RDONLY | O_NONBLOCK);

    char userInput[1];
    ssize_t result = read(fdr, userInput, 1);
    if (result > 0) {
        if (solarPanelRetract || solarPanelDeploy) {
            if(userInput[0] == 'i') {
                dmsInc = TRUE;
            } else {
                dmsInc = FALSE;
            }
            if(userInput[0] == 'd') {
                dmsDec = TRUE;
            } else {
                dmsDec = FALSE;
            }
        }
    }
}
