#ifndef _WEBCAM_JPEG_DEVICE_SOURCE_HH
#define _WEBCAM_JPEG_DEVICE_SOURCE_HH

#include "DeviceSource.h"
#include "ImpEncoder.h"
#include "logger.h"

class ImpCapture {
public:
    ImpCapture(impParams params);

    int getWidth();

    int getHeight();

    size_t read(char *buffer, size_t bufferSize);

    int getFd();

    unsigned long getBufferSize();

    void *thread();

protected:
    ImpEncoder *impEncoder;
    int width;
    int height;
    int mode;
   //unsigned char *m_buffer;

};

#endif // _WEBCAM_JPEG_DEVICE_SOURCE_HH