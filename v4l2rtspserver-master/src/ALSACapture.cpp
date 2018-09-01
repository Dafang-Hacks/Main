/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** ALSACapture.cpp
** 
** V4L2 RTSP streamer                                                                 
**                                                                                    
** ALSA capture overide of V4l2Capture
**                                                                                    
** -------------------------------------------------------------------------*/

#include "ALSACapture.h"
#include "soundcard.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <loguru.hpp>

/**
 * lame_error_callback, lame_message_callback, lame_debug_callback: LAME
 * logging callback functions.
 *
 * [Parameters]
 *     format: Format string.
 *     args: Format arguments.
 */
static void lame_error_callback(const char *format, va_list args)
{

    LOG_F(ERROR,format, args);
}

static void lame_message_callback(const char *format, va_list args)
{
    LOG_F(INFO,format, args);
}


extern "C" {
#include <wave.h>
#include <noise_remover.h>
#include "filt.h"
}
/* Tested configuration
+--------------+---------------+--------------+--------------+--------------+--------------+
|      Audio   |    inSample   |   outSample  |    Filter    |   S/W Vol    |   H/W Vol    |
+--------------+---------------+--------------+--------------+--------------+--------------+
|      MP3     |    8000       |     8000     |     1,2      |    yes       |    yes       |
|              |    8000       |     44100    |     1,2      |    yes       |    yes       |
|              |    44100      |     44100    |     1,2      |    yes       |    yes       |
+--------------+---------------+--------------+--------------+--------------+--------------+
|      OPUS    |    8000       |     48000    |     1,2      |    yes       |    yes       |
|              |    48000      |     48000    |     1,2      |    yes       |    yes       |
+--------------+---------------+--------------+--------------+--------------+--------------+
|      PCM     |    8000       |      8000    |     1,2      |    yes       |    yes       |
+--------------+---------------+--------------+--------------+--------------+--------------+
|      PCMU    |    8000       |      8000    |     1,2      |    yes       |    yes       |
+--------------+---------------+--------------+--------------+--------------+--------------+
*/


// ------------------------------------------------------
// IMP Audio
#include <imp/imp_audio.h>

void ALSACapture::initAudioIMP(const ALSACaptureParameters & params)
{
	int devID = 1;
	IMPAudioIOAttr attr;

	attr.samplerate = (IMPAudioSampleRate) params.m_inSampleRate;
	attr.bitwidth = AUDIO_BIT_WIDTH_16;
	attr.soundmode = AUDIO_SOUND_MODE_MONO;
	attr.frmNum = 50;
	attr.numPerFrm = getBufferSize()/10;

	attr.chnCnt = 1;
	int ret = IMP_AI_SetPubAttr(devID, &attr);
	if(ret != 0) {
		LOG_F(ERROR,"set ai %d attr err: %d", devID, ret);
		exit(0);
		return ;
	}

	memset(&attr, 0x0, sizeof(attr));
	ret = IMP_AI_GetPubAttr(devID, &attr);
	if(ret != 0) {
		LOG_F(ERROR, "get ai %d attr err: %d", devID, ret);
		return ;
	}

	LOG_F(INFO, "Audio In GetPubAttr samplerate : %d", attr.samplerate);
	LOG_F(INFO, "Audio In GetPubAttr   bitwidth : %d", attr.bitwidth);
	LOG_F(INFO, "Audio In GetPubAttr  soundmode : %d", attr.soundmode);
	LOG_F(INFO, "Audio In GetPubAttr     frmNum : %d", attr.frmNum);
	LOG_F(INFO, "Audio In GetPubAttr  numPerFrm : %d", attr.numPerFrm);
	LOG_F(INFO, "Audio In GetPubAttr     chnCnt : %d", attr.chnCnt);


	/* Step 2: enable AI device. */
	ret = IMP_AI_Enable(devID);
	if(ret != 0) {
		LOG_F(ERROR, "enable ai %d err", devID);
		return ;
	}

	/* Step 3: set audio channel attribute of AI device. */
	int chnID = 0;
	IMPAudioIChnParam chnParam = {0};
	chnParam.usrFrmDepth = 50;
	ret = IMP_AI_SetChnParam(devID, chnID, &chnParam);
	if(ret != 0) {
		LOG_F(ERROR, "set ai %d channel %d attr err: %d", devID, chnID, ret);
		return ;
	}

	memset(&chnParam, 0x0, sizeof(chnParam));
	ret = IMP_AI_GetChnParam(devID, chnID, &chnParam);
	if(ret != 0) {
		LOG_F(ERROR, "get ai %d channel %d attr err: %d", devID, chnID, ret);
		//return ;
	}

	LOG_F(INFO, "Audio In GetChnParam usrFrmDepth : %d", chnParam.usrFrmDepth);

	/* Step 4: enable AI channel. */
	ret = IMP_AI_EnableChn(devID, chnID);
	if(ret != 0) {
		LOG_F(ERROR, "Audio Record enable channel failed");
		return ;
	}

    int chnVol = 70;
    if (m_newConfig->hardVolume != -1)
    {
        chnVol = m_newConfig->hardVolume;
        m_currentConfig.hardVolume = m_newConfig->hardVolume;
    }

	/* Step 5: Set audio channel volume. */
	ret = IMP_AI_SetVol(devID, chnID, chnVol);
	if(ret != 0) {
		LOG_F(ERROR, "Audio Record set volume failed");
	}

	ret = IMP_AI_GetVol(devID, chnID, &chnVol);
	if(ret != 0) {
		LOG_F(ERROR, "Audio Record get volume failed");
		//return ;
	}
    LOG_F(INFO,"Audio In GetVol    vol : %d", chnVol);


   UpdateIMPFilter();
}

