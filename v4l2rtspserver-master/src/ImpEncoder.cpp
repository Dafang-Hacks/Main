#define LOGURU_WITH_STREAMS 1
#include <loguru.hpp>

#include <stdio.h>

#include <imp/imp_log.h>
#include <imp/imp_common.h>
#include <imp/imp_system.h>
#include <imp/imp_framesource.h>
#include <imp/imp_encoder.h>
#include <imp/imp_ivs.h>
#include <imp/imp_ivs_move.h>
#include <imp/imp_encoder.h>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <signal.h>
#include "ImpEncoder.h"
#include "INIReader.h"
#include "OSD.hpp"
#include "sharedmem.h"

#include <stdexcept>
#include <tuple>

#include <mosquitto.h>

extern "C" {
struct mosquitto *mosq_cli = NULL;

extern int IMP_OSD_SetPoolSize(int newPoolSize);
extern int IMP_Encoder_SetPoolSize(int newPoolSize0);
extern int IMP_OSD_GetPoolSize();
extern int IMP_Encoder_GetPoolSize();
}

static bool m_osdOn = true;
static bool m_jpegOn = true;
static bool m_motionOn = true;

static char *fontMono = NULL;
static char *fontSans = NULL;
static int image_width;
static int image_height;

static bool gDetectionOn = false;
static bool ismotionActivated = true;

static char *detectionScriptOn = NULL;
static char *detectionScriptOff = NULL;
static char *detectionTracking= NULL;
static IMPIVSInterface *interface = NULL;

// Activate or not tracking
bool isMotionTracking = false;
int gNbRegions  = 0;
int motionTimeout = -1; // -1 is for deactivation

static int ivsMoveStart(int grp_num, int chn_num, IMPIVSInterface **interface, int nbRegions, int val[], int width, int height );
static void *ivsMoveDetectionThread(void *arg);

extern "C" {
static void mqtt_pub(const char *topic, const char *payload) {
    if (!mosq_cli) return;
    /* if I lost my connection, reconnect and re-try */
    if(mosquitto_publish(mosq_cli, NULL, topic, strlen(payload), payload, 0, false) == MOSQ_ERR_NO_CONN) {
        mosquitto_reconnect(mosq_cli);
        mosquitto_publish(mosq_cli, NULL, topic, strlen(payload), payload, 0, false);
    }
}

static void mqtt_conn(void) {
    /* TODO: err msgs */
    mosq_cli = mosquitto_new(NULL, true, NULL);
    if(mosq_cli != NULL) {
        if(mosquitto_connect(mosq_cli, "127.0.0.1", 1883, 3600) == MOSQ_ERR_SUCCESS) {
            mqtt_pub("rtsp/server", "ON");
            mqtt_pub("rtsp/motion/state/detect", ismotionActivated ? "ON" : "OFF");
        } else mosq_cli = NULL;
    }
}

static void mqtt_disc(void) {
    if(mosq_cli != NULL) {
        mqtt_pub("rtsp/server", "OFF");
        mosquitto_disconnect(mosq_cli);
        mosquitto_destroy(mosq_cli);
    }
}
}

static int ivsSetsensitivity(int nbRegion, int sens)
{
	int ret = 0;
	gNbRegions = nbRegion;
	IMP_IVS_MoveParam param;
	if (isMotionTracking == true)
	{
	    gNbRegions = 4;
	}

	for (int i =0 ; i<gNbRegions; i++)
	{
        ret = IMP_IVS_GetParam(0, &param);
        if (ret < 0) {
            LOG_S(ERROR) << "IMP_IVS_GetParam(0) failed";
            return -1;
        }

        param.sense[i] = sens;
        LOG_S(INFO) << "Set sensitivity to region " << i << " to " << sens;

        ret = IMP_IVS_SetParam(0, &param);
        if (ret < 0) {
            LOG_S(ERROR) << "IMP_IVS_SetParam(0) failed";
            return -1;
        }
    }
	return 0;
}

static int ivsSetDetectionRegion(int nbRegion, int detectionRegion[] )
{
	int ret = 0;
	gNbRegions = nbRegion;
    ret = ivsMoveStart(0, 0, &interface, nbRegion, detectionRegion,image_width,image_height) ;
    if (ret < 0) {
        LOG_S(ERROR) << "ivsMoveStart failed";
    }
    pthread_t tid;
    //  start to get ivs move result
    if (pthread_create(&tid, NULL, ivsMoveDetectionThread, NULL)) {
        LOG_S(ERROR) << "create ivs_move_get_result_process failed";
    }
  	return 0;
}


