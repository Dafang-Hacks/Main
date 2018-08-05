/*
 * sample-Encoder-jpeg.c
 *
 * Copyright (C) 2014 Ingenic Semiconductor Co.,Ltd
 */


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

#include <stdexcept>
#include <tuple>

// ---- OSD
//

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_MODULE_H
#include FT_DRIVER_H

#include "sharedmem.h"
#include "../../v4l2rtspserver-tools/sharedmem.h"
#include "../inc/imp/imp_encoder.h"

#include "../inc/OSD.hpp"

int image_width;
int image_height;

bool gDetectionOn = false;
bool ismotionActivated = true;
char fontMono[256] = {0};
char fontSans[256] = {0};
char detectionScriptOn[256] = {0};
char detectionScriptOff[256]= {0};
char detectionTracking[256]= {0};



IMPIVSInterface *inteface = NULL;

// Activate or not tracking
bool isMotionTracking = false;
int motionTimeout = -1; // -1 is for deactivation

static int ivsMoveStart(int grp_num, int chn_num, IMPIVSInterface **interface, int x0, int y0, int x1, int y1, int width, int height );
static void *ivsMoveDetectionThread(void *arg);
static void snap_jpeg(std::vector<uint8_t> &buffer);



static int ivsSetsensitivity(int sens)
{
	int ret = 0;
	IMP_IVS_MoveParam param;
	ret = IMP_IVS_GetParam(0, &param);
	if (ret < 0) {
		LOG_S(ERROR) << "IMP_IVS_GetParam(0) failed";
		return -1;
	}

    param.sense[0] = sens;
    param.sense[1] = sens;
    param.sense[2] = sens;
    param.sense[3] = sens;

	ret = IMP_IVS_SetParam(0, &param);
	if (ret < 0) {
		LOG_S(ERROR) << "IMP_IVS_SetParam(0) failed";
		return -1;
	}
	return 0;
}

static int ivsSetDetectionRegion(int detectionRegion[4] )
{
	int ret = 0;
    ret = ivsMoveStart(0, 0, &inteface, detectionRegion[0], detectionRegion[1], detectionRegion[2], detectionRegion[3],image_width,image_height) ;
    if (ret < 0) {
        LOG_S(ERROR) << "ivsMoveStart(0, 0) failed";
    }
    pthread_t tid;
    //  start to get ivs move result
    if (pthread_create(&tid, NULL, ivsMoveDetectionThread, NULL)) {
        LOG_S(ERROR) << "create sample_ivs_move_get_result_process failed";
    }
  	return 0;
}


