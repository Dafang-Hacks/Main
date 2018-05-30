/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** v4l2DeviceSource.cpp
** 
** V4L2 Live555 source 
**
** -------------------------------------------------------------------------*/

#include <fcntl.h>
#include <iomanip>
#include <sstream>

// project

#define LOGURU_WITH_STREAMS 1
#include <loguru.hpp>

#include "DeviceSource.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
// ---------------------------------
// V4L2 FramedSource Stats
// ---------------------------------
int V4L2DeviceSource::Stats::notify(int tv_sec, int framesize) {
    m_fps++;
    m_size += framesize;
    if (tv_sec != m_fps_sec) {
        LOG_S(9) << m_msg << "tv_sec:" << tv_sec << " fps:" << m_fps << " bandwidth:" << (m_size / 128) << "kbps";
        m_fps_sec = tv_sec;
        m_fps = 0;
        m_size = 0;
    }
    return m_fps;
}

// ---------------------------------
// V4L2 FramedSource
// ---------------------------------
V4L2DeviceSource *
V4L2DeviceSource::createNew(UsageEnvironment &env, DeviceInterface *device, int outputFd, unsigned int queueSize,
                            bool useThread) {
    V4L2DeviceSource *source = NULL;
    if (device) {
        source = new V4L2DeviceSource(env, device, outputFd, queueSize, useThread);
    }
    return source;
}

// Constructor
V4L2DeviceSource::V4L2DeviceSource(UsageEnvironment &env, DeviceInterface *device, int outputFd, unsigned int queueSize,
                                   bool useThread)
        : FramedSource(env),
          m_in("in"),
          m_out("out"),
          m_outfd(outputFd),
          m_device(device),
          m_queueSize(queueSize) {
    m_eventTriggerId = envir().taskScheduler().createEventTrigger(V4L2DeviceSource::deliverFrameStub);
    memset(&m_thid, 0, sizeof(m_thid));
    memset(&m_mutex, 0, sizeof(m_mutex));
    if (m_device) {
        if (useThread) {
            pthread_mutex_init(&m_mutex, NULL);
            pthread_create(&m_thid, NULL, threadStub, this);
        } else {
            envir().taskScheduler().turnOnBackgroundReadHandling(m_device->getFd(),
                                                                 V4L2DeviceSource::incomingPacketHandlerStub, this);
        }
    }
}

// Destructor
V4L2DeviceSource::~V4L2DeviceSource() {
    envir().taskScheduler().deleteEventTrigger(m_eventTriggerId);
    pthread_join(m_thid, NULL);
    pthread_mutex_destroy(&m_mutex);
    delete m_device;
}

// thread mainloop
void *V4L2DeviceSource::thread() {
    int stop = 0;
    fd_set fdset;
    FD_ZERO(&fdset);

    loguru::set_thread_name("DeviceSource");
    LOG_S(INFO) << "begin thread";
    while (!stop) {
        if (this->getNextFrame() <= 0) {
            LOG_S(ERROR) << "error:" << strerror(errno);
            stop = 1;
        }

    }
    LOG_S(INFO) << "end thread";
    return NULL;
}

// getting FrameSource callback
void V4L2DeviceSource::doGetNextFrame() {
    deliverFrame();
}

// stopping FrameSource callback
void V4L2DeviceSource::doStopGettingFrames() {
    LOG_S(INFO) << "V4L2DeviceSource::doStopGettingFrames";
    FramedSource::doStopGettingFrames();
}

// deliver frame to the sink
void V4L2DeviceSource::deliverFrame() {
    if (isCurrentlyAwaitingData()) {
        fDurationInMicroseconds = 0;
        fFrameSize = 0;

        pthread_mutex_lock(&m_mutex);
        if (m_captureQueue.empty()) {
            //LOG_S(INFO) << "Queue is empty";
        } else {
            timeval curTime;
            gettimeofday(&curTime, NULL);
            Frame *frame = m_captureQueue.front();
            m_captureQueue.pop_front();

            m_out.notify(curTime.tv_sec, frame->m_size);
            if (frame->m_size > (int)fMaxSize) {
                fFrameSize = fMaxSize;
                fNumTruncatedBytes = frame->m_size - fMaxSize;
            } else {
                fFrameSize = frame->m_size;
            }
            timeval diff;
            timersub(&curTime, &(frame->m_timestamp), &diff);

            LOG_S(9) << "deliverFrame\ttimestamp:" << curTime.tv_sec << "." << curTime.tv_usec << "\tsize:"
                       << fFrameSize << "\tdiff:" << (diff.tv_sec * 1000 + diff.tv_usec / 1000) << "ms\tqueue:"
                       << m_captureQueue.size();

            fPresentationTime = frame->m_timestamp;
            memcpy(fTo, frame->m_buffer, fFrameSize);
            delete frame;
        }
        pthread_mutex_unlock(&m_mutex);

        if (fFrameSize > 0) {
            // send Frame to the consumer
            FramedSource::afterGetting(this);
        }
    }
}

