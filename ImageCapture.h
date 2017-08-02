#ifndef imageCapture_h
#define imageCapture_h

void imageCapture(void* data);

struct ImageCaptureDataStruct {
    unsigned int* imageDataRawPtr;
    unsigned int* imageDataPtr;
};

#endif /* imageCapture_h */
