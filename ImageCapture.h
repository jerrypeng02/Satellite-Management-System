#ifndef imageCapture_h
#define imageCapture_h

void imageCapture(void* data);

struct ImageCaptureDataStruct {
    unsigned int** imageFrequencyPtr;
};
typedef struct ImageCaptureDataStruct ImageCaptureData;

#endif /* imageCapture_h */
