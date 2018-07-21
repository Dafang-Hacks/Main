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
#include <stdexcept>
//#include "logger.h"

// ---- OSD
//

//#include <time.h>
#include "../inc/font/SansBig.h"
#include "../inc/font/SansSmall.h"
#include "../inc/font/MonospaceBig.h"
#include "../inc/font/MonospaceSmall.h"

#include "sharedmem.h"
#include "../../v4l2rtspserver-tools/sharedmem.h"
#include "../inc/imp/imp_encoder.h"

#define OSD_REGION_HEIGHT               60

int grpNum = 0;
unsigned int gRegionH = 0;
unsigned gRegionW = 0;
int gwidth;
int gheight;
int gpos;

// OSD for text and OSD for detection indicator
#define OSD_TEXT 0
#define OSD_MOTION 1
IMPRgnHandle prHander[2] = {INVHANDLE, INVHANDLE};


#define OSD_DETECTIONHEIGHT  40
#define OSD_DETECTIONWIDTH  40

bool gDetectionOn = false;
bool ismotionActivated = true;
IMPIVSInterface *inteface = NULL;
// Activate or not tracking
bool isMotionTracking = false;
int motionTimeout = -1; // -1 is for deactivation

static int ivsMoveStart(int grp_num, int chn_num, IMPIVSInterface **interface, int x0, int y0, int x1, int y1, int width, int height );
static void *ivsMoveDetectionThread(void *arg);
static int snap_jpeg(int width, int height);

static unsigned char charDetection[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

};



static void setOsdPosXY(IMPRgnHandle handle, int width, int height, int fontSize, int posX, int posY) {
    int ret = 0;
    IMPOSDRgnAttr rAttrFont;
    memset(&rAttrFont, 0, sizeof(IMPOSDRgnAttr));
    rAttrFont.type = OSD_REG_PIC;
    rAttrFont.rect.p0.x = posX;
    rAttrFont.rect.p0.y = posY;
    rAttrFont.rect.p1.x= rAttrFont.rect.p0.x + width -1;
    rAttrFont.rect.p1.y = rAttrFont.rect.p0.y + fontSize -1 ;
    rAttrFont.fmt = PIX_FMT_BGRA;
    LOG_S(INFO)  << "OSD pos " <<  rAttrFont.rect.p0.x << "," << rAttrFont.rect.p0.y << "," << rAttrFont.rect.p1.x << ',' << rAttrFont.rect.p1.y;
    rAttrFont.data.picData.pData = NULL;
    ret= IMP_OSD_SetRgnAttr(handle, &rAttrFont);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_OSD_SetRgnAttr TimeStamp error";
    }
}

static void setOsdPos(IMPRgnHandle handle, int width, int height, int fontSize, int pos) {

    // 1 is down
    if (pos == 1) {
        setOsdPosXY(handle, width, height, fontSize, 0, height - (fontSize));
    } else {
        setOsdPosXY(handle, width, height, fontSize, 0,0);
    }
}

static IMPRgnHandle osdInit(int number, int width, int height, int pos) {
    int ret = 0;
    IMPOSDGrpRgnAttr grAttrFont;
    IMPRgnHandle rHanderFont;

    rHanderFont = IMP_OSD_CreateRgn(NULL);
    if (rHanderFont == INVHANDLE) {
        LOG_S(ERROR) << "IMP_OSD_CreateRgn TimeStamp error !";
        return INVHANDLE;
    }

    ret = IMP_OSD_RegisterRgn(rHanderFont, 0, NULL);
    if (ret < 0) {
        LOG_S(ERROR) << "IVS IMP_OSD_RegisterRgn failed";
        return INVHANDLE;
    }

    setOsdPos(rHanderFont, width,height,OSD_REGION_HEIGHT, pos);

    if (IMP_OSD_GetGrpRgnAttr(rHanderFont, 0, &grAttrFont) < 0) {
        LOG_S(ERROR) << "IMP_OSD_GetGrpRgnAttr Logo error !";
        return INVHANDLE;

    }
    memset(&grAttrFont, 0, sizeof(IMPOSDGrpRgnAttr));
    grAttrFont.show = 0;

    /* Disable Font global alpha, only use pixel alpha. */
    grAttrFont.gAlphaEn = 0; 
    grAttrFont.fgAlhpa = 0;
    grAttrFont.bgAlhpa = 0;
    grAttrFont.layer = number +1;

    if (IMP_OSD_SetGrpRgnAttr(rHanderFont, 0, &grAttrFont) < 0) {
        LOG_S(ERROR) << "IMP_OSD_SetGrpRgnAttr Logo error !";
        return INVHANDLE;
    }


    return rHanderFont;
}

