/**********
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version. (See <http://www.gnu.org/copyleft/lesser.html>.)

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********/
// Copyright (c) 1996-2018, Live Networks, Inc.  All rights reserved
// A test program that reads a H.264 Elementary Stream video file
// and streams it using RTP
// main program
//
// NOTE: For this application to work, the H.264 Elementary Stream video file *must* contain SPS and PPS NAL units,
// ideally at or near the start of the file.  These SPS and PPS NAL units are used to specify 'configuration' information
// that is set in the output stream's SDP description (by the RTSP server that is built in to this application).
// Note also that - unlike some other "*Streamer" demo applications - the resulting stream can be received only using a
// RTSP client (such as "openRTSP")

#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>
#include <GroupsockHelper.hh>
#include <sys/types.h>  
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

#define LOGURU_IMPLEMENTATION 1
#include "loguru.hpp"
 
//#include "capture_and_encoding.h"
#include "ImpEncoder.h"
UsageEnvironment* env;
char const* inputFileName = "/tmp/h264_fifo";
H264VideoStreamFramer* videoSource;
RTPSink* videoSink;

void play(); // forward
void destroy();
EventTriggerId DeviceSource::eventTriggerId = 0;


// -----------------------------------------
//    create UserAuthenticationDatabase for RTSP server
// -----------------------------------------
UserAuthenticationDatabase* createUserAuthenticationDatabase(const std::list<std::string> & userPasswordList, const char* realm)
{
        UserAuthenticationDatabase* auth = NULL;
        if (userPasswordList.size() > 0)
        {
                auth = new UserAuthenticationDatabase(realm, (realm != NULL) );

                std::list<std::string>::const_iterator it;
                for (it = userPasswordList.begin(); it != userPasswordList.end(); ++it)
                {
                        std::istringstream is(*it);
                        std::string user;
                        getline(is, user, ':');
                        std::string password;
                        getline(is, password);
                        auth->addUserRecord(user.c_str(), password.c_str());
                }
        }

        return auth;
}



void* encode(void *param)
{
	ImpEncoder((impParams*)param);
	int fd = open(inputFileName, O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd < 0) 
	{
		*env << "Failed open fifo\n";
		exit(1);; 
	}   
	while (1) 
	{
		get_stream(fd ,0);
	}


}

