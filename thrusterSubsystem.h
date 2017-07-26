#ifndef thrusterSubsystem_h
#define thrusterSubsystem_h

// thruster subsystem function
void thrusterSubsystem(void* data);

//
struct ThrusterSubsystemDataStruct {
    unsigned int* thrusterComm;
    unsigned short* fuelLev;
};
typedef struct ThrusterSubsystemDataStruct ThrusterSubsystemData;

#endif /* thrusterSubsystem_h */
