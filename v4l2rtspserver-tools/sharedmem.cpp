#include <cstdlib>
#include "sharedmem.h"

SharedMem::SharedMem() {
    currentConfig.nightmode = 0;
    currentConfig.flip = 0;
    memset(currentConfig.osdTimeDisplay,0,STRING_MAX_SIZE);
    currentConfig.osdColor = 0;
    currentConfig.osdSize = 0;
    currentConfig.osdFixedWidth = 0;
    currentConfig.sensitivity = -2;
    currentConfig.motionOSD = 0;
    currentConfig.detectionRegion[0] = 0;
    currentConfig.detectionRegion[1] = 0;
    currentConfig.detectionRegion[2] = 0;
    currentConfig.detectionRegion[3] = 0;
    currentConfig.motionTracking = false;
    currentConfig.motionTimeout = -1;
    currentConfig.softVolume = -1;
    currentConfig.hardVolume = -1;
    currentConfig.filter = 0;
    currentConfig.highfilter = 0;
    currentConfig.aecfilter = 0;
    currentConfig.frmRateConfig[0] = 25;
    currentConfig.frmRateConfig[1] = 1;
    memset(currentConfig.osdFontName,0,STRING_MAX_SIZE);
    currentConfig.osdTime = 1;

    key_image_mem = ftok("/usr/", '1');
    key_image_semaphore = ftok("/usr/", '2');
    key_config_mem = ftok("/usr/", '3');
    key_config_semaphore = ftok("/usr/", '4');

    semaphore_lock[0].sem_flg = 0;
    semaphore_lock[0].sem_num = (unsigned short) -1;
    semaphore_lock[0].sem_op = SEM_UNDO;


    semaphore_unlock[0].sem_flg = 0;
    semaphore_unlock[0].sem_num = (unsigned short) 1;
    semaphore_unlock[0].sem_op = SEM_UNDO;



}

SharedMem::~SharedMem() {

}

int SharedMem::getImageSize() {
    return this->getMemorySize(key_image_mem);
}

void *SharedMem::getImageBuffer() {
    int memlen = this->getImageSize();
    void *memory = malloc(memlen);

    this->lockSemaphore(key_image_semaphore);
    this->readMemory(key_image_mem, memory, memlen);
    this->unlockSemaphore(key_image_semaphore);


    return memory;
}

void SharedMem::copyImage(void *imageMemory, int imageSize) {
    this->lockSemaphore(key_image_semaphore);
    this->writeMemory(key_image_mem, imageMemory, imageSize);
    this->unlockSemaphore(key_image_semaphore);
}

shared_conf *SharedMem::getConfig() {
    return &currentConfig;
}

void SharedMem::readConfig(){
    this->lockSemaphore(key_config_semaphore);
    this->readMemory(key_config_mem, &currentConfig, sizeof(shared_conf));
    this->unlockSemaphore(key_config_semaphore);
}

void SharedMem::setConfig() {
    this->lockSemaphore(key_config_semaphore);
    this->writeMemory(key_config_mem, &currentConfig, sizeof(shared_conf));
    this->unlockSemaphore(key_config_semaphore);
}

void SharedMem::lockSemaphore(key_t key) {
    semop(key, &semaphore_lock[0], 1);
}

void SharedMem::unlockSemaphore(key_t key) {
    semop(key, &semaphore_unlock[0], 1);


}

void SharedMem::readMemory(key_t key, void *memory, int memorylenght) {
    void *shared_mem;
    int shm_id = shmget(key, 0, 0);
    if (shm_id == -1) {
        return;
    }
    shared_mem = shmat(shm_id, NULL, 0);
    memcpy(memory, shared_mem, (size_t) memorylenght);
    shmdt(shared_mem);
}

int SharedMem::getMemorySize(key_t key) {
    int shm_id = shmget(key, 0, 0);
    struct shmid_ds buf;
    shmctl(shm_id, IPC_STAT, &buf);
    int memlen = buf.shm_segsz;
    return memlen;
}

void SharedMem::writeMemory(key_t key, void *memory, int memorylenght) {

    int shm_id;

    shm_id = shmget(key, 0, 0);
    if (shm_id != -1) {
        int memlen = this->getMemorySize(key);
        if (memlen != memorylenght) {
            shmctl(shm_id, IPC_RMID, NULL);
        }
    }


    shm_id = shmget(key, memorylenght, IPC_CREAT);
    if (shm_id != -1) {
        void *shared_mem;
        shared_mem = shmat(shm_id, NULL, 0);
        memcpy(shared_mem, memory, memorylenght);
        shmdt(shared_mem);
    } else {

    }


}

void *SharedMem::getImage() {
    int memlen = getImageSize();
    void *memory = malloc((size_t) memlen);
    this->lockSemaphore(key_image_semaphore);
    this->readMemory(key_image_mem, memory, memlen);
    this->unlockSemaphore(key_image_semaphore);
    return memory;

}