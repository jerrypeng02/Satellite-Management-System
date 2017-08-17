#include "satelliteComs.h"
#include "constant.h"
#include "webserver.h"
#include "util.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#ifdef BEAGLEBONE
#define EARTH_COMMAND "/dev/pts/0"
#define EARTH_TERMINAL "/dev/pts/1"
#else
#define EARTH_COMMAND "/dev/ttys000"
#define EARTH_TERMINAL "/dev/ttys001"
#endif

#define MAX_SIZE 1024

struct ResponseContext context;
// satellite communication function
void satelliteComs(void* data) {

    volatile int i;
    static struct ValuePair params[18] = {{"satelliteName", "Explorer"},
                                         {"date", "2017-8-16"},
                                         {"operatorName", "Zehao Sun"},
                                         {"response", ""},

                                         {"solarPanelState", "2017-8-16"},
                                         {"solarPanelRetract", "2017-8-16"},
                                         {"solarPanelDeploy", "2017-8-16"},
                                         {"batteryLev", "2017-8-16"},
                                         {"fuelLev", "2017-8-16"},
                                         {"powerCon", "2017-8-16"},
                                         {"powerGen", "2017-8-16"},
                                         {"transportDis", "2017-8-16"},
                                         {"batteryTemp1", "2017-8-16"},
                                         {"batteryTemp2", "2017-8-16"},
                                         {"imageData", "2017-8-16"},
                                         {"batteryLow", "2017-8-16"},
                                         {"fuelLow", "2017-8-16"},
                                         {"batteryOverTemp", "2017-8-16"}};
    static char values[18][100];
    static char command[MAX_SIZE], payload[MAX_SIZE];

    static int fdEarthR = -1;

    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;

    // connect to earth terminal
    if (fdEarthR == -1) {
        fdEarthR = open(EARTH_COMMAND, O_RDONLY | O_NONBLOCK);

        context.params = params;
        context.paramSize = 18;
        context.command = command;
        context.payload = payload;
        for (i = 3; i < context.paramSize; i ++) {
            values[i][0] = '\0';
            params[i].value = values[i];
        }

        start_webserver(8888, &context);
    }
}

char* getParamValuePtr(const char* name) {
    volatile int i;
    for (i = 0; i < context.paramSize; i ++) {
        if (strcmp(name, context.params[i].name) == 0)
            return context.params[i].value;
    }
    return NULL;
}

char* getEarthCommand() {
    return context.command;
}

char* getEarthPayload() {
    return context.payload;
}

void clearEarthCommand() {
    context.command[0] = '\0';
}

