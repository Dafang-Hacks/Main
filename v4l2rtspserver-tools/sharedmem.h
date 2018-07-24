#ifndef SHAREDMEM_H
#define SHAREDMEM_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/sem.h>
#define STRING_MAX_SIZE          256
struct shared_conf {
    int nightmode;
    int flip;
    char osdTimeDisplay[STRING_MAX_SIZE];
    int bitrate;
    int osdColor;
    int osdSize;
    int osdPosY;
    int osdSpace;
    bool osdFixedWidth;
    int sensitivity;
    int motionOSD;
    int detectionRegion[4]; // x0,y0,x1,y1
    bool motionTracking;
    int motionTimeout;
    int hardVolume;
    int softVolume;
    char filter;
    bool highfilter;
    bool aecfilter;
    int frmRateConfig[2];
    char osdFontName[STRING_MAX_SIZE];
};


class SharedMem {
public:
    SharedMem();

    void* getImage();

    static SharedMem &instance() {
        static SharedMem _instance;
        return _instance;
    }


    ~SharedMem();

    shared_conf *getConfig();
    void setConfig();


    int getImageSize();
    void *getImageBuffer();
    void copyImage(void *imageMemory, int imageSize);
    void readConfig();

private:
    key_t key_image_mem;
    key_t key_image_semaphore;
    key_t key_config_mem;
    key_t key_config_semaphore;

    struct shared_conf currentConfig;



    void readMemory(key_t key, void *memory, int memorylenght);

    void lockSemaphore(key_t key);

    void unlockSemaphore(key_t key);



    void writeMemory(key_t key, void *memory, int memorylenght);


    int getMemorySize(key_t key);

    struct sembuf semaphore_lock[1];
    struct sembuf semaphore_unlock[1];



};


#endif
