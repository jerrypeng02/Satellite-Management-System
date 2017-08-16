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
//#define EARTH_TERMINAL "/dev/pts/1"
#else
#define EARTH_COMMAND "/dev/ttys000"
//#define EARTH_TERMINAL "/dev/ttys001"
#endif

#define MAX_SIZE 1024

char earthCommand = '\0';

struct ResponseContext context;
// satellite communication function
void satelliteComs(void* data) {

    volatile int i;
    static struct ValuePair params[17] = {{"satelliteName", "Explorer"},
                                         {"date", "2017-8-16"},
                                         {"operatorName", "Zehao Sun"},

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
    static char values[17][100];

    static int fdEarthR = -1;

    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;

    // connect to earth terminal
    if (fdEarthR == -1) {
        fdEarthR = open(EARTH_COMMAND, O_RDONLY | O_NONBLOCK);

        context.params = params;
        context.paramSize = 17;
        for (i = 3; i < context.paramSize; i ++) {
            values[i][0] = '\0';
            params[i].value = values[i];
        }

        start_webserver(8888, &context);
    }

    // initialize
    *((SatelliteComsData*)data)->thrusterComm = randomInteger(0, 0xFFFF);

    // read earthCommand
    char buffer[MAX_SIZE];
    ssize_t result = read(fdEarthR, buffer, MAX_SIZE);
    
    if (result != -1) {
        earthCommand = buffer[0];
    } else {
        earthCommand = 0;
    }
    
    //printf("command: %d", thrusterComm);
    /*
    if (fdEarthW && fp) {
    	dprintf(fdEarthW, "\033[2J");
    	dprintf(fdEarthW, "\033[1;1H");

	    // Read the buffer from the file
        fseek(fp, 0, SEEK_SET);
        fread(buffer, 1, MAX_SIZE, fp);

        // Transmit buffer to terminal
        dprintf(fdEarthW, "%s\n", buffer);

        fseek(fp, 0, SEEK_SET);
	}
    Bool fuelLow = *((SatelliteComsData*)data)->fuelLow;
    Bool batteryLow = *((SatelliteComsData*)data)->batteryLow;
    Bool solarPanelState = *((SatelliteComsData*)data)->solarPanelState;
    unsigned short batteryLev = *((SatelliteComsData*)data)->batteryLev;
    unsigned short fuelLev = *((SatelliteComsData*)data)->fuelLev;
    unsigned short powerCon = *((SatelliteComsData*)data)->powerCon;
    unsigned short powerGen = *((SatelliteComsData*)data)->powerGen;

    // encode the status information from satellite to earth
    unsigned int result = (fuelLow << 3) | (batteryLow << 2) | (solarPanelState << 1)
    | (powerCon << 4) | (batteryLev << 8)
    | (fuelLev << 16) | (powerGen << 24);
    */

}

char* getParamValuePtr(const char* name) {
    volatile int i;
    for (i = 0; i < context.paramSize; i ++) {
        if (strcmp(name, context.params[i].name) == 0)
            return context.params[i].value;
    }
    return NULL;
}

