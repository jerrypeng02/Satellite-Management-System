//
//  keyBoardConsole.h
//  
//
//  Created by Ningyang Peng on 7/25/17.
//
//

#ifndef keyBoardConsole_h
#define keyBoardConsole_h

#include "constant.h"

// keyboard console function
void keyBoardConsole(void* data);

//
struct KeyBoardConsoleDataStruct {
    Bool* dmsInc;
    Bool* dmsDec;
};

typedef struct KeyBoardConsoleDataStruct KeyBoardConsoleData;

#endif /* keyBoardConsole_h */
