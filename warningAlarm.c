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
    static time_t occurTime = 0;
    
    if (start == 0)
        start = time(NULL);
    
    if(BOT)
        occurTime = time(NULL);

    int flag = 0;
    Bool alarm = FALSE;
    
    FILE *led;
    FILE *led1;
    FILE *led2;
    
    int batteryLev = *((WarningAlarmData*)data)->batteryLev;
    int fuelLev = *((WarningAlarmData*)data)->fuelLev;
    Bool* BOT = *((WarningAlarmData*)data)->BOT;

    
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
    
    //
    if ((t % 5) && (!alarm)) {
        flag |= 0x3;
    }
    
    // TODO: check if alarm is acknowledged
    // CODE HERE
    // avaliable audible alarm;
    FILE *gpioE, *gpioW;
    char userInput[1];
    
    time_t currTime = time(NULL);
    currTime = currTime - occurTime;
    
    gpioE = fopen("/sys/class/gpio/export", "w");
    fseek(gpioE,0,SEEK_SET);
    fprintf(gpioE,"65");
    fflush(gpioE);
    fclose(gpioE);
    
    if(BOT) {
        if(currTime > 15000000000 && userInput[0] != 'a') {
            gpioW = fopen("/sys/class/gpio/gpio7/value", "w");
            fseek(gpioW,0,SEEK_SET);
            fprintf(gpioW,"0");
            fflush(gpioW);
            fclose(gpioW);
            
            if (flag & 0x3) {
                led1 = fopen(OUTPUT_LED1, "w");
                led2 = fopen(OUTPUT_LED2, "w");
                fprintf(led1, "%d", 1);
                fprintf(led2, "%d", 1);
                fflush(led1);
                fflush(led2);
                fclose(led1);
                fclose(led2);
            } else {
                led1 = fopen(OUTPUT_LED1, "w");
                led2 = fopen(OUTPUT_LED2, "w");
                fprintf(led1, "%d", 0);
                fprintf(led2, "%d", 0);
                fflush(led1);
                fflush(led2);
                fclose(led1);
                fclose(led2);
            }
        } else if(currTime <= 15000000000 && userInput[0] == 'a') {
            gpioW = fopen("/sys/class/gpio/gpio7/value", "w");
            fseek(gpioW,0,SEEK_SET);
            fprintf(gpioW,"1");
            fflush(gpioW);
            fclose(gpioW);
        }
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
