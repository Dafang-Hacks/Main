/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** ALSACapture.h
** 
** V4L2 RTSP streamer                                                                 
**                                                                                    
** ALSA capture overide of V4l2Capture
**                                                                                    
** -------------------------------------------------------------------------*/

#ifndef ALSA_CAPTURE
#define ALSA_CAPTURE


#include "lame.h"
#include "logger.h"
#include "../../opus/include/opus.h"
#include "../../v4l2rtspserver-tools/sharedmem.h"
typedef enum
{
    ENCODE_MP3,
    ENCODE_OPUS,
    ENCODE_PCM,
    ENCODE_ULAW,
} audioencoding;

typedef enum
{
    SOURCE_IMP,
    SOURCE_DSP,
} audioSource;


struct ALSACaptureParameters 
{
	ALSACaptureParameters(const char* devname,  unsigned int inSampleRate, unsigned int outSampleRate, int verbose, audioencoding encode) :
		m_devName(devname), m_inSampleRate(inSampleRate),m_outSampleRate(outSampleRate), m_channels(1), m_verbose(verbose), m_encode(encode) {};


	std::string      m_devName;
	unsigned int     m_inSampleRate;
	unsigned int     m_outSampleRate;
	unsigned int     m_channels; //Always 1
	int              m_verbose;
	audioencoding    m_encode;

};

// Todo calculate the buffer depending on the inpus format + audio format
#define RECBUF_SIZE		44100*2*1
class ALSACapture
{
	public:
		static ALSACapture* createNew(const ALSACaptureParameters & params) ;
		virtual ~ALSACapture();
		void close();
	
	protected:
		ALSACapture(const ALSACaptureParameters & params);
		size_t readMP3(char* buffer, size_t bufferSize, int volume);
		size_t readOpus(char* buffer, size_t bufferSize, int volume);
		size_t readPCM(char* buffer, size_t bufferSize, int volume);
		size_t readULAW(char* buffer, size_t bufferSize, int volume);

		// This is non object progamming, sorry ...
		size_t readMP3IMP(char* buffer, size_t bufferSize, int volume);
        size_t readOpusIMP(char* buffer, size_t bufferSize, int volume);
        size_t readPCMIMP(char* buffer, size_t bufferSize, int volume);
        size_t readULAWIMP(char* buffer, size_t bufferSize, int volume);

        void udpateHWVolume(unsigned int newVol);
        void UpdateIMPFilter();
        short filter(short val,bool swap, int num_sample =0);
        void setSwVolume(short &val, int vol);
        unsigned char ulaw_encode(short sample);

        ssize_t readAudio(int fd, void *buf, size_t count);

        void initAudioIMP(const ALSACaptureParameters & params);
        void initAudio(const ALSACaptureParameters & params);

	public:
		virtual size_t read(char* buffer, size_t bufferSize);		
		virtual int getFd();
		
		virtual unsigned long getBufferSize();// { return RECBUF_SIZE; };
		virtual int getWidth()  {return -1;}
		virtual int getHeight() {return -1;}	
		
		unsigned long getInSampleRate() { return m_params.m_inSampleRate; };
		unsigned long getOutSampleRate() { return m_params.m_outSampleRate; };
		unsigned long getChannels  () { return m_params.m_channels; };
		
	private:
		unsigned long         m_bufferSize;
		unsigned long         m_periodSize;
		ALSACaptureParameters m_params;
        int fd;
		lame_global_flags *gfp;
		OpusEncoder *encoder;
		SharedMem m_sharedMem;
		struct shared_conf m_currentConfig;
		shared_conf *m_newConfig;
		int m_Filtermethod;
		bool m_HighFiltermethod;
		bool m_AECFiltermethod;
		audioSource m_audioSource;

};

#endif