static void* update_thread(void *p) {
    loguru::set_thread_name("update_thread");

    std::vector<uint8_t> jpeg_buffer;

    FT_Library library;
    FT_Face face;
    int font_baseline_offset = 0;
    
    if (FT_Init_FreeType(&library) != 0) {
        LOG_S(ERROR) << "Could not initialize FreeType";
        return NULL;
    }

    FT_UInt hinting_engine = FT_HINTING_ADOBE;

    if (FT_Property_Set(library, "cff", "hinting-engine", &hinting_engine) != 0) {
        LOG_S(ERROR) << "Could not set hinting engine";
        return NULL;
    }

    bool firstConfigPass = true;
    bool alreadySetDetectionRegion = false;

    SharedMem &sharedMem = SharedMem::instance();
    shared_conf *newConfig = sharedMem.getConfig();
    shared_conf currentConfig = {0};

    // Move it to the top right of the screen
    OSD motion_osd = OSD(image_width - DETECTION_CIRCLE_SIZE, 0, DETECTION_CIRCLE_SIZE, DETECTION_CIRCLE_SIZE, 0);

    // Default to top left and 10px high until we read the config
    OSD timestamp_osd = OSD(0, 0, image_width, 10, 1);

    if (IMP_OSD_Start(0) != 0) {
        LOG_S(ERROR) << "OSD show error";
        return NULL;
    }

    motion_osd.show(true);
    timestamp_osd.show(true);

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

        if ((currentConfig.frmRateConfig[0] != newConfig->frmRateConfig[0]) || (currentConfig.frmRateConfig[1] != newConfig->frmRateConfig[1])) {
            IMPEncoderFrmRate rate = {0};
            LOG_S(INFO) << "Attempt to changed fps to " << newConfig->frmRateConfig[0] << "," << newConfig->frmRateConfig[1];
            rate.frmRateNum = newConfig->frmRateConfig[0];
            rate.frmRateDen = newConfig->frmRateConfig[1];

            int ret = IMP_Encoder_SetChnFrmRate(0, &rate);

            if (ret != 0) {
                LOG_S(ERROR) << "IMP_Encoder_SetChnFrmRate(0) error:" << ret;
            }
        }

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
            timestamp_osd.setBounds(timestamp_osd.getX(), newConfig->osdPosY, timestamp_osd.getWidth(), timestamp_osd.getHeight());

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
            timestamp_osd.setBounds(timestamp_osd.getX(), timestamp_osd.getY(), image_width, font_height);

            LOG_S(INFO) << "Max font bitmap height is " << font_height << " and baseline offset is " << font_baseline_offset;

            LOG_S(INFO) << "Changed OSD size";
        }
        if (currentConfig.motionTracking != newConfig->motionTracking ) {
            isMotionTracking = newConfig->motionTracking;
            if (isMotionTracking == true) {
                LOG_S(INFO) << "Tracking set to On";
                if (alreadySetDetectionRegion == false)
                {
                    alreadySetDetectionRegion = true;
                    ivsSetsensitivity(newConfig->sensitivity);
                    ivsSetDetectionRegion(newConfig->detectionRegion);
                }
            } else {
                    LOG_S(INFO) << "Tracking set to Off";
            }
        }

        if (currentConfig.sensitivity != newConfig->sensitivity) {
            if (newConfig->sensitivity == -1) {
                ismotionActivated = false;
                LOG_S(INFO) << "Deactivate motion";
            } else {
                ismotionActivated = true;
                LOG_S(INFO) << "Changed motion sensitivity=" << newConfig->sensitivity ;
                if (alreadySetDetectionRegion == false) {
                    alreadySetDetectionRegion = true;
                    ivsSetDetectionRegion(newConfig->detectionRegion);
                    LOG_S(INFO) << "Changed motion region";
                }

                ivsSetsensitivity(newConfig->sensitivity);
            }
        }
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

        if (newConfig->motionTimeout > 0) {
            motionTimeout = newConfig->motionTimeout;
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


        // Draw the timestamp OSD
        osd_draw_timestamp(timestamp_osd, face, font_baseline_offset, currentConfig);

        // Draw the motion detection circle
        if (currentConfig.motionOSD != -1) {
            osd_draw_detection_circle(motion_osd, gDetectionOn, currentConfig);
        }


        // Dump the JPEG once every second
        jpeg_buffer.clear();

        try {
            snap_jpeg(jpeg_buffer);
            sharedMem.copyImage(jpeg_buffer.data(), jpeg_buffer.size());
        } catch (const std::runtime_error &e) {
            LOG_S(INFO) << "Failed to read the JPEG stream: " << e.what();
        }
    }


    return NULL;
}
// ---- END OSD
//


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

static int ivsMoveStart(int grp_num, int chn_num, IMPIVSInterface **interface, int x0, int y0, int x1, int y1, int width, int height )
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
        param.roiRectCnt = 1;

        // Sensitivity (0 to 4)
        param.sense[0] = 4;

        param.roiRect[0].p0.x = x0;
        param.roiRect[0].p0.y = y0;
        if (x1 == 0 && y1 == 0)
        {
            param.roiRect[0].p1.x = width - 1;
            param.roiRect[0].p1.y = height  - 1;
        } else {
            param.roiRect[0].p1.x = x1 - 1;
            param.roiRect[0].p1.y = y1  - 1;

        }

        LOG_S(INFO) << "Detection region= ((" << param.roiRect[0].p0.x << "," << param.roiRect[0].p0.y << ")-("<< param.roiRect[0].p1.x << "," << param.roiRect[0].p1.y << "))";
    }

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

    return 0;
}

static void endofmotion(int sig)
{
    // In case of inactivity execute the script with no arguments
    exec_command(detectionTracking, NULL);
    LOG_S(INFO) << "End of motion";
}

