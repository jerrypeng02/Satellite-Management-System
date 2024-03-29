#include "vehicleComms.h"
#include "constant.h"
#include "pipeCommons.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

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

    //printf("SSSS\n");
    
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;

    char buf[MAX_SIZE];

    char *command = ((VehicleCommsData*)data)->vehicleCommand;
    char *response = ((VehicleCommsData*)data)->response;
    
        // F Forward
        // B Back
        // L Left
        // R Right
        // D Drill down – Start
        // H Drill up – Stop

    if (*command == 'F' ||
            *command  == 'B' ||
            *command  == 'L' ||
            *command  == 'R' ||
            *command  == 'D' ||
            *command  == 'H') {
        write(fdw, command, 1);
        *command = '\0';
        //close(fd0); // close the FIFO
        //unlink(myfifo0);

    }
    
    int result = read(fdr, buf, MAX_SIZE);
    if (result > 0) {
        if (result == 1) {
            if (buf[0] == 'T') {
                write(fdw, "K", 1);
                isrNum = 0;
                raise(SIGUSR1);
                printf("Request for transport lift-off\n");
            } else if (buf[0] == 'D') {
                write(fdw, "C", 1);
                isrNum = 1;
                raise(SIGUSR1);
                printf("Request to dock\n");
            }

            if (buf[0] == 'S') {
                write(fdw, "W", 1);
                isrNum = 2;
                raise(SIGUSR1);
                printf("Start image capture");
            } else if (buf[0] == 'I') {
                write(fdw, "P", 1);
                isrNum = 3;
                raise(SIGUSR1);
                printf("Send image data\n");
            }
        } else {
            *response = 'A';
            buf[result] = '\0';
        }
    }
}