void ALSACapture::initAudio(const ALSACaptureParameters & params)
{
    LOG_F(INFO, "Open ALSA device: %s", params.m_devName.c_str() );


    if ((fd = ::open(params.m_devName.c_str(), O_RDONLY, 0)) == -1)
    {
        LOG_F(ERROR,"cannot open audio device: %s", params.m_devName.c_str());
    }

    int format= AFMT_S16_LE;
    if (::ioctl(fd, SNDCTL_DSP_SETFMT, &format)==-1)
    { // Fatal error
         LOG_F(ERROR,"Cant set format...%s", params.m_devName.c_str());
    }

    int stereo = params.m_channels-1;
    LOG_F(INFO,"Channel Count: %d", params.m_channels);
    if (::ioctl(fd, SNDCTL_DSP_STEREO, &stereo)==-1)
    { // Fatal error
       LOG_F(ERROR,"Cant set Mono/Stereo ...%s", params.m_devName.c_str());
    }

    int speed =  params.m_inSampleRate;

    if (ioctl(fd, SNDCTL_DSP_SPEED, &speed)==-1)
    { // Fatal error
         LOG_F(ERROR, "Cant set Speed ...%s",params.m_devName.c_str());
    }

    if (m_newConfig->hardVolume != -1)
    {
        if (ioctl(fd, SNDCTL_EXT_SET_RECORD_VOLUME, &m_newConfig->hardVolume)==-1)
        { // Fatal error
            LOG_F(ERROR, "Cant set vol %d", m_newConfig->hardVolume);
        }
        m_currentConfig.hardVolume = m_newConfig->hardVolume;
    }
}

ALSACapture* ALSACapture::createNew(const ALSACaptureParameters & params) 
{ 
    ALSACapture* capture = new ALSACapture(params);
    if (capture)
    {
        if (capture->getFd() == -1)
        {
            delete capture;
            capture = NULL;
        }
    }
    return capture;
}

ALSACapture::~ALSACapture()
{
    this->close();
}

void ALSACapture::close()
{

}