static void *ivsMoveDetectionThread(void *arg)
{
    int ret = 0;
    int chn_num = 0; 
    IMP_IVS_MoveOutput *result = NULL;
    bool isWasOn = false;

    loguru::set_thread_name("ivsMoveDetectionThread");

    while (1) {

        if (ismotionActivated == true) {

            ret = IMP_IVS_PollingResult(chn_num, IMP_IVS_DEFAULT_TIMEOUTMS);
            if (ret < 0) {
                LOG_S(ERROR) << "IMP_IVS_PollingResult("<<chn_num << "," << IMP_IVS_DEFAULT_TIMEOUTMS<< ") failed";
                return (void *)-1;
            }
            ret = IMP_IVS_GetResult(chn_num, (void **)&result);
            if (ret < 0) {
                LOG_S(ERROR) << "IMP_IVS_GetResult(" << chn_num << ") failed";
                return (void *)-1;
            }

            if (isMotionTracking == true) {
               if (result->retRoi[0] == 1 ||
                   result->retRoi[1] == 1 ||
                   result->retRoi[2] == 1 ||
                   result->retRoi[3] == 1) {
                        char param[4][2] = {};
                        isWasOn = true;
                        gDetectionOn = true;

                       snprintf(param[0], sizeof(param[0]), "%.1d", result->retRoi[0]);
                       snprintf(param[1], sizeof(param[1]), "%.1d", result->retRoi[1]);
                       snprintf(param[2], sizeof(param[2]), "%.1d", result->retRoi[2]);
                       snprintf(param[3], sizeof(param[3]), "%.1d", result->retRoi[3]);

                       exec_command(detectionTracking, param);
                       exec_command(detectionScriptOn, NULL);

                       if (motionTimeout != -1)
                       {
                            signal(SIGALRM, endofmotion);
                            alarm(motionTimeout);
                       }
                }
                else
                {
                    if (isWasOn == true) {
                        exec_command(detectionScriptOff, NULL);
                    }
                    gDetectionOn = false;
                    isWasOn = false;
                }
            } else {
                // Detection !!!
                if ((isWasOn == false) &&
                    (result->retRoi[0]) == 1)
                {
                    isWasOn = true;
                    gDetectionOn = true;
                    exec_command(detectionScriptOn, NULL);
                    LOG_S(INFO) << "Detect !!";

                } else {
                        if (isWasOn == true) {
                            exec_command(detectionScriptOff, NULL);
                        }
                        gDetectionOn = false;
                        isWasOn = false;
                }

                if ((isWasOn == true) &&
                    (result->retRoi[0] == 0))
                {
                    isWasOn = false;
                    gDetectionOn = false;
                    exec_command(detectionScriptOff, NULL);
                    LOG_S(INFO) << "Detect finished!!";
                }
            }

            ret = IMP_IVS_ReleaseResult(chn_num, (void *)result);
            if (ret < 0) {
                LOG_S(ERROR) << "IMP_IVS_ReleaseResult("<< chn_num << ") failed";
                return (void *)-1;
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

    // Init Structure:
    memset(&chn, 0, sizeof(chn_conf));

    chn.index = 0;
    chn.enable = 1;
    chn.fs_chn_attr.pixFmt = PIX_FMT_NV12;
    chn.fs_chn_attr.outFrmRateNum = currentParams.framerate;
    chn.fs_chn_attr.outFrmRateDen = 1;
    chn.fs_chn_attr.nrVBs = 3;
    chn.fs_chn_attr.type = FS_PHY_CHANNEL;

    chn.fs_chn_attr.crop.enable = 0;
    chn.fs_chn_attr.crop.width = currentParams.width;
    chn.fs_chn_attr.crop.height = currentParams.height;
    chn.fs_chn_attr.crop.top = 0;
    chn.fs_chn_attr.crop.left = 0;

    chn.fs_chn_attr.scaler.enable = 1;
    chn.fs_chn_attr.scaler.outwidth = currentParams.width;
    chn.fs_chn_attr.scaler.outheight = currentParams.height;


    chn.fs_chn_attr.picWidth = currentParams.width;
    chn.fs_chn_attr.picHeight = currentParams.height;

    chn.framesource_chn.deviceID = DEV_ID_FS;
    chn.framesource_chn.groupID = 0;
    chn.framesource_chn.outputID = 0;

    chn.imp_encoder.deviceID = DEV_ID_ENC;
    chn.imp_encoder.groupID = 0;
    chn.imp_encoder.outputID = 0;

    chn.OSD_Cell.deviceID = DEV_ID_OSD;
    chn.OSD_Cell.groupID = 0;
    chn.OSD_Cell.outputID = 0;


    encoderMode = currentParams.rcmode;
    int ret;

    // Ini file to overide some path when /system/sdcard won't exit

    char dirNameBuffer[PATH_MAX + 1] = {0};
    // Read the symbolic link '/proc/self/exe'.
    const char *linkName = "/proc/self/exe";
    readlink(linkName, dirNameBuffer, sizeof(dirNameBuffer) - 1);

    // Read the same exe file + ini
    strncat(dirNameBuffer, ".ini", sizeof(dirNameBuffer) - 1);
    LOG_S(INFO) << "Try to read extra configuration on " << dirNameBuffer;
    INIReader reader(dirNameBuffer);
    if (reader.ParseError() < 0) {
        LOG_S(INFO) << "Can't load 'v4l2rstpserver.ini'";
        strcpy(fontMono,"/system/sdcard/fonts/NotoMono-Regular.ttf" );
        strcpy(fontSans,"/system/sdcard/fonts/NotoSans-Regular.ttf" );
        strcpy(detectionScriptOn, "/system/sdcard/scripts/detectionOn.sh");
        strcpy(detectionScriptOff, "/system/sdcard/scripts/detectionOff.sh");
        strcpy(detectionTracking, "/system/sdcard/scripts/detectionTracking.sh");

    } else {
        LOG_S(INFO) << "Parsing 'v4l2rstpserver.ini'!!!";
        strcpy(fontMono,reader.Get("Configuration", "FontFixedWidth", "").c_str());
        strcpy(fontSans,reader.Get("Configuration", "FontRegular", "").c_str());
        strcpy(detectionScriptOn, reader.Get("Configuration", "DetectionScriptOn", "").c_str());
        strcpy(detectionScriptOff, reader.Get("Configuration", "DetectionScriptOff", "").c_str());
        strcpy(detectionTracking, reader.Get("Configuration", "DetectionTracking", "").c_str());
    }


    /* Step.1 System init */
    ret = sample_system_init();
    if (ret < 0) {
        LOG_S(ERROR) <<"IMP_System_Init() failed";
    }

    /* Step.2 FrameSource init */
    ret = sample_framesource_init();
    if (ret < 0) {
        LOG_S(ERROR) << "FrameSource init failed";

    }


    ret = IMP_Encoder_CreateGroup(0);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_CreateGroup(0) error !";

    }

    ret = IMP_Encoder_CreateGroup(1);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_CreateGroup(1) error !";
    }

    /* Step.3 Encoder init */
    ret = sample_jpeg_init();
    if (ret < 0) {
        LOG_S(ERROR) << "Encoder JPEG init failed";

    }

    /* Step.3 Encoder init */
    ret = sample_encoder_init();
    if (ret < 0) {
        LOG_S(ERROR) << "Encoder h264 init failed";

    }

    // ----- OSD implementation: Init
    //
    if (IMP_OSD_CreateGroup(0) != 0) {
       LOG_S(ERROR) << "IMP_OSD_CreateGroup(0) error !";
    }

    image_width = currentParams.width;
    image_height = currentParams.height;

    
    

    /* Step Bind */
    ret = IMP_System_Bind(&chn.framesource_chn, &chn.OSD_Cell);
    if (ret < 0) {
        LOG_S(ERROR) << "Bind FrameSource channel0 and OSD failed";
    }

    ret = IMP_System_Bind(&chn.OSD_Cell, &chn.imp_encoder);
    if (ret < 0) {
        LOG_S(ERROR) << "Bind OSD and Encoder failed";
    }

    // ----- Motion implementation: Init
    //
    // Motion detection stuff, not sure it is optimized, maybe some calls are useless
    IMPCell ivs_grp0 = { DEV_ID_IVS , 0, 0};
    ret = IMP_IVS_CreateGroup(0);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_IVS_CreateGroup(0) failed";
    }

    ret = IMP_System_Bind (&chn.framesource_chn, &ivs_grp0);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_System_Bind";
    }

    // --- OSD and other stuffs thread
    pthread_t tid;
    ret = pthread_create(&tid, NULL, update_thread, NULL);
    sleep(0);
    if (ret) {
        LOG_S(ERROR) << "thread create error";
    }

    ret = sample_framesource_streamon();
    if (ret < 0) {
        LOG_S(ERROR) << "ImpStreamOn failed";

    }

    /* drop several pictures of invalid data */
    nanosleep((const struct timespec[]){{0, 500000000L}}, NULL);

    // JPEG
    ret = IMP_Encoder_StartRecvPic(1);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_StartRecvPic(2) failed";
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
    ret = sample_framesource_streamoff();
    if (ret < 0) {
        LOG_S(ERROR) << "FrameSource StreamOff failed";

    }

    /* Step.b UnBind */

    ret = IMP_System_UnBind(&chn.framesource_chn, &chn.imp_encoder);
    if (ret < 0) {
        LOG_S(ERROR) << "UnBind FrameSource channel0 and Encoder failed";

    }


    /* Step.c Encoder exit */
    ret = sample_encoder_exit();
    if (ret < 0) {
        LOG_S(ERROR) << "Encoder exit failed";

    }

    /* Step.d FrameSource exit */
    ret = sample_framesource_exit();
    if (ret < 0) {
        LOG_S(ERROR) << "FrameSource exit failed";

    }

    /* Step.e System exit */
    ret = sample_system_exit();
    if (ret < 0) {
        LOG_S(ERROR) << "sample_system_exit() failed";

    }

}

