#include "keyBoardConsole.h"
#include "solarPanelControl.h"
#include "constant.h"
#include <time.h>
#define PAUSE_TIME 2

void pauseSec(int sec); // software timer

// keyboard console function
void keyBoardConsole(void* data) {
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;

    Bool dmsInc = *((KeyBoardConsoleData*)data)->dmsInc;
    Bool dmsDec = *((KeyBoardConsoleData*)data)->dmsDec;
    Bool solarPanelDeploy = *((SolarPanelControlData*)data)->solarPanelDeploy;
    Bool solarPanelRetract = *((SolarPanelControlData*)data)->solarPanelRetract;
    
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
        if (solarPanelRetract || solarPanelDeploy) {
            if(userinput[0] == 'i') {
                dmsInc = TRUE;
            } else {
                dmsInc = FALSE;
            }
        
            if(userinput[0] == 'd') {
                dmsDec = TURE;
            } else {
                dmsDec = FALSE;
            }
        }
    }
    pauseSec(PAUSE_TIME);
    
}

void pauseSec(int sec) {
    time_t now,later;
    now = time(NULL);
    later = time(NULL);
    while((later - now) < (double) sec)
        later = time(NULL);
}
