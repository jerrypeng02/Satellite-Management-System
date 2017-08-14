#include "warningAlarm.h"
#include "satelliteComs.h"
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

void enablePWM();

// warning alarm function
void warningAlarm(void* data) {
    
    int batteryLev = *((WarningAlarmData*)data)->batteryLev;
    int fuelLev = *((WarningAlarmData*)data)->fuelLev;
    Bool* batteryOverTemp = ((WarningAlarmData*)data)->batteryOverTemp;
    
    static time_t start = 0;
    
    if (start == 0)
        start = time(NULL);

    int flag = 0;
    
    FILE *led;
    FILE *led1;
    FILE *led2;
    
    time_t t = time(NULL);
    t = t - start;
    
    if (batteryLev > 20 && fuelLev > 20) {
        flag |= 0x4;
    }
    
    // TODO: check if alarm is acknowledged
    // CODE HERE
    // avaliable audible alarm;
    FILE *gpioW, *run;
    static unsigned long startCounter = 0;
    static int isOver = 0;
    
    if (startCounter == 0 && *batteryOverTemp) {
        startCounter = taskCounter;
    }
    
    if (startCounter > 0) {
        // in 15 sec
        if (taskCounter - startCounter < MINOR_CYCLE_NUM_IN_MAJOR * 3) {
            if (earthCommand == 'a') {
                startCounter = 0;
                run = fopen("/sys/devices/ocp.3/pwm_test_P9_19.17/run", "w");
                fseek(run,0,SEEK_SET);
                fprintf(run,"%d",0);
                fflush(run);
                fclose(run);
            } else {
                // gpioW = fopen("/sys/class/gpio/gpio7/value", "w");
                // fseek(gpioW,0,SEEK_SET);
                // fprintf(gpioW,"1");
                // fflush(gpioW);
                // fclose(gpioW);
                enablePWM();
            }
        } else {
            // after 15 sec
            isOver = 1;
            run = fopen("/sys/devices/ocp.3/pwm_test_P9_19.17/run", "w");
            fseek(run,0,SEEK_SET);
            fprintf(run,"%d",0);
            fflush(run);
            fclose(run);
        }
    }
    
    if (isOver) {
        if ((taskCounter / MINOR_CYCLE_NUM_IN_MAJOR) % 2 == 0) {
            if ((taskCounter / MINOR_CYCLE_NUM_IN_MAJOR / 50) % 2 == 0) {
                flag |= 0x3;
            }
        } else {
            flag |= 0x3;
        }
    } else {
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

void enablePWM() {
    FILE *pwm,*duty,*period,*run;
    
        pwm = fopen("/sys/devices/bone_capemgr.9/slots", "w");
        fseek(pwm,0,SEEK_SET);
        fprintf(pwm,"am33xx_pwm");
        fflush(pwm);
        
        fprintf(pwm,"bone_pwm_P9_16");
        fflush(pwm);
    
        period = fopen("/sys/devices/ocp.3/pwm_test_P9_16.17/period", "w");
        fseek(period,0,SEEK_SET);
        fprintf(period,"%d",200000000);
        fflush(period);
    
        duty = fopen("/sys/devices/ocp.3/pwm_test_P9_16.17/duty", "w");
        fseek(duty,0,SEEK_SET);
        fprintf(duty,"%d",100000000);
        fflush(duty);
    
        run = fopen("/sys/devices/ocp.3/pwm_test_P9_19.17/run", "w");
        fseek(run,0,SEEK_SET);
        fprintf(run,"%d",1);
        fflush(run);
    
        // fseek(run,0,SEEK_SET);
        // fprintf(run,"%d",1);
        // fflush(run);
        // while(1);
    
        fclose(pwm);
        fclose(duty);
        fclose(period);
        fclose(run);
        return 0;
}

void enableGPIOforWarning() {
    FILE* gpioE = fopen("/sys/class/gpio/export", "w");
    fseek(gpioE,0,SEEK_SET);
    fprintf(gpioE,"7");
    fflush(gpioE);
    fclose(gpioE);
    
    FILE* gpioD = fopen("/sys/class/gpio/gpio7/direction", "w");
    fseek(gpioD,0,SEEK_SET);
    fprintf(gpioD,"out");
    fflush(gpioD);
    fclose(gpioD);
}
