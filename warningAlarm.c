#include "warningAlarm.h"
#include "constant.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>

#define DEBUG

#ifndef DEBUG
#define OUTPUT_LED1 "/sys/class/leds/beaglebone:green:usr1/brightness"
#else
#define OUTPUT_LED1 "led1.txt"
#endif

#ifndef DEBUG
#define OUTPUT_LED2 "/sys/class/leds/beaglebone:green:usr2/brightness"
#else
#define OUTPUT_LED2 "led2.txt"
#endif

#ifndef DEBUG
#define OUTPUT_LED3 "/sys/class/leds/beaglebone:green:usr3/brightness"
#else
#define OUTPUT_LED3 "led3.txt"
#endif

// warning alarm function
void warningAlarm(void* data) {
    static time_t start = 0;
    if (start == 0)
        start = time(NULL);
    //(WarningAlarmData*)data
    int flag = 0;
    
    FILE *led;
    
    int batteryLev = *((WarningAlarmData*)data)->batteryLev;
    int fuelLev = *((WarningAlarmData*)data)->fuelLev;
    
    time_t t = time(NULL);
    t = t - start;
    
    if (t % 2) {
        if (batteryLev <= 10) {
            flag |= 0x2;
        }
        
        if (fuelLev <= 10) {
            flag |= 0x1;
        }
    }
    
    if ((t / 2) % 2) {
        if (batteryLev <= 50 && batteryLev > 10) {
            flag |= 0x2;
        }
        
        if (fuelLev <= 50 && fuelLev > 10) {
            flag |= 0x1;
        }
    }
    
    if (batteryLev > 20 && fuelLev > 20) {
        flag |= 0x4;
    }
    
    // error: LED is always on
    if (flag & 0x1) {
        led = fopen(OUTPUT_LED1, "w");
        fprintf(led, "%d", 1);
        fflush(led);
        fclose(led);
    } else {
        led = fopen(OUTPUT_LED1, "w");
        fprintf(led, "%d", 0);
        fflush(led);
        fclose(led);
    }
    
    if (flag & 0x2) {
        led = fopen(OUTPUT_LED2, "w");
        fprintf(led, "%d", 1);
        fflush(led);
        fclose(led);
    } else {
        led = fopen(OUTPUT_LED2, "w");
        fprintf(led, "%d", 0);
        fflush(led);
        fclose(led);
    }
    
    if (flag & 0x4) {
        led = fopen(OUTPUT_LED3, "w");
        fprintf(led, "%d", 1);
        fflush(led);
        fclose(led);
    } else {
        led = fopen(OUTPUT_LED3, "w");
        fprintf(led, "%d", 0);
        fflush(led);
        fclose(led);
    }
}