static void* update_thread(void *p) {
    loguru::set_thread_name("update_thread");

    std::vector<uint8_t> jpeg_buffer;
    FT_Library library = NULL;
    FT_Face face = NULL;
    int font_baseline_offset = 0;

    if (m_osdOn == true) {

        if (FT_Init_FreeType(&library) != 0) {
            LOG_S(ERROR) << "Could not initialize FreeType";
            return NULL;
        }

        FT_UInt hinting_engine = FT_HINTING_ADOBE;

        if (FT_Property_Set(library, "cff", "hinting-engine", &hinting_engine) != 0) {
            LOG_S(ERROR) << "Could not set hinting engine";
            return NULL;
        }
    }
    bool firstConfigPass = true;
    bool alreadySetDetectionRegion = false;

    SharedMem &sharedMem = SharedMem::instance();
    shared_conf *newConfig = sharedMem.getConfig();
    shared_conf currentConfig = {0};
    OSD *motion_osd = NULL;
    OSD *timestamp_osd = NULL;
    // Force initializing
    currentConfig.sensitivity = -2;
    if (m_osdOn == true) {
        // Move it to the top right of the screen
        motion_osd = new OSD(image_width - DETECTION_CIRCLE_SIZE, 0, DETECTION_CIRCLE_SIZE, DETECTION_CIRCLE_SIZE, 0);

        // Default to top left and 10px high until we read the config
        timestamp_osd = new OSD(0, 0, image_width, 10, 1);

        if (IMP_OSD_Start(0) != 0) {
            LOG_S(ERROR) << "OSD show error";
            return NULL;
        }

        motion_osd->show(true);
        timestamp_osd->show(true);
    }
    while (true) {
        sharedMem.readConfig();

        // Update the settings
        if (currentConfig.flip != newConfig->flip) {
            LOG_S(INFO) << "Changed FLIP";
            if (newConfig->flip == 1) {
                IMP_ISP_Tuning_SetISPVflip(IMPISP_TUNING_OPS_MODE_ENABLE);
                IMP_ISP_Tuning_SetISPHflip(IMPISP_TUNING_OPS_MODE_ENABLE);
            } else {
                IMP_ISP_Tuning_SetISPVflip(IMPISP_TUNING_OPS_MODE_DISABLE);
                IMP_ISP_Tuning_SetISPHflip(IMPISP_TUNING_OPS_MODE_DISABLE);
            }
        }

        if (currentConfig.nightmode != newConfig->nightmode) {
            LOG_S(INFO) << "Changed NIGHTVISION";
            ImpEncoder::setNightVision(newConfig->nightmode);
        }

        if (currentConfig.bitrate != newConfig->bitrate) {
            LOG_S(INFO) << "Attempt to changed Bitrate to " << newConfig->bitrate;
            IMPEncoderAttrRcMode attr;
            int ret = IMP_Encoder_GetChnAttrRcMode(0, &attr);
            if (ret < 0) {
                LOG_S(INFO) << "Unable to get param to change the bitrate";
            }
            switch (attr.rcMode) {
                case ENC_RC_MODE_SMART:
                    LOG_S(INFO) << "Setting SMART maxBitrate.";
                    attr.attrH264Smart.maxBitRate = (uint)newConfig->bitrate;
                    break;

                case ENC_RC_MODE_CBR:
                    LOG_S(INFO) << "Setting CBR outBitrate.";
                    attr.attrH264Cbr.outBitRate = (uint)newConfig->bitrate;
                    break;

                case ENC_RC_MODE_VBR:
                    LOG_S(INFO) << "Setting VBR maxBitrate.";
                    attr.attrH264Vbr.maxBitRate = (uint)newConfig->bitrate;
                    break;

                default:
                    LOG_S(INFO) << "Bitrate does not apply to rcmode " << attr.rcMode;
                    break;
            }
            IMP_Encoder_SetChnAttrRcMode(0, &attr);
            if (ret < 0) {
                LOG_S(INFO) << "Unable to change the bitrate";
            }

        }

/*        if ((currentConfig.frmRateConfig[0] != newConfig->frmRateConfig[0]) || (currentConfig.frmRateConfig[1] != newConfig->frmRateConfig[1])) {
            IMPEncoderFrmRate rate = {0};
            LOG_S(INFO) << "Attempt to change fps to " << newConfig->frmRateConfig[0] << "," << newConfig->frmRateConfig[1];
            rate.frmRateNum = newConfig->frmRateConfig[0];
            rate.frmRateDen = newConfig->frmRateConfig[1];
	        int ret = IMP_ISP_Tuning_SetSensorFPS(newConfig->frmRateConfig[0],newConfig->frmRateConfig[1]);
            //int ret = IMP_Encoder_SetChnFrmRate(0, &rate);

            if (ret != 0) {
                LOG_S(ERROR) << "IMP_ISP_Tuning_SetSensorFPS error:" << ret;
            }
        }*/

        if (m_osdOn == true) {
            // Remap the old pre-defined color values
            if (newConfig->osdColor == 0)       newConfig->osdColor = RGBAColor::WHITE;
            else if (newConfig->osdColor == 1)  newConfig->osdColor = RGBAColor::BLACK;
            else if (newConfig->osdColor == 2)  newConfig->osdColor = RGBAColor::RED;
            else if (newConfig->osdColor == 3)  newConfig->osdColor = RGBAColor::GREEN;
            else if (newConfig->osdColor == 4)  newConfig->osdColor = RGBAColor::BLUE;
            else if (newConfig->osdColor == 5)  newConfig->osdColor = RGBAColor::CYAN;
            else if (newConfig->osdColor == 6)  newConfig->osdColor = RGBAColor::YELLOW;
            else if (newConfig->osdColor == 7)  newConfig->osdColor = RGBAColor::MAGENTA;

            if (firstConfigPass || (currentConfig.osdFixedWidth != newConfig->osdFixedWidth)
                                || strcmp(currentConfig.osdFontName, newConfig->osdFontName) != 0) {
                int result;

                if (newConfig->osdFontName[0] != 0) {
                    LOG_S(INFO) << "Font name:" << newConfig->osdFontName;
                    FT_Done_Face(face);
                    result = FT_New_Face(library, newConfig->osdFontName, 0, &face);
                } else if (newConfig->osdFixedWidth) {
                    LOG_S(INFO) << "Font name:" << fontMono;
                    FT_Done_Face(face);
                    result = FT_New_Face(library, fontMono, 0, &face);
                } else {
                    LOG_S(INFO) << "Font name:" << fontSans;
                    FT_Done_Face(face);
                    result = FT_New_Face(library, fontSans, 0, &face);
                }

                if (result != 0) {
                    LOG_S(ERROR) << "Could not load or parse the font file";
                }

                // to trigger OSD resize
                firstConfigPass = true;

                LOG_S(INFO) << "Changed OSD font";
            }

            if (currentConfig.osdPosY != newConfig->osdPosY) {
                timestamp_osd->setBounds(timestamp_osd->getX(), newConfig->osdPosY, timestamp_osd->getWidth(), timestamp_osd->getHeight());

                // As the size changed, re-display the OSD
                LOG_S(INFO) <<  "Changed OSD y-offset";

                // to trigger OSD resize
                firstConfigPass = true;
            }

            // Old interface specify 0 for "small" font
            if (newConfig->osdSize == 0) {
                newConfig->osdSize = 18;
            // and 1 for "bigger" font
            } else if (newConfig->osdSize == 1) {
                newConfig->osdSize = 40;
            }

            if (firstConfigPass || (currentConfig.osdSize != newConfig->osdSize)) {
                if (FT_Set_Char_Size(face, 0, newConfig->osdSize * 64, 100, 100) != 0) {
                    LOG_S(ERROR) << "Could not set font size";
                }

                int font_height;
                std::tie(font_height, font_baseline_offset) = get_vertical_font_dimensions(face);
                timestamp_osd->setBounds(timestamp_osd->getX(), timestamp_osd->getY(), image_width, font_height);

                LOG_S(INFO) << "Max font bitmap height is " << font_height << " and baseline offset is " << font_baseline_offset;

                LOG_S(INFO) << "Changed OSD size";
            }
        }

        if (m_motionOn == true) {

            if (currentConfig.motionTracking != newConfig->motionTracking ) {
                isMotionTracking = newConfig->motionTracking;
                if (isMotionTracking == true) {
                    LOG_S(INFO) << "Tracking set to On";
                    if (alreadySetDetectionRegion == false)
                    {
                        alreadySetDetectionRegion = true;
                        ivsSetsensitivity(newConfig->nbDetectionRegion, newConfig->sensitivity);
                        ivsSetDetectionRegion(newConfig->nbDetectionRegion, newConfig->detectionRegion);
                    }
                } else {
                        LOG_S(INFO) << "Tracking set to Off";
                }
            }

            if (currentConfig.sensitivity != newConfig->sensitivity) {
                if (newConfig->sensitivity == -1) {
                    ismotionActivated = false;
                    LOG_S(INFO) << "Deactivate motion";
                    mqtt_pub("rtsp/motion/state/detect", "OFF");
                } else {
                    ismotionActivated = true;
                    mqtt_pub("rtsp/motion/state/detect", "ON");
                    LOG_S(INFO) << "Changed motion sensitivity=" << newConfig->sensitivity ;
                    if (alreadySetDetectionRegion == false) {
                        alreadySetDetectionRegion = true;
                        ivsSetDetectionRegion(newConfig->nbDetectionRegion, newConfig->detectionRegion);
                        LOG_S(INFO) << "Changed motion region";
                    }

                    ivsSetsensitivity(newConfig->nbDetectionRegion, newConfig->sensitivity);
                }
            }
        }

        if ((m_osdOn == true) && (m_motionOn == true)) {
            // Remap the old pre-defined color values
            if (newConfig->motionOSD == 0)       newConfig->motionOSD = RGBAColor::WHITE;
            else if (newConfig->motionOSD == 1)  newConfig->motionOSD = RGBAColor::BLACK;
            else if (newConfig->motionOSD == 2)  newConfig->motionOSD = RGBAColor::RED;
            else if (newConfig->motionOSD == 3)  newConfig->motionOSD = RGBAColor::GREEN;
            else if (newConfig->motionOSD == 4)  newConfig->motionOSD = RGBAColor::BLUE;
            else if (newConfig->motionOSD == 5)  newConfig->motionOSD = RGBAColor::CYAN;
            else if (newConfig->motionOSD == 6)  newConfig->motionOSD = RGBAColor::YELLOW;
            else if (newConfig->motionOSD == 7)  newConfig->motionOSD = RGBAColor::MAGENTA;

            if (currentConfig.motionOSD != newConfig->motionOSD) {
                LOG_S(INFO) << "Display motion OSD color=" << newConfig->motionOSD;
            }
        }
        if (m_motionOn == true) {
            if (newConfig->motionTimeout > 0) {
                motionTimeout = newConfig->motionTimeout;
            }
        }

        memcpy(&currentConfig, newConfig, sizeof(shared_conf));
        strcpy(currentConfig.osdTimeDisplay, newConfig->osdTimeDisplay);

        if (firstConfigPass) {
            firstConfigPass = false;
        }

        // Read the current time
        struct timespec spec;
        clock_gettime(CLOCK_REALTIME, &spec);

        // Sleep until just a little after the next second
        spec.tv_sec = 0;
        spec.tv_nsec = 1010000000L - spec.tv_nsec;

        nanosleep(&spec, NULL);

        if (m_osdOn == true) {
            // Draw the timestamp OSD
            osd_draw_timestamp(*timestamp_osd, face, font_baseline_offset, currentConfig);
            // Draw the motion detection circle
            if (currentConfig.motionOSD != -1) {
                osd_draw_detection_circle(*motion_osd, gDetectionOn, currentConfig);
            }
        }

        if (m_jpegOn == true) {
            // Dump the JPEG once every second
            jpeg_buffer.clear();

            try {
                ImpEncoder::snap_jpeg(jpeg_buffer);
                sharedMem.copyImage(jpeg_buffer.data(), jpeg_buffer.size());
            } catch (const std::runtime_error &e) {
                LOG_S(INFO) << "Failed to read the JPEG stream: " << e.what();
            }
        }
    }


    return NULL;
}