int main(int argc, char** argv, char**environ) 
{
        int width = 1280;
        int height = 720;
        int rcmode = ENC_RC_MODE_VBR;
        int fps = 25;
	char url[256] = "unicast";



        loguru::set_thread_name("main thread");
	loguru::g_stderr_verbosity = 10;

	LOG_S(INFO) << "=== Dumping environment variables ===";
	while (*environ)
	{
		LOG_S(INFO) << *environ;
		environ++;
	}
	LOG_S(INFO) << "=== end ===";




	// Begin by setting up our usage environment:
	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	env = BasicUsageEnvironment::createNew(*scheduler);

	// Create 'groupsocks' for RTP and RTCP:
	struct in_addr destinationAddress;
	destinationAddress.s_addr = chooseRandomIPv4SSMAddress(*env);
	// Note: This is a multicast address.  If you wish instead to stream
	// using unicast, then you should use the "testOnDemandRTSPServer"
	// test program - not this test program - as a model.

	const unsigned short rtpPortNum = 18888;
	const unsigned short rtcpPortNum = rtpPortNum+1;
	const unsigned char ttl = 255;

	const Port rtpPort(rtpPortNum);
	const Port rtcpPort(rtcpPortNum);

	int fd;	
	const char *realm = NULL;
	std::list <std::string> userPasswordList;
	 unsigned short rtspPort = 8554;
	 unsigned short rtspOverHTTPPort = 0;
	 bool multicast = false;
	 int verbose = 0;
	 std::string outputFile;


	impParams params;
	params.width = width;
        params.height = height;
        params.rcmode = rcmode;
	params.mode = IMP_MODE_H264_SNAP;
        params.framerate = fps;
        params.bitrate = (double)2000.0 * (width * height) / (1280 * 720);



	// decode parameters
	int c = 0;
	while ((c = getopt(argc, argv, "v::Q:O:b:" "I:P:p:m:u:M:ct:S::" "R:U:" "nwsf::F:W:H:r:" "AC:a:E:" "Vh")) != -1) 
	{
		switch (c) {
            case 'v':
                verbose = 1;
                        loguru::g_stderr_verbosity = 10;
                if (optarg && *optarg == 'v') verbose++;
                break;
            case 'O':
                outputFile = optarg;
                break;
//            case 'b':       webroot = optarg; break;

                // RTSP/RTP
            case 'I':
                ReceivingInterfaceAddr = inet_addr(optarg);
                break;
            case 'P':
                rtspPort = atoi(optarg);
                break;
/*            case 'p':
                rtspOverHTTPPort = atoi(optarg);
                break;*/
            case 'u':
                strcpy(url, optarg);
                break;
//            case 'm':
//               multicast = true;
//               murl = optarg;
//              break;
//            case 'M':
//                multicast = true;
//                maddr = optarg;
//                break;
//            case 'c':
//                repeatConfig = false;
//                break;
//            case 't':
//                timeout = atoi(optarg);
//                break;
//            case 'S':
//                hlsSegment = optarg ? atoi(optarg) : defaultHlsSegment;
//                break;
                // users
            case 'R':
                realm = optarg;
                break;
            case 'U':
                userPasswordList.push_back(optarg);
                break;
/*            case 's':
                useThread = false;
                break;
            case 'f':
                format = decodeVideoFormat(optarg);
                break;*/
            case 'F':
                fps = atoi(optarg);
                break;
            case 'W':
                width = atoi(optarg);
                break;
            case 'H':
                height = atoi(optarg);
                break;
            case 'r':
                rcmode = atoi(optarg);
                break;
/*            case 'A':   disableAudio = true; break;
            case 'a':   alsaAudio = true; break;
            case 'E':   decodeEncodeFormat(optarg,encode,inAudioFreq, outAudioFreq); break;
*/
            // help
            case 'h':
            default: {
                std::cout << argv[0] << " [-v[v]] [-O file]" << std::endl;
                std::cout
                        //<< "\t          [-I interface] [-P RTSP port] [-p RTSP/HTTP port] [-m multicast url] [-u unicast url] [-M multicast addr] [-c] [-t timeout] [-T] [-S[duration]]"
                        << "\t          [-P RTSP port] [-u unicast url]"
                        << std::endl;
                std::cout << "\t          [-r] [-w] [-s] [-f[format] [-W width] [-H height] [-F fps]"
                          << std::endl;
//                std::cout << "\t -v        : verbose" << std::endl;
//                std::cout << "\t -vv       : very verbose" << std::endl;
                std::cout << "\t -O output : Copy captured frame to stdout" << std::endl;
//                std::cout << "\t -b <webroot>     : path to webroot" << std::endl;

/*                std::cout << "\t RTSP/RTP options :" << std::endl;
                std::cout << "\t -I addr   : RTSP interface (default autodetect)" << std::endl;
                std::cout << "\t -P port   : RTSP port (default " << rtspPort << ")" << std::endl;
                std::cout << "\t -p port   : RTSP over HTTP port (default " << rtspOverHTTPPort << ")" << std::endl;
                std::cout << "\t -U user:password : RTSP user and password" << std::endl;
                std::cout << "\t -R realm  : use md5 password 'md5(<username>:<realm>:<password>')" << std::endl;
                std::cout << "\t -u url    : unicast url (default " << url << ")" << std::endl;
                std::cout << "\t -m url    : multicast url (default " << murl << ")" << std::endl;
                std::cout << "\t -M addr   : multicast group:port (default is random_address:20000)" << std::endl;
                std::cout << "\t -c        : don't repeat config (default repeat config before IDR frame)" << std::endl;
                std::cout << "\t -t timeout: RTCP expiration timeout in seconds (default " << timeout << ")"
                          << std::endl;
                std::cout << "\t -S[duration]: enable HLS & MPEG-DASH with segment duration  in seconds (default "
                          << defaultHlsSegment << ")" << std::endl;

                std::cout << "\t -fformat  : capture using format (-W,-H,-F are used)" << std::endl;
                std::cout << "\t -W width  : capture width (default " << width << ")" << std::endl;
                std::cout << "\t -H height : capture height (default " << height << ")" << std::endl;
                std::cout << "\t -F fps    : capture framerate (default " << fps << ")" << std::endl;
                std::cout << "\t -r mode   : encode mode (0 = FixedQp, 1 = CBR, 2 = VBR, 3 = SMART, default = " << rcmode << ")" << std::endl;

                std::cout << "\t Sound options :" << std::endl;
                std::cout << "\t -A     : Disable audio"<< std::endl;
                std::cout << "\t -a     : Use ALSA driver instead of IMP sdk"<< std::endl;
                std::cout << "\t -E EncodeFormat:inSampleRate:OutSampleRate "<< std::endl;
                std::cout << "\t\tEncodeFormat:in MP3 | OPUS | PCM | PCMU"<< std::endl;
                std::cout << "\t\tOutSampleRate: output sample rate (forced to 48000 for OPUS, OutSampleRate is forced to 8000 for PCM and PCMU)"<< std::endl;
*/
                exit(0);
            }
          }
	}
                                                                          

	UserAuthenticationDatabase* auth = createUserAuthenticationDatabase(userPasswordList, realm);

	unlink(inputFileName);
	  
	if (mkfifo(inputFileName, 0777) < 0) {
			*env << "mkfifo Failed\n";
			exit(1);; 
	}
	pthread_t thread;
    	if(pthread_create(&thread, NULL, encode, &params) == -1) 
	{
		perror("pthread_create");
		return EXIT_FAILURE;
    	}

	Groupsock rtpGroupsock(*env, destinationAddress, rtpPort, ttl);
	rtpGroupsock.multicastSendOnly(); // we're a SSM source
	Groupsock rtcpGroupsock(*env, destinationAddress, rtcpPort, ttl);
	rtcpGroupsock.multicastSendOnly(); // we're a SSM source

	// Create a 'H264 Video RTP' sink from the RTP 'groupsock':
	OutPacketBuffer::maxSize = 600000;
	videoSink = H264VideoRTPSink::createNew(*env, &rtpGroupsock, 96);
	// Create (and start) a 'RTCP instance' for this RTP sink:
	const unsigned estimatedSessionBandwidth = 10000; // in kbps; for RTCP b/w share
	const unsigned maxCNAMElen = 100;
	unsigned char CNAME[maxCNAMElen+1];
	gethostname((char*)CNAME, maxCNAMElen);
	CNAME[maxCNAMElen] = '\0'; // just in case
	RTCPInstance* rtcp = RTCPInstance::createNew(*env, &rtcpGroupsock, estimatedSessionBandwidth, CNAME, videoSink, NULL /* we're a server */, True /* we're a SSM source */);
	// Note: This starts RTCP running automatically

	RTSPServer* rtspServer = RTSPServer::createNew(*env, rtspPort, auth);
	if (rtspServer == NULL) {
		*env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
		exit(1);
	}
	ServerMediaSession* sms  = ServerMediaSession::createNew(*env, url, inputFileName, "Session streamed by \"DafangHack\"",   True /*SSM*/);
	sms->addSubsession(PassiveServerMediaSubsession::createNew(*videoSink, rtcp));
	rtspServer->addServerMediaSession(sms);

	char* urlDisplay = rtspServer->rtspURL(sms);
	*env << "Play this stream using the URL \"" << urlDisplay << "\"\n";
	delete[] urlDisplay;

	// Start the streaming:
	*env << "Beginning streaming...\n";
	play();

	env->taskScheduler().doEventLoop(); // does not return
	return 0; // only to prevent compiler warning
}

void afterPlaying(void* /*clientData*/) 
{
	*env << "...done reading from file\n";
	videoSink->stopPlaying();
	Medium::close(videoSource);

	//destroy();
	// Note that this also closes the input file that this source read from.

	// Start playing once again:
	play();
}

void play() 
{
	// Open the input file as a 'byte-stream file source':
	ByteStreamFileSource* fileSource
		= ByteStreamFileSource::createNew(*env, inputFileName);
	if (fileSource == NULL) {
		*env << "Unable to open file \"" << inputFileName
			<< "\" as a byte-stream file source\n";
		exit(1);
	}

	FramedSource* videoES = fileSource;

	// Create a framer for the Video Elementary Stream:
	videoSource = H264VideoStreamFramer::createNew(*env, videoES);

	// Finally, start playing:
	*env << "Beginning to read from file...\n";
	videoSink->startPlaying(*videoSource, afterPlaying, videoSink);
}