ALSACapture::ALSACapture(const ALSACaptureParameters & params) : m_bufferSize(0), m_periodSize(0), m_params(params)
{
    m_sharedMem = SharedMem::instance();
    m_sharedMem.readConfig();
    m_newConfig = m_sharedMem.getConfig();

    memcpy(&m_currentConfig, m_newConfig, sizeof(shared_conf));

    m_audioSource = params.m_source;

    if (m_audioSource == SOURCE_IMP)
    {
        initAudioIMP(params);
    } else {
        initAudio(params);
    }

    switch (params.m_encode)
    {
    case ENCODE_OPUS:
    {
#define APPLICATION OPUS_APPLICATION_VOIP
       int err =0;
        /*Create a new encoder state */
       encoder = opus_encoder_create(params.m_inSampleRate, params.m_channels, APPLICATION, &err);
       if (err<0)
       {
          LOG_F(ERROR, "failed to create an encoder: %s", opus_strerror(err));
       }

       /* Set the desired bit-rate. You can also set other parameters if needed.
          The Opus library is designed to have good defaults, so only set
          parameters you know you need. Doing otherwise is likely to result
          in worse quality, but better. */

       opus_encoder_ctl(encoder, OPUS_SET_BITRATE(params.m_outSampleRate));
       opus_encoder_ctl(encoder, OPUS_SET_COMPLEXITY(0));
       break;
      }
    case ENCODE_MP3:
    {
       // Lame Init:
        gfp = lame_init();
        lame_set_errorf(gfp, lame_error_callback);
        lame_set_msgf  (gfp, lame_message_callback);
        //lame_set_debugf(lame, lame_debug_callback);
        lame_set_num_channels(gfp,params.m_channels );
        //lame_set_mode(gfp, 3);
        lame_set_in_samplerate(gfp, params.m_inSampleRate);
        lame_set_out_samplerate(gfp, params.m_outSampleRate);
      //  lame_set_scale(gfp, 3.0);
        int ret_code = lame_init_params(gfp);
        if (ret_code < 0)
        { /* Fatal error */
         LOG_F(ERROR,"Cant init Lame");
        }
        //lame_print_config(gfp);
        break;
    }
    case ENCODE_PCM:
    default:
        break;
    }

}

void ALSACapture::setSwVolume(short &val, int vol)
{
 val += (short) (val * (vol / 100.0));
}


#define _SWAP(val) (val << 8) | ((val >> 8) & 0xFF);
inline signed short lowpass(signed short input, bool swap)
{
   static signed short last_sample=0;
   signed short retvalue=(input + (last_sample * 7)) >> 3;
   last_sample=retvalue;
   if (swap)
    return _SWAP(retvalue);
   return retvalue;
}

inline short ALSACapture::filter(short val,bool swap, int num_sample)
{
    if (m_Filtermethod == 1)
    {
         static struct noise_remover_s nrm;
         static bool isInit = false;
         if (isInit == false) {
            noise_remover_init( &nrm );
            isInit = true;
         }

        short y;
        /* process audio */
        val = ASHIFT16(val,-2);

        y = noise_remover ( &nrm, val, 1 );  /* training=1 */

        if( y>8192 ) // 8192 = ASHIFT16( 32768, -2 )
            y = 32767;
        else if( y<-8192 )
            y = -32768;
        else
            y = ASHIFT16(y,+2);

        return lowpass(y, swap);
    }

    if (m_Filtermethod == 2)
    {
        return lowpass(val, swap);
    }

    if (swap == true)
        return _SWAP(val);
    return val;
}

void ALSACapture::udpateHWVolume(unsigned int newVol)
{
    if (m_audioSource == SOURCE_IMP)
    {
        int devID = 1;
        int chnID = 0;
        int ret;
        ret = IMP_AI_SetVol(devID, chnID, newVol);
        if(ret != 0) {
            LOG_F(ERROR, "Audio Record set volume failed");
        }
    } else {
     if (ioctl(fd, SNDCTL_EXT_SET_RECORD_VOLUME, &newVol)==-1)
        { // Fatal error
            LOG_F(ERROR, "Cant set vol %d", newVol);
        }
    }
    LOG_F(INFO, "Set H/Wvol %d", newVol);
}