#if MOTION_SYSTEM_CALLS
static int file_exist(const char *filename)
{
  FILE *f = fopen(filename,"r");
  if (f == NULL)
    return 0;
   fclose(f);
   return 1;
}

static void exec_command(const char *command, char param[4][2])
{
     if (file_exist(command))
     {
      if (param == NULL) {
         LOG_S(INFO) << "Will execute command " << command;
         int retVal =  system(command);
         LOG_S(INFO) << "Execute " << command << " returned:" << retVal;
      } else {
         char exe[256] = {0};
         snprintf(exe, sizeof(exe), "%s %s %s %s %s", command, param[0],param[1],param[2],param[3]);

         LOG_S(INFO) << "Will execute command " << exe;
         int retVal =  system(exe);
         LOG_S(INFO) << "Execute " << exe << " returned:" << retVal;
      }
     }
     else
     {
         LOG_S(INFO) << "command " << command << " does not exist\n";
     }

}
#else /* !MOTION_SYSTEM_CALLS */
static void exec_command(const char *command, char param[4][2]) { ; /* do nothing */ }
#endif /* MOTION_SYSTEM_CALLS */

static int ivsMoveStart(int grp_num, int chn_num, IMPIVSInterface **interface, int nbRegions, int val[], int width, int height )
{
    int ret = 0;
    IMP_IVS_MoveParam param;
    memset(&param, 0, sizeof(IMP_IVS_MoveParam));
    // Skip to 50 avoid detection at startup, not sure it impacts when running
    param.skipFrameCnt = 50;
    param.frameInfo.width = width;
    param.frameInfo.height = height;
    if (isMotionTracking == true) {
        // use the 4 regions
        param.roiRectCnt = 4;
        // Sensitivity (0 to 4)
        param.sense[0] = 4;
        param.sense[1] = 4;
        param.sense[2] = 4;
        param.sense[3] = 4;

        // 0,0 +--------------------+-----------------+
        //     |                    |                 |
        //     |           0        |      1          |
        //     |                    |                 |
        // h/2 +--------------------+-----------------+
        //     |                    |                 |
        //     |          2         |      3          |
        //     |                    |                 |
        // h   +--------------------+-----------------+
        //                          w/2               w

        // Region 0
        param.roiRect[0].p0.x = 0;
        param.roiRect[0].p0.y = 0;
        param.roiRect[0].p1.x = (width/2)- 1;
        param.roiRect[0].p1.y = (height/2) - 1;

        // Region 1
        param.roiRect[1].p0.x = (width/2);
        param.roiRect[1].p0.y = 0;
        param.roiRect[1].p1.x = (width)- 1;
        param.roiRect[1].p1.y = (height/2) - 1;

        // Region 2
        param.roiRect[2].p0.x = 0;
        param.roiRect[2].p0.y = (height/2);
        param.roiRect[2].p1.x = (width/2)- 1;
        param.roiRect[2].p1.y = (height) - 1;

        // Region 3
        param.roiRect[3].p0.x = width/2;
        param.roiRect[3].p0.y = (height/2);
        param.roiRect[3].p1.x = width-1;
        param.roiRect[3].p1.y = (height) - 1;

        LOG_S(INFO) << "Detection region for motion tracking\n";
    }
    else
    {
       // Define the detection region, for now only one of the size of the video
        param.roiRectCnt = nbRegions;
        LOG_S(INFO) << "Number of detection region=" << nbRegions;
        for (int i=0, j=0; i< nbRegions*4; i+=4, j++)
        {
            // Sensitivity (0 to 4)
            param.sense[j] = 4;
            param.roiRect[j].p0.x = val[i];
            param.roiRect[j].p0.y = val[i+1];
            if (val[i+2] == 0 && val[i+3] == 0)
            {
                param.roiRect[j].p1.x = width - 1;
                param.roiRect[j].p1.y = height  - 1;
            } else {
                param.roiRect[j].p1.x = val[i+2] - 1;
                param.roiRect[j].p1.y = val[i+3] - 1;

            }
        }
        for (int i=0; i< nbRegions; i++)
        {
            LOG_S(INFO) << "Detection region= ((" << param.roiRect[i].p0.x << "," << param.roiRect[i].p0.y << ")-("<< param.roiRect[i].p1.x << "," << param.roiRect[i].p1.y << "))";
        }
    }
    if ( nbRegions > 0)
    {
        *interface = IMP_IVS_CreateMoveInterface(&param);
        if (*interface == NULL) {
            LOG_S(ERROR) << "IMP_IVS_CreateGroup(0) failed";
            return -1;
        }

        ret = IMP_IVS_CreateChn(chn_num, *interface);
        if (ret < 0) {
            LOG_S(ERROR) << "IMP_IVS_CreateChn(" << chn_num << ") failed";
            return -1;
        }

        ret = IMP_IVS_RegisterChn(grp_num, chn_num);
        if (ret < 0) {
            LOG_S(ERROR) << "IMP_IVS_RegisterChn(" << grp_num << "," << chn_num << "failed";
            return -1;
        }

        ret = IMP_IVS_StartRecvPic(chn_num);
        if (ret < 0) {
            LOG_S(ERROR) << "IMP_IVS_StartRecvPic(" << chn_num << ") failed";
            return -1;
        }
    }
    return 0;
}

static void endofmotion(int sig)
{
    // In case of inactivity execute the script with no arguments
    exec_command(detectionTracking, NULL);
    mqtt_pub("rtsp/motion/track", "OFF");
    LOG_S(INFO) << "End of motion";
}


