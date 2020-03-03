#ifndef SOAP_HELPERS_HEADER
#define SOAP_HELPERS_HEADER

#include <vector>
#include "system_utils.h"
#include "../generated/soapStub.h"

#define MAIN_PROFILE 			"main-profile"

#define VIDEO_SRC 				"h264-src"
#define VIDEO_CFG 				"video-cfg"
#define VIDEO_ENC_H264_CFG 		"h264-cfg"
#define VIDEO_ENC_JPEG_CFG 		"jpeg-cfg"

#define AUDIO_SRC 				"audio-src"
#define AUDIO_CFG 				"audio-cfg"
#define AUDIO_ENC_G711_CFG 		"g711-cfg"
#define AUDIO_OUT_CFG 			"audio-out-cfg"
#define AUDIO_OUTPUT 			"audio-output"

#define PTZ_CFG 				"ptz-cfg"
#define PTZ_NODE 				"ptz-node"

#define META_DATA 				"meta"

template<typename T>
T* soap_new_ptr(struct soap* soap, T value)
{
	T* ptr = (T*)soap_malloc(soap, sizeof(T));
	*ptr = value;

	return ptr;
}

tds__DeviceServiceCapabilities * getDeviceServiceCapabilities(soap * soap);
trt__Capabilities * getMediaServiceCapabilities(soap *soap);
timg__Capabilities * getImagingServiceCapabilities(soap * soap);
tptz__Capabilities * getPTZServiceCapabilities(soap * soap);
// trc__Capabilities * getRecordingServiceCapabilities(soap * soap);
// tse__Capabilities * getSearchServiceCapabilities(soap * soap);
trv__Capabilities * getReceiverServiceCapabilities(soap * soap);
// trp__Capabilities * getReplayServiceCapabilities(soap * soap);
// tev__Capabilities * getEventServiceCapabilities(soap * soap);
// tls__Capabilities * getDisplayServiceCapabilities(soap * soap);
tmd__Capabilities * getDeviceIOServiceCapabilities(soap * soap);

std::vector<tt__Profile *> getProfiles(soap * soap);

std::vector<tt__PTZConfiguration *> getPTZConfigurations(soap * soap);
tt__PTZSpaces * getPTZSpaces(soap * soap);
std::vector<tt__PTZNode *> getPTZNodes(soap * soap);

std::vector<tt__VideoSource *> getVideoSources(soap * soap);
std::vector<tt__VideoSourceConfiguration *> getVideoSourceConfigurations(soap * soap);
std::vector<tt__VideoEncoderConfiguration *> getVideoEncoderConfigurations(soap * soap);

std::vector<tt__AudioSourceConfiguration *> getAudioSourceConfigurations(soap * soap);
std::vector<tt__AudioOutputConfiguration *> getAudioOutputConfigurations(soap * soap);
std::vector<tt__AudioEncoderConfiguration *> getAudioEncoderConfigurations(soap * soap);

std::vector<tt__MetadataConfiguration *> getMetadataConfigurations(soap * soap);
tt__MulticastConfiguration * getMulticastConfiguration( soap * soap );
tt__IPAddress * getIpAddress( soap * soap );
std::vector<tt__VideoResolution *> getVideoResolutions( soap * soap );

#endif
