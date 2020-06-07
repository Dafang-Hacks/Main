#ifndef __SAMPLE_ENCODER_JPEG_H__
#define __SAMPLE_ENCODER_JPEG_H__

#include <imp/imp_common.h>
#include <imp/imp_osd.h>
#include <imp/imp_framesource.h>
#include <imp/imp_isp.h>
#include <imp/imp_encoder.h>

#include <unistd.h>
#include <list>
#include <vector>
#include <sys/ioctl.h>

typedef enum detectionSaveToDiskState
{
    BUFFERIZE = 0,
    LIVETODISK =1,
    LIVETODISKBUFFER=2
} DetectionSaveToDiskState;

#define IMP_MODE_JPEG 1
#define IMP_MODE_H264_STREAM 2
#define IMP_MODE_H264_SNAP 3
#define CH0_INDEX  0
#define CH1_INDEX  1
#define CHN_ENABLE 1
#define CHN_DISABLE 0
#define CHN0_EN                 1
#define CHN1_EN                 1
#define CROP_EN                    1
#define SENSOR_FRAME_RATE_NUM        25
#define SENSOR_FRAME_RATE_DEN        1
#define SENSOR_WIDTH_SECOND        640
#define SENSOR_HEIGHT_SECOND        360

#define FS_CHN_NUM            1
#define ENC_H264_CHANNEL        0
#define ENC_JPEG_CHANNEL        1
#define SLEEP_TIME            1
#define SENSOR_NAME                "jxf22"
#define SENSOR_CUBS_TYPE        TX_SENSOR_CONTROL_INTERFACE_I2C
#define SENSOR_I2C_ADDR            0x40
#define SENSOR_WIDTH            320
#define SENSOR_HEIGHT            240
#define STRING_MAX_SIZE          256


#define SENSOR_INFO_IOC_MAGIC  'S'
#define IOCTL_SINFO_GET			_IO(SENSOR_INFO_IOC_MAGIC, 100)
#define IOCTL_SINFO_FLASH		_IO(SENSOR_INFO_IOC_MAGIC, 101)

#define SENSOR_TYPE_INVALID	-1

enum SENSOR_TYPE
{
    SENSOR_TYPE_OV9712=0,
    SENSOR_TYPE_OV9732,
    SENSOR_TYPE_OV9750,
    SENSOR_TYPE_JXH42,
    SENSOR_TYPE_SC1035,
    SENSOR_TYPE_SC1135,
    SENSOR_TYPE_SC1045,
    SENSOR_TYPE_SC1145,
    SENSOR_TYPE_AR0130,
    SENSOR_TYPE_JXH61,
    SENSOR_TYPE_GC1024,
    SENSOR_TYPE_GC1064,
    SENSOR_TYPE_GC2023,
    SENSOR_TYPE_BF3115,
    SENSOR_TYPE_IMX225,
    SENSOR_TYPE_OV2710,
    SENSOR_TYPE_IMX322,
    SENSOR_TYPE_SC2135,
    SENSOR_TYPE_SP1409,
    SENSOR_TYPE_JXH62,
    SENSOR_TYPE_BG0806,
    SENSOR_TYPE_OV4689,
    SENSOR_TYPE_JXF22,
    SENSOR_TYPE_IMX323,
    SENSOR_TYPE_IMX291
};

typedef struct SENSOR_INFO_S
{
    const char *name;
} SENSOR_INFO_T;



struct impParams {
    int mode;
    int width;
    int height;
    int bitrate;
    int framerate;
    int rcmode;
};
struct chn_conf {
    unsigned int index;//0 for main channel ,1 for second channel
    unsigned int enable;
    IMPFSChnAttr fs_chn_attr;
    IMPCell framesource_chn;
    IMPCell imp_encoder;
    IMPCell OSD_Cell;
};

class ImpEncoder {
public:
    ImpEncoder(impParams params);

    ~ImpEncoder();

    void requestIDR();

    int snap_h264(uint8_t *buffer);
    int snap_jpg(uint8_t *buffer);
    static void snap_jpeg(std::vector<uint8_t> &buffer);
    static int save_stream(uint8_t *buffer, IMPEncoderStream &stream);
    static void save_stream(std::vector<uint8_t> &buffer, IMPEncoderStream &stream);

    bool listEmpty();

    IMPEncoderPack getFrame();

    void static setNightVision(bool state);

    void writeBufferToDisk();
private:
    int encoderMode;



    int system_init();


    int system_exit();

    int framesource_streamon();

    int framesource_streamoff();

    int framesource_init();

    int framesource_exit();

    int encoder_init(int, int, int, int);

    int jpeg_init();

    int encoder_exit(void);

    int osd_exit(IMPRgnHandle *prHandle, int grpNum);

    IMPSensorInfo sensor_info;

    impParams currentParams;

    int encoder_chn_exit(int encChn);

    chn_conf m_chn;

    pthread_mutex_t m_mutex;

    int getSensorName();


};



// Operations on timespecs
#define  MS_IN_NS 1000000
#define timespecclear(tvp)      ((tvp)->tv_sec = (tvp)->tv_nsec = 0)
#define timespecisset(tvp)      ((tvp)->tv_sec || (tvp)->tv_nsec)
#define timespeccmp(tvp, uvp, cmp)                                      \
        (((tvp)->tv_sec == (uvp)->tv_sec) ?                             \
            ((tvp)->tv_nsec cmp (uvp)->tv_nsec) :                       \
            ((tvp)->tv_sec cmp (uvp)->tv_sec))

#define timespecadd(vvp, uvp)                                           \
        do {                                                            \
                (vvp)->tv_sec += (uvp)->tv_sec;                         \
                (vvp)->tv_nsec += (uvp)->tv_nsec;                       \
               if ((vvp)->tv_nsec >= 1000000000) {                     \
                        (vvp)->tv_sec++;                                \
                        (vvp)->tv_nsec -= 1000000000;                   \
                }                                                       \
        } while (0)

#define timespecsub(vvp, uvp)                                           \
        do {                                                            \
                (vvp)->tv_sec -= (uvp)->tv_sec;                         \
                (vvp)->tv_nsec -= (uvp)->tv_nsec;                       \
                if ((vvp)->tv_nsec < 0) {                               \
                        (vvp)->tv_sec--;                                \
                        (vvp)->tv_nsec += 1000000000;                   \
                }                                                       \
        } while (0)
#endif