static void *ivsMoveDetectionThread(void *arg)
{
    int ret = 0;
    int chn_num = 0; 
    IMP_IVS_MoveOutput *result = NULL;
    bool isWasOn[IMP_IVS_MOVE_MAX_ROI_CNT] = {false};

    loguru::set_thread_name("ivsMoveDetectionThread");

    while (1)
    {
        if (ismotionActivated == true)
        {
            ret = IMP_IVS_PollingResult(chn_num, IMP_IVS_DEFAULT_TIMEOUTMS);
            if (ret < 0)
            {
                LOG_S(ERROR) << "IMP_IVS_PollingResult("<<chn_num << "," << IMP_IVS_DEFAULT_TIMEOUTMS<< ") failed";
                usleep(1000);
                continue;
            }

            ret = IMP_IVS_GetResult(chn_num, (void **)&result);
            if (ret < 0)
            {
                LOG_S(ERROR) << "IMP_IVS_GetResult(" << chn_num << ") failed";
                usleep(1000);
                continue;
            }

            if (isMotionTracking == true)
            {
               if (result->retRoi[0] == 1 ||
                   result->retRoi[1] == 1 ||
                   result->retRoi[2] == 1 ||
                   result->retRoi[3] == 1) {
                        char param[4][2] = {};
                        char mqtt_payload[16];
                        isWasOn[0] = true;
                        gDetectionOn = true;

                       snprintf(param[0], sizeof(param[0]), "%.1d", result->retRoi[0]);
                       snprintf(param[1], sizeof(param[1]), "%.1d", result->retRoi[1]);
                       snprintf(param[2], sizeof(param[2]), "%.1d", result->retRoi[2]);
                       snprintf(param[3], sizeof(param[3]), "%.1d", result->retRoi[3]);
                       snprintf(mqtt_payload, sizeof(mqtt_payload), "%.1d,%.1d,%.1d,%.1d", result->retRoi[0], result->retRoi[1], result->retRoi[2], result->retRoi[3]);

                       exec_command(detectionTracking, param);
                        mqtt_pub("rtsp/motion/track", mqtt_payload);
                       exec_command(detectionScriptOn, NULL);
                        mqtt_pub("rtsp/motion/detect", "ON");

                       if (motionTimeout != -1)
                       {
                            signal(SIGALRM, endofmotion);
                            alarm(motionTimeout);
                       }
                }
                else
                {
                    if (isWasOn[0]  == true) {
                        exec_command(detectionScriptOff, NULL);
                        mqtt_pub("rtsp/motion/detect", "OFF");
                    }
                    gDetectionOn = false;
                    isWasOn[0]  = false;
                }
            }
            else
            {
                bool needToExecuteFinished = true;
                for (int i=0; i < gNbRegions ; i++) {
                    if ((isWasOn[i] == false) && (result->retRoi[i] == 1)) {
                        LOG_S(INFO) << "Detection on area " << i;

                        isWasOn[i] = true;

                        if (gDetectionOn == false) {
                            gDetectionOn = true;
                            exec_command(detectionScriptOn, NULL);
                            mqtt_pub("rtsp/motion/detect", "ON");
                            // Keep on the loop to display logs on the detected area
                        }
                        // Set this flag to avoid executing the detectionOff script on the same loop
                        needToExecuteFinished = false;
                    } else {
                        // to execute detection finished script, all area should be off
                        if ((isWasOn[i] == true) && (result->retRoi[i] == 0)) {
                            isWasOn[i] = false;
                            needToExecuteFinished = false;
                            LOG_S(INFO) << "Detect finished on area " << i;
                        }
                    }
                }

                if (gDetectionOn == true) {
                    IMPEncoderStream stream;

                    if (IMP_Encoder_PollingStream(1, 1000) == 0 && IMP_Encoder_GetStream(1, &stream, 1) == 0) {
                        for (unsigned int i = 0; i < stream.packCount; i++) {
                            IMPEncoderPack &packet = stream.pack[i];

                            LOG_S(INFO) << "MQTT send snap #" << i << " size=" << packet.length;
                            mosquitto_publish(mosq_cli, NULL, "rtsp/motion/detect/snap", packet.length, (void *)packet.virAddr, 0, false);
                        }
                        IMP_Encoder_ReleaseStream(1, &stream);
                    }

                    if (needToExecuteFinished == true) {
                        LOG_S(INFO) << "Detect finished on all area !!";
                        gDetectionOn = false;
                        exec_command(detectionScriptOff, NULL);
                        mqtt_pub("rtsp/motion/detect", "OFF");
                    }
                }

                ret = IMP_IVS_ReleaseResult(chn_num, (void *)result);
                if (ret < 0) {
                    LOG_S(ERROR) << "IMP_IVS_ReleaseResult("<< chn_num << ") failed";
                    return (void *)-1;
                }
            }
        }
        else
        {
            sleep(1);
        }
    }
    return (void *)0;
}
ImpEncoder::ImpEncoder(impParams params) {
    currentParams = params;

    int skiptype = 0;
    int quality = 0;
    int maxgop = 25;
    int maxSameSceneCnt = 0;
    IMPVersion pstVersion = {};
    int ret;
    bool reducePoolSize = false;
    unsigned long reducePoolSizeBuffer = 0;

    // Ini file to override some path when /system/sdcard won't exit

    char dirNameBuffer[PATH_MAX + 1] = {0};
    // Read the symbolic link '/proc/self/exe'.
    const char *linkName = "/proc/self/exe";
    readlink(linkName, dirNameBuffer, sizeof(dirNameBuffer) - 1);

    // Read the same exe file + ini
    strncat(dirNameBuffer, ".ini", sizeof(dirNameBuffer) - 1);
    LOG_S(INFO) << "Try to read extra configuration on " << dirNameBuffer;
    INIReader reader(dirNameBuffer);
    if (reader.ParseError() >= 0) 
    {
        LOG_S(INFO) << dirNameBuffer <<  "not found: set default values";
    	m_motionOn = true;
        m_osdOn = true;
        m_jpegOn = true;
        fontMono = strdup("/system/sdcard/fonts/NotoMono-Regular.ttf");
        fontSans = strdup("/system/sdcard/fonts/NotoSans-Regular.ttf");
        detectionScriptOn = strdup( "/system/sdcard/scripts/detectionOn.sh");
        detectionScriptOff = strdup( "/system/sdcard/scripts/detectionOff.sh");
        detectionTracking = strdup( "/system/sdcard/scripts/detectionTracking.sh");
	    reducePoolSize = false;
	    reducePoolSizeBuffer = 0;
    }
    else 
    {
        LOG_S(INFO) << "Parsing " <<  dirNameBuffer ;
        m_motionOn = reader.GetBoolean("Configuration","MOTION",true);
        m_osdOn = reader.GetBoolean("Configuration","OSD",true);
        m_jpegOn = reader.GetBoolean("Configuration","JPEG",true);
        reducePoolSize = reader.GetBoolean("Configuration","POOLSIZE", false);
	    reducePoolSizeBuffer = reader.GetInteger("Configuration", "POOLSIZEBUFFER",0x64000);
        if (m_osdOn == true)
        {
            LOG_S(INFO) << "OSD activated";
            fontMono = strdup(reader.Get("Configuration", "FontFixedWidth", "").c_str());
            fontSans = strdup(reader.Get("Configuration", "FontRegular", "").c_str());
        } 
        else
        {
            LOG_S(INFO) << "OSD deactivated";
        }

        if (m_motionOn == true)
        {
            LOG_S(INFO) << "Motion activated";
            detectionScriptOn = strdup(reader.Get("Configuration", "DetectionScriptOn", "").c_str());
            detectionScriptOff = strdup(reader.Get("Configuration", "DetectionScriptOff", "").c_str());
            detectionTracking = strdup(reader.Get("Configuration", "DetectionTracking", "").c_str());
        }
        else
        {
            LOG_S(INFO) << "Motion deactivated";
        }

        if (m_jpegOn == true)
        {
            LOG_S(INFO) << "JPEG capture activated";
        }
        else
        {
            LOG_S(INFO) << "JPEG capture deactivated";
        }
        if ( reducePoolSize == true)
        {
            LOG_S(INFO) << "Reduce pool size activated" << " Buffer size=" << reducePoolSizeBuffer;
        }
        else
        {
            LOG_S(INFO) << "Reduce pool size deactivated";
        }

        skiptype = reader.GetInteger("Video", "SkipType", 0);
        quality = reader.GetInteger("Video", "Quality", 0);
        maxSameSceneCnt = reader.GetInteger("Video", "maxSameSceneCnt", 1);
        maxgop = reader.GetInteger("Video", "maxGop", 8);
        LOG_S(INFO) << "Video settings: skip:" << skiptype << " quality:" << quality << " maxSameSceneCnt:" << maxSameSceneCnt << " MaxGop:" << maxgop;
     }

    SharedMem &sharedMem = SharedMem::instance();
   	shared_conf *newConfig = sharedMem.getConfig();

    IMP_System_GetVersion(&pstVersion);
    LOG_S(INFO) << "IMP Lib version" << pstVersion.aVersion;

    // Init Structure:
    memset(&m_chn, 0, sizeof(chn_conf));

    m_chn.index = 0;
    m_chn.enable = 1;
    m_chn.fs_chn_attr.pixFmt = PIX_FMT_NV12;
    m_chn.fs_chn_attr.outFrmRateNum = newConfig->frmRateConfig[0];
    m_chn.fs_chn_attr.outFrmRateDen = newConfig->frmRateConfig[1];
    if ( reducePoolSize == true)
    {
        m_chn.fs_chn_attr.nrVBs = 2;
    }
    else
    {
        m_chn.fs_chn_attr.nrVBs = 3;
    }
    m_chn.fs_chn_attr.type = FS_PHY_CHANNEL;

    m_chn.fs_chn_attr.crop.enable = 0;
    m_chn.fs_chn_attr.crop.width = currentParams.width;
    m_chn.fs_chn_attr.crop.height = currentParams.height;
    m_chn.fs_chn_attr.crop.top = 0;
    m_chn.fs_chn_attr.crop.left = 0;

    m_chn.fs_chn_attr.scaler.enable = 1;
    m_chn.fs_chn_attr.scaler.outwidth = currentParams.width;
    m_chn.fs_chn_attr.scaler.outheight = currentParams.height;


    m_chn.fs_chn_attr.picWidth = currentParams.width;
    m_chn.fs_chn_attr.picHeight = currentParams.height;

    m_chn.framesource_chn.deviceID = DEV_ID_FS;
    m_chn.framesource_chn.groupID = 0;
    m_chn.framesource_chn.outputID = 0;

    m_chn.imp_encoder.deviceID = DEV_ID_ENC;
    m_chn.imp_encoder.groupID = 0;
    m_chn.imp_encoder.outputID = 0;

    if (m_osdOn == true) {
        m_chn.OSD_Cell.deviceID = DEV_ID_OSD;
        m_chn.OSD_Cell.groupID = 0;
        m_chn.OSD_Cell.outputID = 0;
    }
    encoderMode = currentParams.rcmode;


    if (reducePoolSize == true)
    {
        // undocumented functions to increase pool size
	    // See https://github.com/geekman/t20-rtspd/blob/master/capture_and_encoding.cpp
	    IMP_OSD_SetPoolSize(reducePoolSizeBuffer);
	    IMP_Encoder_SetPoolSize(0x100000);
    }

    /* Step.1 System init */
    ret = system_init();
    if (ret < 0) {
        LOG_S(ERROR) <<"IMP_System_Init() failed";
    }

    /* Step.2 FrameSource init */
    ret = framesource_init();
    if (ret < 0) {
        LOG_S(ERROR) << "FrameSource init failed";
    }
    LOG_S(INFO) << "Set FPS to " << newConfig->frmRateConfig[0] << "/" << newConfig->frmRateConfig[1];
    ret = IMP_ISP_Tuning_SetSensorFPS(newConfig->frmRateConfig[0], newConfig->frmRateConfig[1]);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_ISP_Tuning_SetSensorFPS failed";
    }

    ret = IMP_ISP_Tuning_SetISPBypass(IMPISP_TUNING_OPS_MODE_ENABLE);
    if (ret < 0) {
	    LOG_S(ERROR) << "IMP_ISP_Tuning_SetISPBypass failed";
    }


    image_width = currentParams.width;
    image_height = currentParams.height;

    ret = IMP_Encoder_CreateGroup(0);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_CreateGroup(0) error !";

    }

    if (m_jpegOn == true) {
        ret = IMP_Encoder_CreateGroup(1);
        if (ret < 0) {
            LOG_S(ERROR) << "IMP_Encoder_CreateGroup(1) error !";
        }
        /* Step.3 Encoder init */
        ret = jpeg_init();
        if (ret < 0) {
            LOG_S(ERROR) << "Encoder JPEG init failed";

        }
    }
    /* Step.3 Encoder init */
    ret = encoder_init(quality, skiptype, maxSameSceneCnt, maxgop);
    if (ret < 0) {
        LOG_S(ERROR) << "Encoder h264 init failed";

    }

    if (m_osdOn == true) {
        // ----- OSD implementation: Init
        //
        if (IMP_OSD_CreateGroup(0) != 0) {
           LOG_S(ERROR) << "IMP_OSD_CreateGroup(0) error !";
        }

        /* Step Bind */
        ret = IMP_System_Bind(&m_chn.framesource_chn, &m_chn.OSD_Cell);
        if (ret < 0) {
            LOG_S(ERROR) << "Bind FrameSource channel0 and OSD failed";
        }

        ret = IMP_System_Bind(&m_chn.OSD_Cell, &m_chn.imp_encoder);
        if (ret < 0) {
            LOG_S(ERROR) << "Bind OSD and Encoder failed";
        }
    }
    /* Step Bind */
    ret = IMP_System_Bind(&m_chn.framesource_chn, &m_chn.imp_encoder);
    if (ret < 0) {
        LOG_S(ERROR) << "Bind FrameSource channel0 and encoder failed";
    }

    if (m_motionOn == true) {
        // ----- Motion implementation: Init

        IMPCell ivs_grp0 = { DEV_ID_IVS , 0, 0};
        ret = IMP_IVS_CreateGroup(0);
        if (ret < 0) {
            LOG_S(ERROR) << "IMP_IVS_CreateGroup(0) failed";
        }

        ret = IMP_System_Bind (&m_chn.framesource_chn, &ivs_grp0);
        if (ret < 0) {
            LOG_S(ERROR) << "IMP_System_Bind";
        }
    }

    // --- OSD and other stuffs thread
    pthread_t tid;
    ret = pthread_create(&tid, NULL, update_thread, NULL);
    sleep(0);
    if (ret) {
        LOG_S(ERROR) << "thread create error";
    }

    ret = framesource_streamon();
    if (ret < 0) {
        LOG_S(ERROR) << "ImpStreamOn failed";

    }

    /* drop several pictures of invalid data */
    nanosleep((const struct timespec[]){{0, 500000000L}}, NULL);
    if (m_jpegOn == true) {
        // JPEG
        ret = IMP_Encoder_StartRecvPic(1);
        if (ret < 0) {
            LOG_S(ERROR) << "IMP_Encoder_StartRecvPic(2) failed";
        }
    }
    // H264
    ret = IMP_Encoder_StartRecvPic(0);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_StartRecvPic(0) failed";
    }



    memset(&m_mutex, 0, sizeof(m_mutex));
    pthread_mutex_init(&m_mutex, NULL);


}