void ALSACapture::UpdateIMPFilter()
{
    int ret;
	IMPAudioIOAttr attr;
	int devID = 1;
    int chnID = 0;

    ret = IMP_AI_GetPubAttr(devID, &attr);
	if(ret != 0) {
		LOG_F(ERROR, "get ai %d attr err: %d", devID, ret);
	}
    if ( m_newConfig->highfilter == true)
    {
	    ret = IMP_AI_EnableHpf(&attr);
	    if(ret != 0) {
	    	LOG_F(ERROR, "Enable audio hpf error.");
	    }
	    LOG_F(INFO, "Enabled highfilter");
    }
    else
    {
        ret = IMP_AI_DisableHpf();
        if(ret != 0) {
            LOG_F(ERROR, "Disable audio hpf error.");
        }
        LOG_F(INFO, "Disabled highfilter");
    }

    if ( m_newConfig->aecfilter == true)
    {
        if (m_params.m_inSampleRate <= 16000)
        {
            // Enable Aec
            ret = IMP_AI_EnableAec(devID, chnID, 0, 0);
            if(ret != 0) {
                LOG_F(ERROR, "Audio enable aec failed");
            }
            LOG_F(INFO, "Enabled aec");
        }
        else
        {
         LOG_F(ERROR, "Can't enable aec for bitrate > 16000");
        }
    }
    else
    {
        ret = IMP_AI_DisableAec(devID, chnID);
        if(ret != 0) {
            LOG_F(ERROR, "Disable aec failed");
        }
        LOG_F(INFO, "Disabled aec");
    }

	// This filter start at 3
	if (m_newConfig->filter >= 3)
	{
	    if (m_params.m_inSampleRate <= 16000)
        {
            ret = IMP_AI_EnableNs(&attr, m_newConfig->filter-3); //NS_VERYHIGH);
            if(ret != 0) {
                LOG_F(ERROR, "enable audio ns error.");
            }
            LOG_F(INFO, "NS Filter=%d", m_newConfig->filter-3);
        }
        else
        {
            LOG_F(ERROR, "Can't enable this filter for bitrate > 16000");
        }
    }
    else
    {
        // This API crashes, so set to the minimum
        ret = IMP_AI_EnableNs(&attr, NS_LOW); //NS_VERYHIGH);
        //ret = IMP_AI_DisableNs();
        if(ret != 0) {
            LOG_F(ERROR, "disable audio ns error.");
        }
        LOG_F(INFO, "NS Filter disabled");
    }

}

size_t ALSACapture::read(char* buffer, size_t bufferSize)
{
    m_sharedMem.readConfig();
    if (m_currentConfig.hardVolume != m_newConfig->hardVolume) {
        udpateHWVolume( m_newConfig->hardVolume);
        m_currentConfig.hardVolume = m_newConfig->hardVolume;
    }
    if (m_audioSource == SOURCE_IMP)
    {
        if (m_Filtermethod != m_newConfig->filter
           || m_HighFiltermethod != m_newConfig->highfilter
           || m_AECFiltermethod != m_newConfig->aecfilter)
        {
           UpdateIMPFilter();
        }
    }
    m_Filtermethod = m_newConfig->filter;
    m_HighFiltermethod = m_newConfig->highfilter;
    m_AECFiltermethod = m_newConfig->aecfilter;
    switch (m_params.m_encode)
    {
        case ENCODE_OPUS:
            if (m_audioSource == SOURCE_IMP)
                return readOpusIMP(buffer, bufferSize, m_newConfig->softVolume);

            return readOpus(buffer, bufferSize, m_newConfig->softVolume);
            break;
        case ENCODE_MP3:
            if (m_audioSource == SOURCE_IMP)
                return readMP3IMP(buffer, bufferSize, m_newConfig->softVolume);
            return readMP3(buffer, bufferSize, m_newConfig->softVolume);
            break;
        case ENCODE_PCM:
            if (m_audioSource == SOURCE_IMP)
                return readPCMIMP(buffer, bufferSize, m_newConfig->softVolume);

            return readPCM(buffer, bufferSize, m_newConfig->softVolume);
            break;
        case ENCODE_ULAW:
            if (m_audioSource == SOURCE_IMP)
                return readULAWIMP(buffer, bufferSize, m_newConfig->softVolume);

            return readULAW(buffer, bufferSize, m_newConfig->softVolume);
            break;

    }
    return 0;
}

