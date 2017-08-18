#include "commandManagement.h"
#include "satelliteComs.h"
#include "constant.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void commandManagement(void* data) {
    CommandManagementData* commandManagementData = (CommandManagementData*) data;

    char *response = getParamValuePtr("response");
    char *value;

    if (*getEarthCommand() != 0)
        printf("EarthCommand: %c\n", *getEarthCommand());

    switch(*getEarthCommand()) {
        case 'S':
        //start measurement
            isrNum = 0;
            raise(SIGUSR1);
            isrNum = 2;
            raise(SIGUSR1);
            response[0] = 'A';
            response[1] = '\0';
            break;
        case 'P':
        //stop measurement
            isrNum = 1;
            raise(SIGUSR1);
            isrNum = 3;
            raise(SIGUSR1);
            response[0] = 'A';
            response[1] = '\0';
            break;
        case 'D':
        // toggle Display
            *commandManagementData->display = !*commandManagementData->display;
            response[0] = 'A';
            response[1] = '\0';
            break;
        case 'T':
            // thrust command
            if (sscanf(getEarthPayload(), "%d", commandManagementData->thrusterComm) == 1) {
                response[0] = 'A';
                response[1] = '\0';
            } else {
                response[0] = 'E';
                response[1] = '\0';
            }
            break;
        case 'M':
            // display specific value
            value = getParamValuePtr(getEarthPayload());

            if (value != NULL) {
                response[0] = 'M';
                strncpy(response + 1, value, 100);
            } else {
                response[0] = 'E';
                response[1] = '\0';
            }
            break;
        case 'V':
            if (*getEarthPayload()) {
                *commandManagementData->vehicleCommand = *getEarthPayload();
                response[0] = 'A';
                response[1] = '\0';
            } else {
                response[0] = 'E';
                response[1] = '\0';
            }

        case 'i':
                *commandManagementData->motorDriveSpeed += 5;
                response[0] = 'A';
                response[1] = '\0';
        case 'd':
                *commandManagementData->motorDriveSpeed -= 5;
                response[0] = 'A';
                response[1] = '\0';
        case '\0':
            break;
        default:
            response[0] = 'E';
            response[1] = '\0';
            break;
    }
    clearEarthCommand();
}
/**
S
The S command indicates START mode. The command shall start the embedded
tasks by directing the hardware to initiate all the measurement tasks. In doing so, the
command shall enable all the interrupts.
P
The P command indicates STOP mode. This command shall stop the embedded tasks
by terminating any running measurement tasks. Such an action shall disable any data
collecting interrupts.
D
The D command enables or disables the display.
T<payload>
The T command transmits the thrust command to the satellite.
M<payload>
The M command. The M command requests the return of the most recent value(s) of
the specified data.
The M response. The M response returns of the most recent value(s) of the specified
data.
A <sp command>
The A response acknowledges the receipt of the identified command.
E
The E error response is given for incorrect commands or non-existent commands.
*/