ImpEncoder::~ImpEncoder() {
    int ret;

    /* Step.b UnBind */
    ret = IMP_Encoder_StopRecvPic(1);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_StopRecvPic() failed";

    }

    ret = IMP_Encoder_StopRecvPic(0);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_StopRecvPic() failed";

    }

    /* Exit sequence as follow... */
    /* Step.a Stream Off */
    ret = framesource_streamoff();
    if (ret < 0) {
        LOG_S(ERROR) << "FrameSource StreamOff failed";

    }

    /* Step.b UnBind */

    ret = IMP_System_UnBind(&m_chn.framesource_chn, &m_chn.imp_encoder);
    if (ret < 0) {
        LOG_S(ERROR) << "UnBind FrameSource channel0 and Encoder failed";

    }

    /* Step.c Encoder exit */
    ret = encoder_exit();
    if (ret < 0) {
        LOG_S(ERROR) << "Encoder exit failed";

    }

    /* Step.d FrameSource exit */
    ret = framesource_exit();
    if (ret < 0) {
        LOG_S(ERROR) << "FrameSource exit failed";

    }

    /* Step.e System exit */
    ret = system_exit();
    if (ret < 0) {
        LOG_S(ERROR) << "system_exit() failed";

    }

}




void ImpEncoder::save_stream(std::vector<uint8_t> &buffer, IMPEncoderStream &stream) {
    for (unsigned int i = 0; i < stream.packCount; i++) {
        IMPEncoderPack &packet = stream.pack[i];

        buffer.insert(buffer.end(), (uint8_t *)packet.virAddr, (uint8_t *)packet.virAddr + packet.length);
    }
}

int ImpEncoder::save_stream(uint8_t *buffer, IMPEncoderStream &stream) {
    int bytes_read = 0;

    for (unsigned int i = 0; i < stream.packCount; i++) {
        IMPEncoderPack &packet = stream.pack[i];

        memcpy(buffer + bytes_read, (void *)packet.virAddr, packet.length);
        bytes_read += packet.length;
    }

    return bytes_read;
}

void ImpEncoder::snap_jpeg(std::vector<uint8_t> &buffer) {
    // Polling JPEG Snap, set timeout as 1000msec
    if (IMP_Encoder_PollingStream(1, 1000) != 0)
    {
        throw std::runtime_error("Polling stream timeout");
    }

    IMPEncoderStream stream;

    if (IMP_Encoder_GetStream(1, &stream, 1) != 0) {
        throw std::runtime_error("IMP_Encoder_GetStream() failed");
    }

    save_stream(buffer, stream);

    IMP_Encoder_ReleaseStream(1, &stream);

}

int clock_monotonic_gettime( timespec *p_tp )
{
  struct timespec _ts;
  int _rc = 0;

  _rc = clock_gettime( CLOCK_MONOTONIC, &_ts );
  p_tp->tv_sec = _ts.tv_sec;
  p_tp->tv_nsec = _ts.tv_nsec;
  return _rc;
}

static void sleep_til_next_slot(timespec *p_uptime,
                                timespec *p_period )
{
  struct timespec _sleepts;
  timespec _uptime;
  timespec _nextuptime = *p_uptime;


  timespecadd( &_nextuptime, p_period );
  *p_uptime = _nextuptime;

  if ( clock_monotonic_gettime( &_uptime ) != 0 )
  {
    LOG_S(ERROR) << "Failed to get time";
    nanosleep( p_period, NULL );
  }
  else if ( timespeccmp( &_nextuptime, &_uptime, <= ) )
  {
    // too late
  }
  else
  {
    timespecsub(&_nextuptime, &_uptime);
    _sleepts.tv_sec = _nextuptime.tv_sec;
    _sleepts.tv_nsec = _nextuptime.tv_nsec;
    nanosleep( &_sleepts, NULL );
  }
}