unsigned long ALSACapture::getBufferSize()
{
  switch (m_params.m_encode)
    {
        case ENCODE_OPUS:
            if (m_audioSource == SOURCE_IMP)
                return (m_params.m_inSampleRate*2 /50)*3*10;
            return (m_params.m_inSampleRate*2 /50)*6;
            break;
        case ENCODE_MP3:
            return m_params.m_inSampleRate * sizeof(short) * 1;
            break;
        case ENCODE_PCM:
            return (m_params.m_inSampleRate*0.02)*sizeof(short)*10 *2;
            break;
        case ENCODE_ULAW:
            return (m_params.m_inSampleRate*0.02)*sizeof(short)*10;
            break;
    }
    return 1;
}

size_t ALSACapture::readULAW(char* buffer, size_t bufferSize, int volume)
{
    int num_samples = bufferSize / sizeof(short);
    short localBuffer[ num_samples ];
    // Read 10 packets of 20ms
    int bytesRead = ::read (fd, &localBuffer, bufferSize); //(m_params.m_inSampleRate*0.02)*sizeof(short)*10);
    num_samples = bytesRead / sizeof(short);

    for (int i =0; i<  num_samples ; i++)
    {
        if (volume != -1) setSwVolume(((signed short*)localBuffer)[i], volume);
        buffer[i] = ulaw_encode(filter(((signed short*)localBuffer)[i], false, num_samples));
    }

    return num_samples;
}

size_t ALSACapture::readPCM(char* buffer, size_t bufferSize, int volume)
{
    int num_samples = bufferSize / sizeof(short);
    short localBuffer[ num_samples ];
    // Read 10 packets of 20ms
    int bytesRead = ::read (fd, &localBuffer, bufferSize); //(m_params.m_inSampleRate*0.02)*sizeof(short)*10);
    num_samples = bytesRead / sizeof(short);

    for (int i =0; i<  num_samples ; i++)
    {
        if (volume != -1) setSwVolume(((signed short*)localBuffer)[i], volume);
        ((signed short*)buffer)[i] = filter(((signed short*)localBuffer)[i], true, num_samples);
    }

    return num_samples*2;
}

size_t ALSACapture::readOpus(char* buffer, size_t bufferSize, int volume)
{
    int num_samples = bufferSize / sizeof(short);
    short localBuffer[ num_samples ];
    int bytesRead = ::read (fd, &localBuffer,bufferSize); //(m_params.m_inSampleRate*2 /50)*6);
    num_samples = bytesRead / sizeof(short);


    for (int i =0; i<  num_samples ; i++)
    {
        if (volume != -1) setSwVolume(((signed short*)localBuffer)[i], volume);
        ((signed short*)localBuffer)[i] = filter(((signed short*)localBuffer)[i], false, num_samples);
    }


     /* Encode the frame. */
     bytesRead = opus_encode(encoder, localBuffer, num_samples, (unsigned char*)buffer, bufferSize);

      if (bytesRead<0)
      {
            LOG_F(ERROR, "Error converting to OPUS %d",bytesRead);
            //LOG(ERROR) << "Buffersize " << bufferSize;
            bytesRead = 1;
      }

    return bytesRead;
}

size_t ALSACapture::readMP3(char* buffer, size_t bufferSize, int volume)
{
    int num_samples = bufferSize / sizeof(short);
    short localBuffer[ num_samples ];
    int mp3buf_size = 1.25*num_samples + 7200;

    int bytesRead = ::read (fd, &localBuffer, bufferSize); //m_params.m_inSampleRate * sizeof(short) * 1); //8192*2);
    num_samples = bytesRead / sizeof(short);

    for (int i =0; i<  num_samples ; i++)
    {
        if (volume != -1) setSwVolume(((signed short*)localBuffer)[i], volume);
        ((signed short*)localBuffer)[i] = filter(((signed short*)localBuffer)[i], false, num_samples);
    }

    bytesRead = lame_encode_buffer( gfp, localBuffer, NULL,  num_samples,(unsigned char*)buffer,mp3buf_size);
    //LOG(ERROR) << "Bytes Converted to MP3:" << bytesRead;
    if(bytesRead == 0){
         LOG_F(ERROR,"Error converting to MP3");
        //LOG(ERROR) << "Buffersize " << bufferSize;
        bytesRead = 1;
    }
    return bytesRead;

}



