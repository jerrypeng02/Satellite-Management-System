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

#define MAX_SIZE 1024

// vehicle communication function
void vehicleComms(void* data) {
    
    static int fdw = -1;
    static int fdr = -1;
    if (fdw == -1 && fdr == -1) {
        mkfifo(FIFO_S_TO_V, 0666); // create the FIFO (named pipe)
        fdw = open(FIFO_S_TO_V, O_WRONLY); // open the FIFO
        mkfifo(FIFO_V_TO_S, 0666); // create the FIFO (named pipe)
        fdr = open(FIFO_V_TO_S, O_RDONLY | O_NONBLOCK); // open the FIFO
    }
    
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
    if (earthCommand == 'F' ||
            earthCommand == 'B' ||
            earthCommand == 'L' ||
            earthCommand == 'R' ||
            earthCommand == 'D' ||
            earthCommand == 'H') {
        *command = earthCommand;
        earthCommand = 0;

        write(fdw, command, 1);
        //close(fd0); // close the FIFO
        /* remove the FIFO */
        //unlink(myfifo0);

    }
    
    int result = read(fdr, buf, MAX_SIZE);
    if (result > 0) {
        if (result == 1) {
            if (buf[0] == 'T') {
                write(fdw, "K", 1);
                printf("transport lift-off\n");
            } else if (buf[0] == 'D') {
                write(fdw, "C", 1);
                printf("dock\n");
            }
        } else {
            *response = 'A';
            buf[result] = '\0';
            earthOutput(buf);
        }
    }
}