void save_stream(std::vector<uint8_t> &buffer, IMPEncoderStream &stream) {
    for (unsigned int i = 0; i < stream.packCount; i++) {
        IMPEncoderPack &packet = stream.pack[i];

        buffer.insert(buffer.end(), (uint8_t *)packet.virAddr, (uint8_t *)packet.virAddr + packet.length);
    }
}

int save_stream(uint8_t *buffer, IMPEncoderStream &stream) {
    int bytes_read = 0;

    for (unsigned int i = 0; i < stream.packCount; i++) {
        IMPEncoderPack &packet = stream.pack[i];

        memcpy(buffer + bytes_read, (void *)packet.virAddr, packet.length);
        bytes_read += packet.length;
    }

    return bytes_read;
}

void snap_jpeg(std::vector<uint8_t> &buffer) {
    // Polling JPEG Snap, set timeout as 1000msec
    if (IMP_Encoder_PollingStream(1, 1000) != 0) {
        throw std::runtime_error("Polling stream timeout");
    }

    IMPEncoderStream stream;

    if (IMP_Encoder_GetStream(1, &stream, 1) != 0) {
        throw std::runtime_error("IMP_Encoder_GetStream() failed");
    }

    save_stream(buffer, stream);
    IMP_Encoder_ReleaseStream(1, &stream);
}