int snap_jpgTimeOut(uint8_t *buffer) 
{
    int size=0;
    // Polling JPEG Snap, set timeout as 1000msec
    for (int i=0; i<20; i++) // do some retries
    {
    	if (IMP_Encoder_PollingStream(1, 1000) != 0)
    	{
        	//throw std::runtime_error("Polling stream timeout");
	        LOG_S(ERROR) << "Polling stream timeout";
 	        usleep(10);
         continue;
    	}
	    else
	    {
            break;
	    }
   }
    IMPEncoderStream stream;

    if (IMP_Encoder_GetStream(1, &stream, 1) != 0)
    {
	 LOG_S(ERROR) << "GetStream failed";
    }

    size = ImpEncoder::save_stream(buffer, stream);
    IMP_Encoder_ReleaseStream(1, &stream);
    return size;
}
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}


static shared_conf currentConfig = {0};
static timespec _nextuptime;
static struct timespec _tsperiod;
static timespec _tsperiod32;

int ImpEncoder::snap_jpg(uint8_t *buffer) 
{
	int frameSize = 0;

	SharedMem &sharedMem = SharedMem::instance();
	shared_conf *newConfig = sharedMem.getConfig();

	if ((currentConfig.frmRateConfig[0] != newConfig->frmRateConfig[0]) || (currentConfig.frmRateConfig[1] != newConfig->frmRateConfig[1])) 
	{
  	   clock_monotonic_gettime(&_nextuptime);
	   unsigned int _periodms = (1000.0 * (float) newConfig->frmRateConfig[1] / (float) newConfig->frmRateConfig[0]);
       _tsperiod.tv_sec = _periodms / 1000;
	   _tsperiod32.tv_sec =  _tsperiod.tv_sec;
	   _tsperiod.tv_nsec = ( _periodms % 1000 ) * MS_IN_NS;
       _tsperiod32.tv_nsec =  _tsperiod.tv_nsec;

	    currentConfig.frmRateConfig[0] = newConfig->frmRateConfig[0];
	    currentConfig.frmRateConfig[1] = newConfig->frmRateConfig[1];
	    LOG_S(INFO) <<"MJPEG New framerate:" << _periodms << " ms";
    }

   /*struct timeval t0;
   struct timeval t1;
   float elapsed;
   gettimeofday(&t0, 0);*/

     sleep_til_next_slot(&_nextuptime, &_tsperiod);
     frameSize = ::snap_jpgTimeOut(buffer);

     /*gettimeofday(&t1, 0);
     elapsed = timedifference_msec(t0, t1);*/
	return frameSize;
}


int ImpEncoder::snap_h264(uint8_t *buffer) {
    // H264 Channel start receive picture
    int num_frames = 1;
    int bytes_read = 0;

    SharedMem &sharedMem = SharedMem::instance();
	shared_conf *newConfig = sharedMem.getConfig();

	if ((currentConfig.frmRateConfig[0] != newConfig->frmRateConfig[0]) || (currentConfig.frmRateConfig[1] != newConfig->frmRateConfig[1]))
	{
  	   clock_monotonic_gettime(&_nextuptime);
	   unsigned int _periodms = (1000.0 * (float) newConfig->frmRateConfig[1] / (float) newConfig->frmRateConfig[0]);
       _tsperiod.tv_sec = _periodms / 1000;
	   _tsperiod32.tv_sec =  _tsperiod.tv_sec;
	   _tsperiod.tv_nsec = ( _periodms % 1000 ) * MS_IN_NS;
       _tsperiod32.tv_nsec =  _tsperiod.tv_nsec;

	    currentConfig.frmRateConfig[0] = newConfig->frmRateConfig[0];
	    currentConfig.frmRateConfig[1] = newConfig->frmRateConfig[1];
	    LOG_S(INFO) <<"snap_h264 New framerate:" << _periodms << " ms";
    }

    sleep_til_next_slot(&_nextuptime, &_tsperiod);

    for (int i = 0; i < num_frames; i++)
    {
        // Polling H264 Stream, set timeout as 1000msec
        if (IMP_Encoder_PollingStream(0, 1000) != 0) {
            LOG_S(ERROR) << "Polling stream timeout";
	        usleep(10);
            continue;
        }

        IMPEncoderStream stream;

        // Get H264 Stream
        if (IMP_Encoder_GetStream(0, &stream, 1) != 0) {
            throw std::runtime_error("IMP_Encoder_GetStream() failed");
        }
        LOG_S(9) << "i" << i << ", stream.packCount"<<stream.packCount <<" stream.h264RefType="<<stream.refType << "seq="<< stream.seq;

        bytes_read += save_stream(buffer, stream);
        IMP_Encoder_ReleaseStream(0, &stream);
    }

    return bytes_read;
}

/*
bool ImpEncoder::listEmpty() {
    pthread_mutex_lock(&m_mutex);
    bool listEmpty = frameList.empty();
    pthread_mutex_unlock(&m_mutex);
    return listEmpty;
}

IMPEncoderPack ImpEncoder::getFrame() {
    pthread_mutex_lock(&m_mutex);
    IMPEncoderPack frame = frameList.front();
    frameList.pop_front();
    pthread_mutex_unlock(&m_mutex);
    return frame;
}
*/

void ImpEncoder::requestIDR()
{
    IMP_Encoder_RequestIDR(0);
}

int ImpEncoder::getSensorName()
{
    int ret  = 0;
    int fd   = 0;
    int data = -1;
    /* open device file */
    fd = open("/dev/sinfo", O_RDWR);
    if (-1 == fd) {
        LOG_S(ERROR) <<"err: open /dev/sinfo sensor failed";
        return -1;
    }
    /* iotcl to get sensor info. */
    /* cmd is IOCTL_SINFO_GET, data note sensor type according to SENSOR_TYPE */

    ret = ::ioctl(fd,IOCTL_SINFO_GET,&data);
    if (0 != ret) {
        close(fd);
        LOG_S(ERROR) <<"err: ioctl failed";
        return -1;
    }
    if (SENSOR_TYPE_INVALID == data)
        LOG_S(ERROR) <<"##### sensor not found";
    /* close device file */
    close(fd);
    return data;
}


int ImpEncoder::system_init()
{
    int ret = 0;
    char sensorName[STRING_MAX_SIZE];
    int sensorId = getSensorName();
    int sensorAddr;

    if ((sensorId == 1)
         || sensorId == 22)
    {
        strcpy(sensorName,"jxf22");
        sensorAddr = 0x40;
    }
    else if(sensorId == 2)
    {
        strcpy(sensorName,"jxh62");
        sensorAddr = 0x30;
    }
    else
    {
        strcpy(sensorName,"jxf23");
        sensorAddr = 0x40;
    }
    LOG_S(INFO) << "Found Sensor with ID:"<<  sensorId << "Name =" << sensorName;
    int sensorNameLen = strlen(sensorName);

    memset(&sensor_info, 0, sizeof(IMPSensorInfo));
    memcpy(sensor_info.name, sensorName, sensorNameLen);
    sensor_info.cbus_type = SENSOR_CUBS_TYPE;
    memcpy(sensor_info.i2c.type, sensorName, sensorNameLen);
    sensor_info.i2c.addr = sensorAddr;

    //IMP_LOG_ERR(TAG, "Imp Log %d\n", IMP_Log_Get_Option());
    //IMP_Log_Set_Option()

    ret = IMP_ISP_Open();
    if (ret < 0) {
        LOG_S(ERROR) << "failed to open ISP";
        return -1;
    }

    ret = IMP_ISP_AddSensor(&sensor_info);
    if (ret < 0) {
        LOG_S(ERROR) << "failed to AddSensor";
        exit(-1);
        return -1;

    }

    ret = IMP_ISP_EnableSensor();
    if (ret < 0) {
        LOG_S(ERROR) << "failed to EnableSensor";
        return -1;
    }


    ret = IMP_System_Init();
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_System_Init failed";
        return -1;
    }


    /* enable turning, to debug graphics */
    ret = IMP_ISP_EnableTuning();
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_ISP_EnableTuning failed";
        return -1;
    }

    /*
       ret = IMP_ISP_Tuning_SetWDRAttr(IMPISP_TUNING_OPS_MODE_DISABLE);
       if (ret < 0) {
       IMP_LOG_ERR(TAG, "failed to set WDR\n");
       return -1;
       }
     */

    mqtt_conn();

    return 0;
}


