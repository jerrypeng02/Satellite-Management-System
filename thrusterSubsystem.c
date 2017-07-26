#include "thrusterSubsystem.h"
#include "constant.h"

// thruster subsystem function
void thrusterSubsystem(void* data) {
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;
    
    unsigned short left = 0;
    unsigned short right = 0;
    unsigned short up = 0;
    unsigned short down = 0;
    
    unsigned int thrusterComm = *((ThrusterSubsystemData*)data)->thrusterComm;
    unsigned short fuelLev = *((ThrusterSubsystemData*)data)->fuelLev;
    
    unsigned int duration = thrusterComm >> 8;
    unsigned int magnititude = (thrusterComm & 0xFF) >> 4;
    unsigned int thruster =  thrusterComm & 0xF;
    
    //printf("command: %d", *((ThrusterSubsystemData*)data)->thrusterComm);
    //printf("duration: %d, mag: %d, thruster: %d\n", duration, magnititude, thruster);
    
    left = thruster & 0x1;
    right = (thruster >> 1) & 0x1;
    up = (thruster >> 2) & 0x1;
    down = (thruster >> 3) & 0x1;
    
    // send thruster signal
    if (duration > 0) {
        // we decide fuel level formulate to be
        if (*((ThrusterSubsystemData*)data)->fuelLev >= magnititude / 3) {
            *((ThrusterSubsystemData*)data)->fuelLev -= magnititude / 3;
        } else {
            *((ThrusterSubsystemData*)data)->fuelLev = 0;
        }
        if (duration >= 5)
            duration -= 5;
        else
            duration = 0;
    }
    
    *((ThrusterSubsystemData*)data)->thrusterComm = (duration << 8) | (magnititude << 4) | thruster;
    //printf("command: %d", *((ThrusterSubsystemData*)data)->thrusterComm);
}