int ImpEncoder::snap_h264(uint8_t *buffer) {
    // H264 Channel start receive picture
    int num_frames = 1;
    int bytes_read = 0;

    for (int i = 0; i < num_frames; i++) {
        // Polling H264 Stream, set timeout as 1000msec

        if (IMP_Encoder_PollingStream(0, 1000) != 0) {
            LOG_S(ERROR) << "Polling stream timeout";
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

void ImpEncoder::requestIDR() {
    IMP_Encoder_RequestIDR(0);
}

int ImpEncoder::getSensorName() {
    int ret  = 0;
    int fd   = 0;
    int data = -1;
    /* open device file */
    fd = open("/dev/sinfo", O_RDWR);
    if (-1 == fd) {
        LOG_S(ERROR) <<"err: open failed\n";
        return -1;
    }
    /* iotcl to get sensor info. */
    /* cmd is IOCTL_SINFO_GET, data note sensor type according to SENSOR_TYPE */

    ret = ::ioctl(fd,IOCTL_SINFO_GET,&data);
    if (0 != ret) {
        close(fd);
        LOG_S(ERROR) <<"err: ioctl failed\n";
        return -1;
    }
    if (SENSOR_TYPE_INVALID == data)
        LOG_S(ERROR) <<"##### sensor not found\n";
    /* close device file */
    close(fd);
    return data;
}
int ImpEncoder::sample_system_init() {


    int ret = 0;

    char sensorName[STRING_MAX_SIZE];
    int sensorId = getSensorName();
    int sensorAddr;

    //LOG_S(INFO) << "Found Sensor with ID:"<<  sensorId;
    if(sensorId == SENSOR_TYPE_JXF22){
        strcpy(sensorName,"jxf22");
        sensorAddr = 0x40;
    } else if(sensorId == SENSOR_TYPE_JXH62){
        strcpy(sensorName,"jxh62");
        sensorAddr = 0x30;
    }else{
        strcpy(sensorName,"jxf23");
        sensorAddr = 0x40;
    }
    LOG_S(INFO) << "Found Sensor with Name:"<<  sensorName;
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

    return 0;
}


int ImpEncoder::sample_system_exit() {
    int ret = 0;

    LOG_S(INFO) << "sample_system_exit start";


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

    LOG_S(INFO) <<" sample_system_exit success";

    return 0;
}

int ImpEncoder::sample_framesource_streamon() {

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

int ImpEncoder::sample_framesource_streamoff() {
    int ret = 0;
    /* Enable channels */

    ret = IMP_FrameSource_DisableChn(0);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_FrameSource_DisableChn(0) error:"<<  ret;
        return -1;
    }

    return 0;
}

int ImpEncoder::sample_framesource_init() {
    int ret;


    ret = IMP_FrameSource_CreateChn(0, &chn.fs_chn_attr);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_FrameSource_CreateChn(0) error:"<<  ret;
        return -1;
    }

    ret = IMP_FrameSource_SetChnAttr(0, &chn.fs_chn_attr);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_FrameSource_SetChnAttr(0) error:"<<  ret;
        return -1;
    }

    ret = IMP_FrameSource_CreateChn(1, &chn.fs_chn_attr);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_FrameSource_CreateChn(1) error:"<<  ret;
        return -1;
    }

    ret = IMP_FrameSource_SetChnAttr(1, &chn.fs_chn_attr);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_FrameSource_SetChnAttr(1) error:"<<  ret;
        return -1;
    }

    return 0;
}