int ImpEncoder::system_exit() {
    int ret = 0;

    LOG_S(INFO) << "system_exit start";

    mqtt_disc();

    IMP_System_Exit();

    ret = IMP_ISP_DisableSensor();
    if (ret < 0) {
        LOG_S(ERROR) << "failed to EnableSensor";
        return -1;
    }

    ret = IMP_ISP_DelSensor(&sensor_info);
    if (ret < 0) {
        LOG_S(ERROR) << "failed to AddSensor";
        return -1;
    }

    ret = IMP_ISP_DisableTuning();
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_ISP_DisableTuning failed";
        return -1;
    }

    if (IMP_ISP_Close()) {
        LOG_S(ERROR) << "failed to open ISP";
        return -1;
    }

    LOG_S(INFO) <<" system_exit success";

    return 0;
}

int ImpEncoder::framesource_streamon() {

    int out_pipe[2];
    int saved_stdout;
    saved_stdout = dup(STDOUT_FILENO);
    pipe(out_pipe);
    dup2(out_pipe[1], STDOUT_FILENO);


    int ret = 0;
    /* Enable channels */

    ret = IMP_FrameSource_EnableChn(0);
    if (ret < 0) {
        dup2(saved_stdout, STDOUT_FILENO);
        LOG_S(ERROR) << "IMP_FrameSource_EnableChn(0) error:"<<  ret;
        return -1;
    } else {
        fflush(stdout);
        dup2(saved_stdout, STDOUT_FILENO);
    }


    return 0;
}

int ImpEncoder::framesource_streamoff() {
    int ret = 0;
    /* Enable channels */

    ret = IMP_FrameSource_DisableChn(0);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_FrameSource_DisableChn(0) error:"<<  ret;
        return -1;
    }

    return 0;
}

int ImpEncoder::framesource_init() {
    int ret;


    ret = IMP_FrameSource_CreateChn(0, &m_chn.fs_chn_attr);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_FrameSource_CreateChn(0) error:"<<  ret;
        return -1;
    }

    ret = IMP_FrameSource_SetChnAttr(0, &m_chn.fs_chn_attr);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_FrameSource_SetChnAttr(0) error:"<<  ret;
        return -1;
    }

    ret = IMP_FrameSource_CreateChn(1, &m_chn.fs_chn_attr);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_FrameSource_CreateChn(1) error:"<<  ret;
        return -1;
    }

    ret = IMP_FrameSource_SetChnAttr(1, &m_chn.fs_chn_attr);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_FrameSource_SetChnAttr(1) error:"<<  ret;
        return -1;
    }

    return 0;
}

int ImpEncoder::framesource_exit() {
    int ret;


    /*Destroy channel i*/
    ret = IMP_FrameSource_DestroyChn(0);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_FrameSource_DestroyChn(0) error:"<<  ret;
        return -1;
    }

    return 0;
}

int ImpEncoder::jpeg_init() {
    int ret;
    IMPEncoderAttr *enc_attr;
    IMPEncoderCHNAttr channel_attr;
    IMPFSChnAttr *imp_chn_attr_tmp;


    imp_chn_attr_tmp = &m_chn.fs_chn_attr;
    memset(&channel_attr, 0, sizeof(IMPEncoderCHNAttr));
    enc_attr = &channel_attr.encAttr;
    enc_attr->enType = PT_JPEG;
    enc_attr->bufSize = 0;
    enc_attr->profile = 2;
    enc_attr->picWidth =  imp_chn_attr_tmp->picWidth;
    enc_attr->picHeight = imp_chn_attr_tmp->picHeight;

    /* Create Channel */
    ret = IMP_Encoder_CreateChn(1, &channel_attr);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_CreateChn(1) error:"<<  ret;
        return -1;
    }

    /* Register Channel */
    ret = IMP_Encoder_RegisterChn(0, 1);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_RegisterChn(0,1) error:"<<  ret;
        return -1;
    }
    return 0;
}