size_t ALSACapture::readPCMIMP(char* buffer, size_t bufferSize, int volume)
{
    int num_samples = bufferSize / sizeof(short);

    int devID = 1;
    int chnID = 0;

    int ret = IMP_AI_PollingFrame(devID, chnID, 10000);
	if (ret != 0 ) {
			LOG_F(ERROR, "Audio Polling Frame Data error");
	}
	IMPAudioFrame frm;
	ret = IMP_AI_GetFrame(devID, chnID, &frm, BLOCK);
	if(ret != 0) {
			LOG_F(ERROR, "Audio Get Frame Data error");
			return 1;
	}
	num_samples = frm.len / sizeof(short);


    for (int i =0; i<  num_samples ; i++)
    {
        if (volume != -1) setSwVolume(((signed short*)frm.virAddr)[i], volume);
        ((signed short*)buffer)[i] = filter(((signed short*)frm.virAddr)[i], true, num_samples);
    }


	ret = IMP_AI_ReleaseFrame(devID, chnID, &frm);
	if(ret != 0) {
		LOG_F(ERROR,  "Audio release frame data error");
		return 1;
	}

    return num_samples*2;

}


size_t ALSACapture::readULAWIMP(char* buffer, size_t bufferSize, int volume)
{
    int num_samples = bufferSize / sizeof(short);

    int devID = 1;
    int chnID = 0;

    int ret = IMP_AI_PollingFrame(devID, chnID, 1000);
	if (ret != 0 ) {
			LOG_F(ERROR, "Audio Polling Frame Data error");
	}
	IMPAudioFrame frm;
	ret = IMP_AI_GetFrame(devID, chnID, &frm, BLOCK);
	if(ret != 0) {
			LOG_F(ERROR, "Audio Get Frame Data error");
			return 1;
	}
	num_samples = frm.len / sizeof(short);

    for (int i =0; i<  num_samples ; i++)
    {
        if (volume != -1) setSwVolume(((signed short*)frm.virAddr)[i], volume);
        buffer[i] = ulaw_encode(filter(((signed short*)frm.virAddr)[i], false, num_samples));
    }

	ret = IMP_AI_ReleaseFrame(devID, chnID, &frm);
	if(ret != 0) {
		LOG_F(ERROR,  "Audio release frame data error");
		return 1;
	}

    return num_samples;
}



size_t ALSACapture::readOpusIMP(char* buffer, size_t bufferSize, int volume)
{
   int num_samples = 0;
   int devID = 1;
   int chnID = 0;

   int ret = IMP_AI_PollingFrame(devID, chnID, 1000);
   if (ret != 0 ) {
        LOG_F(ERROR, "Audio Polling Frame Data error");
   }
   IMPAudioFrame frm;
   ret = IMP_AI_GetFrame(devID, chnID, &frm, BLOCK);
   if(ret != 0) {
    LOG_F(ERROR, "Audio Get Frame Data error");
    return 1;
   }
   num_samples = frm.len / sizeof(short);

    for (int i =0; i<  num_samples ; i++)
    {
        if (volume != -1) setSwVolume(((signed short*)frm.virAddr)[i], volume);
        ((signed short*)frm.virAddr)[i] = filter(((signed short*)frm.virAddr)[i], false, num_samples);
    }

    /* Encode the frame. */
    int bytesRead = opus_encode(encoder, (short*)frm.virAddr, num_samples, (unsigned char*)buffer, bufferSize);

    ret = IMP_AI_ReleaseFrame(devID, chnID, &frm);
	if(ret != 0) {
		LOG_F(ERROR,  "Audio release frame data error");
		return 1;
	}
    if (bytesRead<0)
    {
        LOG_F(ERROR, "Error converting to OPUS %d",bytesRead);
        //LOG(ERROR) << "Buffersize " << bufferSize;
        bytesRead = 1;
    }

    return bytesRead;
}