int ImpEncoder::sample_framesource_exit() {
    int ret;


    /*Destroy channel i*/
    ret = IMP_FrameSource_DestroyChn(0);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_FrameSource_DestroyChn(0) error:"<<  ret;
        return -1;
    }

    return 0;
}

int ImpEncoder::sample_jpeg_init() {
    int ret;
    IMPEncoderAttr *enc_attr;
    IMPEncoderCHNAttr channel_attr;
    IMPFSChnAttr *imp_chn_attr_tmp;


    imp_chn_attr_tmp = &chn.fs_chn_attr;
    memset(&channel_attr, 0, sizeof(IMPEncoderCHNAttr));
    enc_attr = &channel_attr.encAttr;
    enc_attr->enType = PT_JPEG;
    enc_attr->bufSize = 0;
    enc_attr->profile = 0;
    enc_attr->picWidth = imp_chn_attr_tmp->picWidth;
    enc_attr->picHeight = imp_chn_attr_tmp->picHeight;

    /* Create Channel */
    ret = IMP_Encoder_CreateChn(1, &channel_attr);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_CreateChn(1) error:"<<  ret;
        return -1;
    }

    /* Resigter Channel */
    ret = IMP_Encoder_RegisterChn(0, 1);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_RegisterChn(0,1) error:"<<  ret;
        return -1;
    }

    return 0;
}

