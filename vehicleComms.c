#include "vehicleComms.h"
#include "satelliteComs.h"
#include "constant.h"
#include "pipeCommons.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_SIZE 1024;

// vehicle communication function
void vehicleComms(void* data) {
    
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;

    char buf[MAX_SIZE];

    char *command = ((VehicleCommsData*)data)->command;
    char *response = ((VehicleCommsData*)data)->response;
    /*
        F Forward
        B Back
        L Left
        R Right
        D Drill down – Start
        H Drill up – Stop
    */
    if (earthCommend == 'F' ||
            earthCommend == 'B' ||
            earthCommend == 'L' ||
            earthCommend == 'R' ||
            earthCommend == 'D' ||
            earthCommend == 'H') {
        *command = earthCommend;

        mkfifo(FIFO_FILE, 0666); // create the FIFO (named pipe)
        int fd0 = open(FIFO_FILE, O_RDWR); // open the FIFO
        write(fd0, command, 1);
        int result = read(fd0, buf, MAX_BUF);
        if (result > 0) {
            *response = 'A';
        }
        earthOutput(buf);
        close(fd0); // close the FIFO
        /* remove the FIFO */
        //unlink(myfifo0);

    }
}