int ImpEncoder::encoder_init(int quality, int skiptype, int maxSameSceneCnt, int maxgop) {

    int ret;
    IMPEncoderAttr *enc_attr;
    IMPEncoderRcAttr *rc_attr;
    IMPFSChnAttr *imp_chn_attr_tmp;
    IMPEncoderCHNAttr channel_attr;
    int bEnableScenecut;

    imp_chn_attr_tmp = &m_chn.fs_chn_attr;
    memset(&channel_attr, 0, sizeof(IMPEncoderCHNAttr));
    enc_attr = &channel_attr.encAttr;
    enc_attr->enType = PT_H264;
    enc_attr->bufSize = 0;
    enc_attr->profile = 1;
    enc_attr->picWidth = imp_chn_attr_tmp->picWidth;
    enc_attr->picHeight = imp_chn_attr_tmp->picHeight;
    rc_attr = &channel_attr.rcAttr;
     
    LOG_S(INFO) << "encoderMode: " << encoderMode;

    rc_attr->maxGop = maxgop; //2 * 25 / 1;
    SharedMem &mem = SharedMem::instance();
    shared_conf *conf = mem.getConfig();
    mem.readConfig();

     if (maxSameSceneCnt > 0)
     {
        bEnableScenecut = 1;
     }
     else
     {
        bEnableScenecut = 0;
     }


    if (encoderMode == ENC_RC_MODE_CBR) {
        LOG_S(INFO) << "Using CBR mode.";
        rc_attr->attrRcMode.rcMode = ENC_RC_MODE_CBR;

        if (conf->bitrate > 0)
        {
            LOG_S(INFO) << "Set initial bitrate (from sharedmem) to " << conf->bitrate;
            rc_attr->attrRcMode.attrH264Cbr.outBitRate = conf->bitrate;
        }
        else
        {
            LOG_S(INFO) << "Set initial bitrate (default value) to " << currentParams.bitrate;
            rc_attr->attrRcMode.attrH264Cbr.outBitRate = currentParams.bitrate;
            conf->bitrate =  currentParams.bitrate;
            mem.setConfig();
        }

        rc_attr->attrRcMode.attrH264Cbr.maxQp = 45;
        rc_attr->attrRcMode.attrH264Cbr.minQp = 15;
        rc_attr->attrRcMode.attrH264Cbr.iBiasLvl = 0;
        rc_attr->attrRcMode.attrH264Cbr.frmQPStep = 3;
        rc_attr->attrRcMode.attrH264Cbr.gopQPStep = 15;
        rc_attr->attrRcMode.attrH264Cbr.adaptiveMode = false;
        rc_attr->attrRcMode.attrH264Cbr.gopRelation = false;
        
        rc_attr->attrHSkip.hSkipAttr.skipType = (IMPSkipType) skiptype; //IMP_Encoder_STYPE_N1X;
        rc_attr->attrHSkip.hSkipAttr.m = 0;
        rc_attr->attrHSkip.hSkipAttr.n = 0;
        rc_attr->attrHSkip.hSkipAttr.maxSameSceneCnt = maxSameSceneCnt;
        rc_attr->attrHSkip.hSkipAttr.bEnableScenecut = bEnableScenecut;
        rc_attr->attrHSkip.hSkipAttr.bBlackEnhance = 0;
        rc_attr->attrHSkip.maxHSkipType = (IMPSkipType) skiptype; //IMP_Encoder_STYPE_N1X;
    } else if (encoderMode == ENC_RC_MODE_VBR) {
        LOG_S(INFO) << "Using VBR mode.";
        rc_attr->attrRcMode.rcMode = ENC_RC_MODE_VBR;

        if (conf->bitrate > 0)
        {
            LOG_S(INFO) << "Set initial bitrate (from sharedmem) to " << conf->bitrate;
            rc_attr->attrRcMode.attrH264Vbr.maxBitRate = conf->bitrate;
        }
        else
        {
            LOG_S(INFO) << "Set initial bitrate (default value) to " << currentParams.bitrate;
            rc_attr->attrRcMode.attrH264Vbr.maxBitRate = currentParams.bitrate;
            conf->bitrate =  currentParams.bitrate;
            mem.setConfig();
        }


        rc_attr->attrRcMode.attrH264Vbr.maxQp =45;
        rc_attr->attrRcMode.attrH264Vbr.minQp = 15;
        rc_attr->attrRcMode.attrH264Vbr.staticTime = 2;
        rc_attr->attrRcMode.attrH264Vbr.iBiasLvl = 0;
        rc_attr->attrRcMode.attrH264Vbr.changePos = 80;
        rc_attr->attrRcMode.attrH264Vbr.qualityLvl = quality;
        rc_attr->attrRcMode.attrH264Vbr.frmQPStep = 3;
        rc_attr->attrRcMode.attrH264Vbr.gopQPStep = 15;
        rc_attr->attrRcMode.attrH264Vbr.gopRelation = false;

        rc_attr->attrHSkip.hSkipAttr.skipType = (IMPSkipType) skiptype; //IMP_Encoder_STYPE_N1X;
        rc_attr->attrHSkip.hSkipAttr.m = 0;
        rc_attr->attrHSkip.hSkipAttr.n = 0;
        rc_attr->attrHSkip.hSkipAttr.maxSameSceneCnt = maxSameSceneCnt;
        rc_attr->attrHSkip.hSkipAttr.bEnableScenecut = bEnableScenecut;
        rc_attr->attrHSkip.hSkipAttr.bBlackEnhance = 0;
        rc_attr->attrHSkip.maxHSkipType = (IMPSkipType) skiptype; //IMP_Encoder_STYPE_N1X;
    } else if (encoderMode == ENC_RC_MODE_SMART) {
        LOG_S(INFO) << "Using SMART mode.";
        if (conf->bitrate > 0)
        {
            LOG_S(INFO) << "Set initial bitrate (from sharedmem) to " << conf->bitrate;
            rc_attr->attrRcMode.attrH264Smart.maxBitRate = conf->bitrate;
        }
        else
        {
            LOG_S(INFO) << "Set initial bitrate (default value) to " << currentParams.bitrate;
            rc_attr->attrRcMode.attrH264Smart.maxBitRate = currentParams.bitrate;
            conf->bitrate =  currentParams.bitrate;
            mem.setConfig();
        }


        rc_attr->attrRcMode.rcMode = ENC_RC_MODE_SMART;
        rc_attr->attrRcMode.attrH264Smart.maxQp = 45;
        rc_attr->attrRcMode.attrH264Smart.minQp = 15;
        rc_attr->attrRcMode.attrH264Smart.staticTime = 2;
        rc_attr->attrRcMode.attrH264Smart.iBiasLvl = 0;
        rc_attr->attrRcMode.attrH264Smart.changePos = 80;
        rc_attr->attrRcMode.attrH264Smart.qualityLvl = quality;
        rc_attr->attrRcMode.attrH264Smart.frmQPStep = 3;
        rc_attr->attrRcMode.attrH264Smart.gopQPStep = 15;
        rc_attr->attrRcMode.attrH264Smart.gopRelation = false;
        
        rc_attr->attrHSkip.hSkipAttr.skipType = (IMPSkipType) skiptype; //IMP_Encoder_STYPE_N4X;
        rc_attr->attrHSkip.hSkipAttr.m = rc_attr->maxGop - 1;
        rc_attr->attrHSkip.hSkipAttr.n = 1;
        rc_attr->attrHSkip.hSkipAttr.maxSameSceneCnt = maxSameSceneCnt;
        rc_attr->attrHSkip.hSkipAttr.bEnableScenecut = bEnableScenecut;
        rc_attr->attrHSkip.hSkipAttr.bBlackEnhance = 0;
        rc_attr->attrHSkip.maxHSkipType = (IMPSkipType) skiptype; //IMP_Encoder_STYPE_N4X;
    } else { /* fixQp */
        LOG_S(INFO) << "Using FIX QP mode.";
        rc_attr->attrRcMode.rcMode = ENC_RC_MODE_FIXQP;
        rc_attr->attrRcMode.attrH264FixQp.qp = 42;

        rc_attr->attrHSkip.hSkipAttr.skipType = (IMPSkipType) skiptype; //IMP_Encoder_STYPE_N1X;
        rc_attr->attrHSkip.hSkipAttr.m = 0;
        rc_attr->attrHSkip.hSkipAttr.n = 0;
        rc_attr->attrHSkip.hSkipAttr.maxSameSceneCnt = maxSameSceneCnt;
        rc_attr->attrHSkip.hSkipAttr.bEnableScenecut = bEnableScenecut;
        rc_attr->attrHSkip.hSkipAttr.bBlackEnhance = 0;
        rc_attr->attrHSkip.maxHSkipType = (IMPSkipType) skiptype; //IMP_Encoder_STYPE_N1X;
    }
                
    ret = IMP_Encoder_CreateChn(0, &channel_attr);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_CreateChn(0) error:"<<  ret;
        return -1;
    }

    ret = IMP_Encoder_RegisterChn(0, 0);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_RegisterChn(0,0) error:"<<  ret;
        return -1;
    }

    IMPEncoderFrmRate rate = {0};
    ret = IMP_Encoder_GetChnFrmRate(0, &rate);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_GetChnFrmRate(0) error:"<<  ret;

    }

    return 0;
}

int ImpEncoder::encoder_chn_exit(int encChn) {
    int ret;
    IMPEncoderCHNStat chn_stat;
    ret = IMP_Encoder_Query(encChn, &chn_stat);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_Query(" <<encChn<<" ) error:"<<  ret;
        return -1;
    }

    if (chn_stat.registered) {
        ret = IMP_Encoder_UnRegisterChn(encChn);
        if (ret < 0) {
            LOG_S(ERROR) << "IMP_Encoder_UnRegisterChn(" <<encChn<<") error:"<<  ret;
            return -1;
        }

        ret = IMP_Encoder_DestroyChn(encChn);
        if (ret < 0) {
            LOG_S(ERROR) << "IMP_Encoder_DestroyChn(" <<encChn<<")  error:"<<  ret;
            return -1;
        }
    }
    return 0;
}

int ImpEncoder::encoder_exit(void) {
    int ret;

    ret = encoder_chn_exit(0);
    if (ret < 0) {
        LOG_S(ERROR) << "encoder_chn_exit(0) error:"<<  ret;
        return -1;
    }

    ret = encoder_chn_exit(ENC_JPEG_CHANNEL);
    if (ret < 0) {
        LOG_S(ERROR) << "encoder_chn_exit(ENC_JPEG_CHANNEL) error:"<<  ret;
        return -1;
    }

    ret = IMP_Encoder_DestroyGroup(0);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_DestroyGroup(0) error:"<<  ret;
        return -1;
    }

    return 0;
}



void ImpEncoder::setNightVision(bool state) {
    IMPISPRunningMode isprunningmode;
    IMPISPSceneMode sceneMode;
    IMPISPColorfxMode colormode;
    int ret;
    if (state) {
        isprunningmode = IMPISP_RUNNING_MODE_NIGHT;
        sceneMode = IMPISP_SCENE_MODE_NIGHT;
        colormode = IMPISP_COLORFX_MODE_BW;
    } else {
        isprunningmode = IMPISP_RUNNING_MODE_DAY;
        sceneMode = IMPISP_SCENE_MODE_AUTO;
        colormode = IMPISP_COLORFX_MODE_AUTO;
    }
    ret = IMP_ISP_Tuning_SetISPRunningMode(isprunningmode);
    if (ret) {
        LOG_S(ERROR) << "IMP_ISP_Tuning_SetISPRunningMode(" << isprunningmode << ") error:"<<  ret;;
    }
    ret = IMP_ISP_Tuning_SetSceneMode(sceneMode);
    if (ret) {
        LOG_S(ERROR) << "IMP_ISP_Tuning_SetSceneMode(" << sceneMode << ") error:"<<  ret;
    }

    ret = IMP_ISP_Tuning_SetColorfxMode(colormode);
    if (ret) {
       LOG_S(ERROR) << "IMP_ISP_Tuning_SetColorfxMode("<< colormode << ") error:"<<  ret;
    }


}

