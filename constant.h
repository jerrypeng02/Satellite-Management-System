//
//  constant.h
//
//
//  Created by Ningyang Peng on 7/25/17.
//
//

#ifndef constant_h
#define constant_h

#define BEAGLEBONE

extern const unsigned long MINOR_CYCLE_NUM_IN_MAJOR;
extern const unsigned long MINOR_CYCLE_MS;
extern const unsigned long MAJOR_CYCLE_SEC;

extern unsigned long taskCounter;

extern double batteryTemp1[16];
extern double batteryTemp2[16];

extern const int HEADER;
extern const int PIN;
extern const int PERIOD;

enum _Boolean { FALSE = 0, TRUE = 1 };

typedef enum _Boolean Bool;

#endif /* constant_h */
