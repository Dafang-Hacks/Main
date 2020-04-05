#include <string.h>    
#include <errno.h>      

#include <imp/imp_isp.h>
#include <imp/imp_system.h>
#include <imp/imp_log.h>
#include <imp/imp_framesource.h>
#include <imp/imp_encoder.h>

#include "imp-common.h" 
#include "capture_and_encoding.h"

#define TAG "capture_and_encoding"

extern struct chn_conf chn[];

int destory()
{

	int ret, i;

	/* Exit sequence as follow */
	/* Step.a Stream Off */
	ret = sample_framesource_streamoff();
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "FrameSource StreamOff failed\n");
		return -1;
	}

	/* Step.b UnBind */
	for (i = 0; i < FS_CHN_NUM; i++) {
		if (chn[i].enable) {
			ret = IMP_System_UnBind(&chn[i].framesource_chn, &chn[i].imp_encoder);
			if (ret < 0) {
				IMP_LOG_ERR(TAG, "UnBind FrameSource channel%d and Encoder failed\n",i);
				return -1;
			}
		}
	}

	/* Step.c Encoder exit */
	ret = sample_encoder_exit();
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "Encoder exit failed\n");
		return -1;
	}

	/* Step.d FrameSource exit */
	ret = sample_framesource_exit();
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "FrameSource exit failed\n");
		return -1;
	}

	/* Step.e System exit */
	ret = sample_system_exit();
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "sample_system_exit() failed\n");
		return -1;
	}

	ret = IMP_Encoder_StopRecvPic(0);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "IMP_Encoder_StopRecvPic() failed\n");
		return -1;
	}

	return 0;
}

static int save_stream(int fd, IMPEncoderStream *stream)
{
	unsigned int ret;
	int i, nr_pack = stream->packCount;

	for (i = 0; i < nr_pack; i++) {
		ret = write(fd, (void *)stream->pack[i].virAddr,
					stream->pack[i].length);
		if (ret != stream->pack[i].length){
			IMP_LOG_ERR(TAG,"stream write error:%s\n", strerror(errno));
			return -1;
		}
	}

	return 0;
}

static int get_h264_stream(int fd, int chn)
{
	int ret;
	
	/* Polling H264 Stream, set timeout as 1000msec */
	ret = IMP_Encoder_PollingStream(chn, 100);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "Polling stream timeout\n");
	}

	IMPEncoderStream stream;
	/* Get H264 Stream */
	ret = IMP_Encoder_GetStream(chn, &stream, 1);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "IMP_Encoder_GetStream() failed\n");
		return -1;
	}
	
	ret = save_stream(fd, &stream);
	if (ret < 0) {
		close(fd);
		return ret;
	}
	
	IMP_Encoder_ReleaseStream(chn, &stream);

	return 0;
}

void *get_stream(int fd, int chn)
{
	int  ret;
	
	ret = IMP_Encoder_StartRecvPic(chn);
	if (ret < 0){
		IMP_LOG_ERR(TAG, "IMP_Encoder_StartRecvPic(%d) failed\n", 1);
		return NULL;
	}
	ret = get_h264_stream(fd, chn);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "Get H264 stream failed\n");
		return NULL;
	}
/*	ret = IMP_Encoder_StopRecvPic(chn);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "IMP_Encoder_StopRecvPic() failed\n");
		return NULL;
	}
*/
	return 0;
}

int capture_and_encoding()
{
	int ret = 0;
	int i = 0;

	
	printf(">>>>>caputre_and_encoding start\n");


	ret = sample_system_init();
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "IMP_System_Init() failed\n");
		return -1;
	}

	/* Step.2 FrameSource init */
	ret = sample_framesource_init();
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "FrameSource init failed\n");
		return -1;
	}

	for (i = 0; i < FS_CHN_NUM; i++) {
		if (chn[i].enable) {
			ret = IMP_Encoder_CreateGroup(chn[i].index);
			if (ret < 0) {
				IMP_LOG_ERR(TAG, "IMP_Encoder_CreateGroup(%d) error !\n", i);
				return -1;
			}
		}
	}

	/* Step.3 Encoder init */
	ret = sample_encoder_init();
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "Encoder init failed\n");
		return -1;
	}

	/* Step.4 Bind */
	for (i = 0; i < FS_CHN_NUM; i++) {
		if (chn[i].enable) {
			ret = IMP_System_Bind(&chn[i].framesource_chn, &chn[i].imp_encoder);
			if (ret < 0) {
				IMP_LOG_ERR(TAG, "Bind FrameSource channel%d and Encoder failed\n",i);
				return -1;
			}
		}
	}

	/* Step.5 Stream On */
	ret = sample_framesource_streamon();
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "ImpStreamOn failed\n");
		return -1;
	}

	return 0;
}