int ImpEncoder::sample_encoder_init() {

    int ret;
    IMPEncoderAttr *enc_attr;
    IMPEncoderRcAttr *rc_attr;
    IMPFSChnAttr *imp_chn_attr_tmp;
    IMPEncoderCHNAttr channel_attr;


    imp_chn_attr_tmp = &chn.fs_chn_attr;
    memset(&channel_attr, 0, sizeof(IMPEncoderCHNAttr));
    enc_attr = &channel_attr.encAttr;
    enc_attr->enType = PT_H264;
    enc_attr->bufSize = 0;
    enc_attr->profile = 0;
    enc_attr->picWidth = imp_chn_attr_tmp->picWidth;
    enc_attr->picHeight = imp_chn_attr_tmp->picHeight;
    rc_attr = &channel_attr.rcAttr;
    
    LOG_S(INFO) << "encoderMode: " << encoderMode;

    SharedMem &mem = SharedMem::instance();
    shared_conf *conf = mem.getConfig();
    mem.readConfig();

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
        
        rc_attr->attrHSkip.hSkipAttr.skipType = IMP_Encoder_STYPE_N1X;
        rc_attr->attrHSkip.hSkipAttr.m = 0;
        rc_attr->attrHSkip.hSkipAttr.n = 0;
        rc_attr->attrHSkip.hSkipAttr.maxSameSceneCnt = 0;
        rc_attr->attrHSkip.hSkipAttr.bEnableScenecut = 0;
        rc_attr->attrHSkip.hSkipAttr.bBlackEnhance = 0;
        rc_attr->attrHSkip.maxHSkipType = IMP_Encoder_STYPE_N1X;
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


        rc_attr->attrRcMode.attrH264Vbr.maxQp = 45;
        rc_attr->attrRcMode.attrH264Vbr.minQp = 15;
        rc_attr->attrRcMode.attrH264Vbr.staticTime = 2;
        rc_attr->attrRcMode.attrH264Vbr.iBiasLvl = 0;
        rc_attr->attrRcMode.attrH264Vbr.changePos = 80;
        rc_attr->attrRcMode.attrH264Vbr.qualityLvl = 2;
        rc_attr->attrRcMode.attrH264Vbr.frmQPStep = 3;
        rc_attr->attrRcMode.attrH264Vbr.gopQPStep = 15;
        rc_attr->attrRcMode.attrH264Vbr.gopRelation = false;

        rc_attr->attrHSkip.hSkipAttr.skipType = IMP_Encoder_STYPE_N1X;
        rc_attr->attrHSkip.hSkipAttr.m = 0;
        rc_attr->attrHSkip.hSkipAttr.n = 0;
        rc_attr->attrHSkip.hSkipAttr.maxSameSceneCnt = 0;
        rc_attr->attrHSkip.hSkipAttr.bEnableScenecut = 0;
        rc_attr->attrHSkip.hSkipAttr.bBlackEnhance = 0;
        rc_attr->attrHSkip.maxHSkipType = IMP_Encoder_STYPE_N1X;
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
        rc_attr->attrRcMode.attrH264Smart.qualityLvl = 2;
        rc_attr->attrRcMode.attrH264Smart.frmQPStep = 3;
        rc_attr->attrRcMode.attrH264Smart.gopQPStep = 15;
        rc_attr->attrRcMode.attrH264Smart.gopRelation = false;
        
        rc_attr->attrHSkip.hSkipAttr.skipType = IMP_Encoder_STYPE_N1X;
        rc_attr->attrHSkip.hSkipAttr.m = rc_attr->maxGop - 1;
        rc_attr->attrHSkip.hSkipAttr.n = 1;
        rc_attr->attrHSkip.hSkipAttr.maxSameSceneCnt = 6;
        rc_attr->attrHSkip.hSkipAttr.bEnableScenecut = 0;
        rc_attr->attrHSkip.hSkipAttr.bBlackEnhance = 0;
        rc_attr->attrHSkip.maxHSkipType = IMP_Encoder_STYPE_N1X;
    } else { /* fixQp */
        LOG_S(INFO) << "Using FIX QP mode.";
        rc_attr->attrRcMode.rcMode = ENC_RC_MODE_FIXQP;
        rc_attr->attrRcMode.attrH264FixQp.qp = 42;

        rc_attr->attrHSkip.hSkipAttr.skipType = IMP_Encoder_STYPE_N1X;
        rc_attr->attrHSkip.hSkipAttr.m = 0;
        rc_attr->attrHSkip.hSkipAttr.n = 0;
        rc_attr->attrHSkip.hSkipAttr.maxSameSceneCnt = 0;
        rc_attr->attrHSkip.hSkipAttr.bEnableScenecut = 0;
        rc_attr->attrHSkip.hSkipAttr.bBlackEnhance = 0;
        rc_attr->attrHSkip.maxHSkipType = IMP_Encoder_STYPE_N1X;
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

int ImpEncoder::sample_encoder_exit(void) {
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

