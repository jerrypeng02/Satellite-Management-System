#ifndef transportDistance_h
#define transportDistance_h

#include "constant.h"

void transportDistance(void* data);

struct TransportDistanceDataStruct {
    
    unsigned short* transportDis;
    
};
typedef struct TransportDistanceDataStruct TransportDistanceData;

#endif /* transportDistance_h */
