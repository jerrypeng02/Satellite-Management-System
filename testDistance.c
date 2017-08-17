
#include "transportDistance.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>

struct TaskControlBlock {
    void(*taskPtr)(void*);
    void* taskDataPtr;
    int priority;
    struct TaskControlBlock* next;
    struct TaskControlBlock* prev;
};
typedef struct TaskControlBlock TCB;

unsigned long transportDis = 0;

int main() {
    TCB* tcbPtr;
    TCB transportDistanceTask;
    TransportDistanceData transportDistanceData;
    transportDistanceData.transportDis = &transportDis;
    
    transportDistanceTask.taskDataPtr = (void*)&transportDistanceData;
    transportDistanceTask.taskPtr = transportDistance;
    //while(1) {
        tcbPtr = &transportDistanceTask;
        tcbPtr->taskPtr((tcbPtr->taskDataPtr));
    
        char output[100];
        printf(output, "Transport Distance: %lu\n", 
        transportDis);
    //}
}