static IMPRgnHandle osdDetectionIndicatorInit(int number, int width, int height, int x, int y) {
    int ret = 0;

    IMPRgnHandle rHanderFont;
    IMPOSDGrpRgnAttr grAttrFont;

    rHanderFont = IMP_OSD_CreateRgn(NULL);
    if (rHanderFont == INVHANDLE) {
        LOG_S(ERROR) << "IMP_OSD_CreateRgn TimeStamp error !";
        return INVHANDLE;
    }

    ret = IMP_OSD_RegisterRgn(rHanderFont, 0, NULL);
    if (ret < 0) {
        LOG_S(ERROR) << "IVS IMP_OSD_RegisterRgn failed";
        return INVHANDLE;
    }

    setOsdPosXY(rHanderFont, OSD_DETECTIONWIDTH, OSD_DETECTIONHEIGHT, OSD_DETECTIONHEIGHT, x, y);

    if (IMP_OSD_GetGrpRgnAttr(rHanderFont, 0, &grAttrFont) < 0) {
        LOG_S(ERROR) << "IMP_OSD_GetGrpRgnAttr Logo error !";
        return INVHANDLE;

    }
    memset(&grAttrFont, 0, sizeof(IMPOSDGrpRgnAttr));
    grAttrFont.show = 0;

    // Disable Font global alpha, only use pixel alpha.
    grAttrFont.gAlphaEn = 0;
    grAttrFont.fgAlhpa = 0;
    grAttrFont.bgAlhpa = 0;
    grAttrFont.layer = number +1;

    if (IMP_OSD_SetGrpRgnAttr(rHanderFont, 0, &grAttrFont) < 0) {
        LOG_S(ERROR) << "IMP_OSD_SetGrpRgnAttr Logo error !\n";
        return INVHANDLE;
    }


    return rHanderFont;
}


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
    ret = ivsMoveStart(0, 0, &inteface, detectionRegion[0], detectionRegion[1], detectionRegion[2], detectionRegion[3],gwidth,gheight) ;
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

static int osd_show(void) {
    int ret;

    ret = IMP_OSD_ShowRgn(prHander[OSD_TEXT], grpNum, 1);
    if (ret != 0) {
        LOG_S(ERROR) << "IMP_OSD_ShowRgn() timeStamp error";
        return -1;
    }

    ret = IMP_OSD_ShowRgn(prHander[OSD_MOTION], grpNum, 1);
    if (ret != 0) {
        LOG_S(ERROR) << "IMP_OSD_ShowRgn() timeStamp error";
        return -1;
    }

    return 0;
}
static uint32_t colorMap[] = { OSD_WHITE,OSD_BLACK, OSD_RED, OSD_GREEN, OSD_BLUE, OSD_GREEN | OSD_BLUE, OSD_RED|OSD_GREEN, OSD_BLUE|OSD_RED};

void osd_draw_timestamp(BaseFont *font, shared_conf &currentConfig) {
    static IMPOSDRgnAttrData rAttrData;

    time_t current_time;
    time(&current_time);

    struct tm *current_date;
    current_date = localtime(&current_time);

    uint32_t *data = (uint32_t*)rAttrData.picData.pData;

    if (data == NULL) {
        LOG_S(INFO) << "Data is empty, allocating it anew";
        data = (uint32_t*)malloc(sizeof(uint32_t) * gRegionW * gRegionH);
        rAttrData.picData.pData = data;
    }

    memset(data, 0, sizeof(uint32_t) * gRegionW * gRegionH);

    char text[STRING_MAX_SIZE];
    strftime(text, STRING_MAX_SIZE, currentConfig.osdTimeDisplay, current_date);

    int cursor_offset = 0;

    for (int i = 0; text[i] != '\x00'; i++) {
        char c = text[i];

        // Check if the char is not in the font
        if (!font->isSupported(c)) {
            LOG_S(INFO) << "Character " << c << " is not supported";
            continue;
        }

        // Fonts are stored in bytes
        int width = font->getWidth(c);
        int height = font->getHeight(c);

        // Check if there is still room
        if (cursor_offset + width > gRegionW) {
            LOG_S(INFO) << "No more space to display " << text + i;
            break;
        }

        // Draw the character
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (font->getPixel(c, x, y)) {
                    data[y * gRegionW + x + cursor_offset] = colorMap[currentConfig.osdColor];
                } else {
                    data[y * gRegionW + x + cursor_offset] = 0; 
                }
            }
        }

        // Move the cursor to the right
        cursor_offset += width + currentConfig.osdSpace;
    }

    IMP_OSD_UpdateRgnAttrData(prHander[0], &rAttrData);
}

