#ifndef vehicleComms_h
#define vehicleComms_h

// vehicle communication function
void vehicleComms(void* data);

//
struct VehicleCommsDataStruct {
    char* command;
    char* response;
};

typedef struct VehicleCommsDataStruct VehicleCommsData;

#endif /* vehicleComms_h */
