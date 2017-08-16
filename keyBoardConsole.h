#ifndef keyBoardConsole_h
#define keyBoardConsole_h

#include "constant.h"

// keyboard console function
void keyBoardConsole(void* data);

//
struct KeyBoardConsoleDataStruct {
    Bool* dmsInc;
    Bool* dmsDec;
    Bool* solarPanelRetract;
    Bool* solarPanelDeploy;
};

typedef struct KeyBoardConsoleDataStruct KeyBoardConsoleData;

#endif /* keyBoardConsole_h */
