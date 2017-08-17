#ifndef __COMMANDMANAGEMENT_H__
#define __COMMANDMANAGEMENT_H__

#include "constant.h"

// console display function
void commandManagement(void* data);

//
struct CommandManagementDataStruct {
    Bool* display;
    unsigned int* thrusterComm;
    char* vehicleCommand;
};
typedef struct CommandManagementDataStruct CommandManagementData;

#endif /* consoleDisplay_h */

