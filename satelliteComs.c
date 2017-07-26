#include "satelliteComs.h"
#include "constant.h"
#include "util.h"

#define MAX_SIZE 1024

FILE *fp = NULL; // declare temp earth output file here

char earthCommend;
// satellite communication function
void satelliteComs(void* data) {

    static int fdEarthR = -1;
    static int fdEarthW = -1;

    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;

    // connect to earth terminal
    if (fdEarthR == -1)
        fdEarthR = open("/dev/ttys001", O_RDONLY | O_NONBLOCK);
    if (fdEarthW == -1)
        fdEarthW = open("/dev/ttys001", O_WRONLY);

    // initialize
    if (fp == NULL) {
        fp = fopen("earthOutput.txt", "w+");
        if(!fp) {
            exit(1);
        }
    }

    // error structure name error
    *((SatelliteComsData*)data)->thrusterComm = randomInteger(0, 0xFFFF);
    ssize_t result = read(fdEarthR, &earthCommend, 1);
    if (result <= 0) {
        earthCommend = 0;
    }

    char buffer[MAX_SIZE];

    if (fdEarthW && fp) {
    	dprintf(fdEarthW, "\033[2J");
    	dprintf(fdEarthW, "\033[1;1H");

	    // Read the buffer from the file
        fseek(fp, 0, SEEK_SET);
        fread(buffer, 1, MAX, fp);

        // Transmit buffer to terminal
        dprintf(fdEarthW, "%s\n", buffer);

        fseek(fp, 0, SEEK_SET);
	}
    //printf("command: %d", thrusterComm);
    /*
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

// output to earth
void earthOutput(char* output) {
    if (fp) {
        fwrite(output, strlen(output) + 1, 1, fp);
    }
}