void osd_draw_detection_circle(shared_conf &currentConfig) {
    static IMPOSDRgnAttrData rAttrData;

    if (currentConfig.motionOSD == -1) {
        return;
    }

    if ((unsigned int)currentConfig.motionOSD * sizeof(colorMap[0]) >= sizeof(colorMap)) {
        LOG_S(INFO) << "OSD motion detection circle color is invalid: " << currentConfig.motionOSD;
        return;
    }

    uint32_t *data = (uint32_t*)rAttrData.picData.pData;

    if (data == NULL) {
        data = (uint32_t *)malloc(OSD_DETECTIONHEIGHT * OSD_DETECTIONWIDTH * sizeof(uint32_t));
        rAttrData.picData.pData = data;
    }

    memset(data, 0, OSD_DETECTIONHEIGHT * OSD_DETECTIONWIDTH * sizeof(uint32_t));

    if (gDetectionOn == true) {
        // Draw the 29x26 red circle
        for (int y = 0; y < 29; y++) {
            for (int x = 0; x < 26; x++) {
                if (charDetection[x + 26 * y]) {
                    data [y * OSD_DETECTIONWIDTH + x] = colorMap[currentConfig.motionOSD];
                } else {
                    data [y * OSD_DETECTIONWIDTH + x] = 0;
                }
            }
        }
    }

    IMP_OSD_UpdateRgnAttrData(prHander[OSD_MOTION], &rAttrData);
}

