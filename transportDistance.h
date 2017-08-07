#ifndef transportDistance_h
#define transportDistance_h

#include "constant.h"

void transportDistance(void* data);

struct TransportDistanceDataStruct {
    
    unsigned long* transportDis;
    
};

void enableGPIOforTransport();

typedef struct TransportDistanceDataStruct TransportDistanceData;

#endif /* transportDistance_h */
