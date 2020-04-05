#include "ImpCapture.h"

#include <sys/types.h>


#include "sharedmem.h"

ImpCapture::ImpCapture(impParams params) {
    impEncoder = new ImpEncoder(params);
    height = params.height;
    width = params.width;
    mode = params.mode;
    //m_buffer = malloc(height*width);
}


int ImpCapture::getWidth() {
    return this->width;
};

int ImpCapture::getHeight() {
    return this->height;
};


size_t ImpCapture::read(char *buffer, size_t bufferSize) {
    int frameSize;


    if (mode != IMP_MODE_JPEG) {
        frameSize = impEncoder->snap_h264((uint8_t*)buffer);
    }
    else
    {
        frameSize = impEncoder->snap_jpg((uint8_t*)buffer);
    }
    //memcpy(buffer, m_buffer, frameSize);
    return frameSize;
}

int ImpCapture::getFd() {
    return 0;
}

unsigned long ImpCapture::getBufferSize() {
    return width*height; //impEncoder->getBufferSize();
}