static void *update_thread(void *p) {
    loguru::set_thread_name("update_thread");

    BaseFont *font = NULL;

    bool alreadySetDetectionRegion = false;

    SharedMem &sharedMem = SharedMem::instance();
    shared_conf *newConfig = sharedMem.getConfig();
    struct shared_conf currentConfig = {0};

    int ret = osd_show();

    if (ret < 0) {
        LOG_S(ERROR) << "OSD show error";
        return NULL;
    }

    while (true) {
        bool osd_text_changed = false;

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
            ret = IMP_Encoder_SetChnFrmRate(0, &rate);
            if (ret < 0) {
                LOG_S(ERROR) << "IMP_Encoder_SetChnFrmRate(0) error:"<<  ret;
            }
        }

        if (strcmp(currentConfig.osdTimeDisplay, newConfig->osdTimeDisplay) != 0) {
            LOG_S(INFO) << "Changed OSD format string";
            osd_text_changed = true;
        }

        if (currentConfig.osdColor != newConfig->osdColor) {
            if ((unsigned int)newConfig->osdColor < sizeof(colorMap) / sizeof(colorMap[0])) {
                LOG_S(INFO) << "Changed OSD color";
            } else {
                LOG_S(INFO) << "New OSD color is invalid!";
                newConfig->osdColor = currentConfig.osdColor;
            }
        }

        if ((currentConfig.osdSize != newConfig->osdSize) || (currentConfig.osdFixedWidth != newConfig->osdFixedWidth)) {
            if (newConfig->osdFixedWidth == true) {
                if (newConfig->osdSize == 0) {
                    font = FONT_MONOSPACE_SMALL;
                } else {
                    font = FONT_MONOSPACE_BIG;
                }
            } else {
                if (newConfig->osdSize == 0) {
                    font = FONT_SANS_SMALL;
                } else {
                    font = FONT_SANS_BIG;
                }
            }

            // As the size changed, re-display the OSD
            setOsdPosXY(prHander[OSD_TEXT], gwidth, gheight, font->getHeight('0'), 0, newConfig->osdPosY);
            LOG_S(INFO) << "Changed OSD size, OSD pos, or OSD font";
        }

        if (currentConfig.osdSpace != newConfig->osdSpace) {
            // As the size changed, re-display the OSD
            LOG_S(INFO) <<  "Changed OSD space";
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
               if (alreadySetDetectionRegion == false)
                {
                    alreadySetDetectionRegion = true;
                    ivsSetDetectionRegion(newConfig->detectionRegion);
                    LOG_S(INFO) << "Changed motion region";
                }

                ivsSetsensitivity(newConfig->sensitivity);
            }
        }
        if (currentConfig.motionOSD != newConfig->motionOSD) {
            LOG_S(INFO) << "Display motion OSD color=" << newConfig->motionOSD;
        }

        if (newConfig->motionTimeout > 0) {
            motionTimeout = newConfig->motionTimeout;
        }

        memcpy(&currentConfig, newConfig, sizeof(shared_conf));

        // memcpy won't copy the text properly
        if (osd_text_changed) {
            strcpy(currentConfig.osdTimeDisplay, newConfig->osdTimeDisplay);
        }

        // Read the current time
        struct timespec spec;
        clock_gettime(CLOCK_REALTIME, &spec);

        // Sleep until the next second
        spec.tv_sec = 0;
        spec.tv_nsec = 1000000000L - spec.tv_nsec;

        nanosleep(&spec, NULL);

        // Draw the OSD
        osd_draw_timestamp(font, currentConfig);
        osd_draw_detection_circle(currentConfig);

        // Take a picture once every second
        snap_jpeg(gwidth, gheight);
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
    exec_command("/system/sdcard/scripts/detectionTracking.sh", NULL);
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

                       exec_command("/system/sdcard/scripts/detectionTracking.sh", param);
                       exec_command("/system/sdcard/scripts/detectionOn.sh", NULL);

                       if (motionTimeout != -1)
                       {
                            signal(SIGALRM, endofmotion);
                            alarm(motionTimeout);
                       }
                }
                else
                {
                    if (isWasOn == true) {
                        exec_command("/system/sdcard/scripts/detectionOff.sh", NULL);
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
                    exec_command("/system/sdcard/scripts/detectionOn.sh", NULL);
                    LOG_S(INFO) << "Detect !!";

                } else {
                        if (isWasOn == true) {
                            exec_command("/system/sdcard/scripts/detectionOff.sh", NULL);
                        }
                        gDetectionOn = false;
                        isWasOn = false;
                }

                if ((isWasOn == true) &&
                    (result->retRoi[0] == 0))
                {
                    isWasOn = false;
                    gDetectionOn = false;
                    exec_command("/system/sdcard/scripts/detectionOff.sh", NULL);
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
    if (IMP_OSD_CreateGroup(0) < 0) {
       LOG_S(ERROR) << "IMP_OSD_CreateGroup(0) error !";
    }
    int osdPos = 0; // 0 = UP,1 = down
    gwidth= currentParams.width;
    gheight = currentParams.height;
    gpos = osdPos;
    gRegionH = OSD_REGION_HEIGHT;
    gRegionW = currentParams.width;

    prHander[OSD_TEXT] = osdInit(OSD_TEXT, currentParams.width, currentParams.height, osdPos);
    if (prHander[OSD_TEXT] == INVHANDLE) {
        LOG_S(ERROR) << "OSD init failed";
    }

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

    prHander[OSD_MOTION] = osdDetectionIndicatorInit(OSD_MOTION, currentParams.width, currentParams.height,
                                                     currentParams.width - OSD_DETECTIONWIDTH, 0);
    if (prHander[OSD_MOTION] == INVHANDLE) {
        LOG_S(ERROR) << "OSD detection indicator init failed";
    }

    ret = IMP_OSD_Start(0);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_OSD_Start error !";
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

int save_stream(void *buffer, IMPEncoderStream *stream) {
    int i, nr_pack = stream->packCount;
    // IMP_LOG_ERR(TAG,  "Pack count: %d\n", nr_pack);

    void *memoryAddress = buffer;
    int bytesRead = 0;
    for (i = 0; i < nr_pack; i++) {
        int packLen = stream->pack[i].length;
        memcpy(memoryAddress, (void *) stream->pack[i].virAddr, packLen);
        memoryAddress = (void *) ((int) memoryAddress + packLen);
        bytesRead = bytesRead + packLen;
        // IMP_LOG_ERR(TAG,  "Pack Len: %d\n", packLen);
    }

    return bytesRead;
}

int snap_jpeg(int width, int height) {
    int ret;
    int bytesRead = 0;
    unsigned char _buffer[width * height];

    /* Polling JPEG Snap, set timeout as 1000msec */
    ret = IMP_Encoder_PollingStream(1, 1000);
    if (ret < 0) {
        LOG_S(ERROR) << "Polling stream timeout";
        return -1;
    }

    IMPEncoderStream stream;
    /* Get JPEG Snap */
    ret = IMP_Encoder_GetStream(1, &stream, 1);
    if (ret < 0) {
        LOG_S(ERROR) << "IMP_Encoder_GetStream() failed";
        return -1;
    }

    ret = save_stream(_buffer, &stream);
    bytesRead = ret;

//    int nr_pack = stream.packCount;
    SharedMem &mem = SharedMem::instance();
    mem.copyImage(_buffer, sizeof(_buffer));

    IMP_Encoder_ReleaseStream(1, &stream);

    return bytesRead;
}



int ImpEncoder::save_stream(void *buffer, IMPEncoderStream *stream) {
    int i, nr_pack = stream->packCount;
    // IMP_LOG_ERR(TAG,  "Pack count: %d\n", nr_pack);

    void *memoryAddress = buffer;
    int bytesRead = 0;
    for (i = 0; i < nr_pack; i++) {
        int packLen = stream->pack[i].length;
        memcpy(memoryAddress, (void *) stream->pack[i].virAddr, packLen);
        memoryAddress = (void *) ((int) memoryAddress + packLen);
        bytesRead = bytesRead + packLen;
        // IMP_LOG_ERR(TAG,  "Pack Len: %d\n", packLen);
    }

    return bytesRead;
}


int ImpEncoder::snap_h264(char *buffer) {
    int nr_frames = 1;
    int ret;
    int bytesRead = 0;
    /* H264 Channel start receive picture */

/*    if (framesCount == currentParams.framerate * 2) {
        framesCount = 0;
        //
        //IMP_Encoder_FlushStream(0);
        //requestIDR();
    } else {
        framesCount++;
    }

*/
    int i;
    for (i = 0; i < nr_frames; i++) {
        /* Polling H264 Stream, set timeout as 1000msec */
        ret = IMP_Encoder_PollingStream(0, 1000);
        if (ret < 0) {
            LOG_S(ERROR) << "Polling stream timeout";
            continue;
        }

        IMPEncoderStream stream;
        /* Get H264 Stream */
        ret = IMP_Encoder_GetStream(0, &stream, 1);
        if (ret < 0) {
            LOG_S(ERROR) << "IMP_Encoder_GetStream() failed";
            return -1;
        }
        LOG_S(9) << "i" << i << ", stream.packCount"<<stream.packCount <<" stream.h264RefType="<<stream.refType << "seq="<< stream.seq;

        ret = save_stream(buffer, &stream);
        bytesRead = ret;
        if (ret < 0) {
            return ret;
        }

        IMP_Encoder_ReleaseStream(0, &stream);
    }


    return bytesRead;
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

    LOG_S(INFO) << "Found Sensor with ID:"<<  sensorId;
    if(sensorId == SENSOR_TYPE_JXF22){
        strcpy(sensorName,"jxf22");
    }else{
        strcpy(sensorName,"jxf23");
    }
    int sensorNameLen = strlen(sensorName);


    memset(&sensor_info, 0, sizeof(IMPSensorInfo));
    memcpy(sensor_info.name, sensorName, sensorNameLen);
    sensor_info.cbus_type = SENSOR_CUBS_TYPE;
    memcpy(sensor_info.i2c.type, sensorName, sensorNameLen);
    sensor_info.i2c.addr = SENSOR_I2C_ADDR;

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

