#include "pipeCommons.h"

#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

int fdr, fdw;

void *myInputThread(void *vargp)
{
    char buffer[1024];
    while(1) {
        scanf("%s", buffer);
        if (buffer[0] == 'T' || buffer[0] == 'D') {
            printf("Command sent: %c", buffer[0]);
            write(fdw, buffer, strlen(buffer));
        }
    }
    return NULL;
}

int main()
{
    /* create the FIFO (named pipe) */
    mkfifo(FIFO_V_TO_S, 0666);
    mkfifo(FIFO_S_TO_V, 0666);

    fdr = open(FIFO_S_TO_V, O_RDONLY);
    fdw = open(FIFO_V_TO_S, O_WRONLY);
    
    char command[1024];
    char buffer[1024];
    int x = 0, y = 0, d = 0;
    
    pthread_t tid;
    pthread_create(&tid, NULL, myInputThread, NULL);

    ssize_t result;
    while (1) {
        result = read(fdr, command, 1024);
        if (result != -1) {
            printf("Command received: %c", command[0]);
            switch (command[0]) {
                case 'F':
                    y ++;
                    break;
                case 'B':
                    y --;
                    break;
                case 'L':
                    x ++;
                    break;
                case 'D':
                    d = 1;
                    break;
                case 'H':
                    d = 0;
                    break;

                // F Forward
                // B Back
                // L Left
                // R Right
                // D Drill down – Start
                // H Drill up – Stop
            }
            sprintf(buffer, "x = %d, y = %d, drill = %d\n", x, y, d);
            printf("send: %s", buffer);
            write(fdw, buffer, strlen(buffer));
        }
    }

    pthread_join(tid, NULL);

    close(fdr);
    close(fdw);

    /* remove the FIFO */
    unlink(FIFO_V_TO_S);
    unlink(FIFO_S_TO_V);

    return 0;
}

