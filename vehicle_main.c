#include "pipeCommons.h"

#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int fdr, fdw;
    /* create the FIFO (named pipe) */
    mkfifo(FIFO_V_TO_S, 0666);
    mkfifo(FIFO_S_TO_V, 0666);

    fdr = open(FIFO_S_TO_V, O_RDONLY);
    fdw = open(FIFO_V_TO_S, O_WRONLY);
    
    char command[1024];
    char buffer[1024];
    int x = 0, y = 0;
    
    ssize_t result;
    while (1) {
        result = read(fdr, command, 1024);
        if (result != -1) {
            printf("Command received: %c", command[0]);
            sprintf(buffer, "x = %d, y = %d\n", x, y);
            printf("send: %s", buffer);
            write(fdw, buffer, strlen(buffer));
        }
    }
    close(fdr);
    close(fdw);

    /* remove the FIFO */
    unlink(FIFO_V_TO_S);
    unlink(FIFO_S_TO_V);

    return 0;
}