// FrameSource callback on read event
void V4L2DeviceSource::incomingPacketHandler() {
    if (this->getNextFrame() <= 0) {
        handleClosure(this);
    }
}

// read from device
int V4L2DeviceSource::getNextFrame() {
    timeval ref;
    gettimeofday(&ref, NULL);
    char buffer[m_device->getBufferSize()];
    int frameSize = m_device->read(buffer, m_device->getBufferSize());
    if (frameSize < 0) {
        LOG_S(9) << "V4L2DeviceSource::getNextFrame errno:" << errno << " " << strerror(errno);
    } else if (frameSize == 0) {
        LOG_S(9) << "V4L2DeviceSource::getNextFrame no data errno:" << errno << " " << strerror(errno);
    } else {
        timeval tv;
        gettimeofday(&tv, NULL);
        timeval diff;
        timersub(&tv, &ref, &diff);
        m_in.notify(tv.tv_sec, frameSize);
        LOG_S(9) << "getNextFrame\ttimestamp:" << ref.tv_sec << "." << ref.tv_usec << "\tsize:" << frameSize
                   << "\tdiff:" << (diff.tv_sec * 1000 + diff.tv_usec / 1000) << "ms";
        processFrame(buffer, frameSize, ref);
        switch(m_outfd){
            case -1:

            break;
            case -2:
                key_t key1;
                key1 = ftok("/usr/include", 'x');
                int shm_id;
                shm_id = shmget( key1, m_device->getBufferSize(), IPC_CREAT);
                void* shared_mem;
                shared_mem = shmat( shm_id, NULL, 0);
                memcpy(shared_mem,buffer,frameSize);
                shmdt(shared_mem);
            break;
            default:
                fwrite (buffer , 1, frameSize, (FILE*)m_outfd);
            break;
        }
    }
    return frameSize;
}


void V4L2DeviceSource::processFrame(char *frame, int frameSize, const timeval &ref) {
    timeval tv;
    gettimeofday(&tv, NULL);
    timeval diff;
    timersub(&tv, &ref, &diff);

    std::list <std::pair<unsigned char *, size_t>> frameList = this->splitFrames((unsigned char *) frame, frameSize);
    while (!frameList.empty()) {
        std::pair<unsigned char *, size_t> &frame = frameList.front();
        size_t size = frame.second;
        char *buf = new char[size];
        memcpy(buf, frame.first, size);
        queueFrame(buf, size, ref);

        LOG_S(9) << "queueFrame\ttimestamp:" << ref.tv_sec << "." << ref.tv_usec << "\tsize:" << size << "\tdiff:"
                   << (diff.tv_sec * 1000 + diff.tv_usec / 1000) << "ms";
        frameList.pop_front();
    }
}

// post a frame to fifo
void V4L2DeviceSource::queueFrame(char *frame, int frameSize, const timeval &tv) {
    pthread_mutex_lock(&m_mutex);
    while (m_captureQueue.size() >= m_queueSize) {
        //LOG_S(INFO) << "Queue full size drop frame size:" << (int) m_captureQueue.size();
        delete m_captureQueue.front();
        m_captureQueue.pop_front();
    }
    m_captureQueue.push_back(new Frame(frame, frameSize, tv));
    pthread_mutex_unlock(&m_mutex);

    // post an event to ask to deliver the frame
    envir().taskScheduler().triggerEvent(m_eventTriggerId, this);
}

// split packet in frames
std::list <std::pair<unsigned char *, size_t>> V4L2DeviceSource::splitFrames(unsigned char *frame, unsigned frameSize) {
    std::list <std::pair<unsigned char *, size_t>> frameList;
    if (frame != NULL) {
        frameList.push_back(std::pair<unsigned char *, size_t>(frame, frameSize));
    }
    return frameList;
}


	