size_t ALSACapture::readMP3IMP(char* buffer, size_t bufferSize, int volume)
{
    int num_samples = 0;
    int mp3buf_size = 1.25*num_samples + 7200;
    int devID = 1;
    int chnID = 0;
    int ret = IMP_AI_PollingFrame(devID, chnID, 2000);
    if (ret != 0 ) {
        LOG_F(ERROR, "Audio Polling Frame Data error");
    }
    IMPAudioFrame frm;
    ret = IMP_AI_GetFrame(devID, chnID, &frm, BLOCK);
    if(ret != 0) {
        LOG_F(ERROR, "Audio Get Frame Data error");
        return 1;
    }
    num_samples = frm.len / sizeof(short);

    for (int i =0; i<  num_samples ; i++)
    {
        if (volume != -1) setSwVolume(((signed short*)frm.virAddr)[i], volume);
        ((signed short*)frm.virAddr)[i] = filter(((signed short*)frm.virAddr)[i], false, num_samples);
    }

    int bytesRead = lame_encode_buffer( gfp, (short*)frm.virAddr, NULL,  num_samples,(unsigned char*)buffer,mp3buf_size);
    //LOG(ERROR) << "Bytes Converted to MP3:" << bytesRead;
    if(bytesRead == 0){
         LOG_F(ERROR,"Error converting to MP3");
        //LOG(ERROR) << "Buffersize " << bufferSize;
        bytesRead = 1;
    }
    ret = IMP_AI_ReleaseFrame(devID, chnID, &frm);
    if(ret != 0) {
    	LOG_F(ERROR,  "Audio release frame data error");
    	return 1;
    }
    return bytesRead;

}

int ALSACapture::getFd()
{
    return fd;
}

/*=================================================================================
**	The following routines came from the sox-12.15 (Sound eXcahcnge) distribution.
**
**	This code is not compiled into libsndfile. It is only used to test the
**	libsndfile lookup tables for correctness.
**
**	I have included the original authors comments.
*/

/*
** This routine converts from linear to ulaw.
**
** Craig Reese: IDA/Supercomputing Research Center
** Joe Campbell: Department of Defense
** 29 September 1989
**
** References:
** 1) CCITT Recommendation G.711  (very difficult to follow)
** 2) "A New Digital Technique for Implementation of Any
**     Continuous PCM Companding Law," Villeret, Michel,
**     et al. 1973 IEEE Int. Conf. on Communications, Vol 1,
**     1973, pg. 11.12-11.17
** 3) MIL-STD-188-113,"Interoperability and Performance Standards
**     for Analog-to_Digital Conversion Techniques,"
**     17 February 1987
**
** Input: Signed 16 bit linear sample
** Output: 8 bit ulaw sample
*/

#define uBIAS 0x84		/* define the add-in bias for 16 bit.frames */

#define uCLIP 32635

unsigned char ALSACapture::ulaw_encode (short sample)
{	static int exp_lut [256] =
	{	0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
		} ;

	int sign, exponent, mantissa ;
	unsigned char ulawbyte ;

	/* Get the sample into sign-magnitude. */
	sign = (sample >> 8) & 0x80 ;					/* set aside the sign */
	if (sign != 0)
		sample = -sample ;							/* get magnitude */
	if (sample > uCLIP)
		sample = uCLIP ;							/* clip the magnitude */

	/* Convert from 16 bit linear to ulaw. */
	sample = sample + uBIAS ;
	exponent = exp_lut [(sample >> 7) & 0xFF] ;
	mantissa = (sample >> (exponent + 3)) & 0x0F ;
	ulawbyte = ~ (sign | (exponent << 4) | mantissa) ;

	return ulawbyte ;
} /* ulaw_encode */

