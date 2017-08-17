#ifndef __COMMANDMANAGEMENT_H__
#define __COMMANDMANAGEMENT_H__

#include "constant.h"

// console display function
void commandManagement(void* data);

//
struct CommandManagementDataStruct {
    char* command;
};
typedef struct CommandManagementDataStruct CommandManagementData;


#endif /* consoleDisplay_h */

