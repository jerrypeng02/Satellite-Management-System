#ifndef imageCapture_h
#define imageCapture_h

void imageCapture(void* data);

struct ImageCaptureDataStruct {
    unsigned int* imageDataRawPtr;
    unsigned int* imageDataPtr;
};

typedef struct ImageCaptureDataStruct ImageCaptureData;

#endif /* imageCapture_h */
