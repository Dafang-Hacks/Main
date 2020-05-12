/**
 * @Author: Sian Croser <SianLee>
 * @Date:   2020-03-03T11:37:51+10:30
 * @Email:  CQoute@gmail.com
 * @Filename: soap_helpers.c
 * @Last modified by:   Sian Croser
 * @Last modified time: 2020-03-05T13:54:02+10:30
 * @License: GPL-3
 */


#include "soap_helpers.h"
#include "../generated/soapH.h"
#include "services/ServiceContext.h"


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

using namespace std;

tds__DeviceServiceCapabilities * getDeviceServiceCapabilities(soap *soap)
{
	tds__DeviceServiceCapabilities *capabilities = soap_new_tds__DeviceServiceCapabilities(soap);

	capabilities->Network = soap_new_tds__NetworkCapabilities(soap);

	capabilities->Network->IPFilter            = soap_new_ptr(soap,false);
	capabilities->Network->ZeroConfiguration   = soap_new_ptr(soap,false);
	capabilities->Network->IPVersion6          = soap_new_ptr(soap,false);
	capabilities->Network->DynDNS              = soap_new_ptr(soap,false);
	capabilities->Network->Dot11Configuration  = soap_new_ptr(soap,false);
	capabilities->Network->Dot1XConfigurations = soap_new_ptr(soap,0);
	capabilities->Network->HostnameFromDHCP    = soap_new_ptr(soap,false);
	capabilities->Network->NTP                 = soap_new_ptr(soap,0);
	capabilities->Network->DHCPv6              = soap_new_ptr(soap,false);


	capabilities->Security = soap_new_tds__SecurityCapabilities(soap);

	capabilities->Security->TLS1_x002e0          = soap_new_ptr(soap,false);
	capabilities->Security->TLS1_x002e1          = soap_new_ptr(soap,false);
	capabilities->Security->TLS1_x002e2          = soap_new_ptr(soap,false);
	capabilities->Security->OnboardKeyGeneration = soap_new_ptr(soap,false);
	capabilities->Security->AccessPolicyConfig   = soap_new_ptr(soap,false);
	capabilities->Security->DefaultAccessPolicy  = soap_new_ptr(soap,false);
	capabilities->Security->Dot1X                = soap_new_ptr(soap,false);
	capabilities->Security->RemoteUserHandling   = soap_new_ptr(soap,false);
	capabilities->Security->X_x002e509Token      = soap_new_ptr(soap,false);
	capabilities->Security->SAMLToken            = soap_new_ptr(soap,false);
	capabilities->Security->KerberosToken        = soap_new_ptr(soap,false);
	capabilities->Security->UsernameToken        = soap_new_ptr(soap,false);
	capabilities->Security->HttpDigest           = soap_new_ptr(soap,false);
	capabilities->Security->RELToken             = soap_new_ptr(soap,false);
	capabilities->Security->MaxUsers             = soap_new_ptr(soap,0);
	capabilities->Security->MaxUserNameLength    = soap_new_ptr(soap,0);
	capabilities->Security->MaxPasswordLength    = soap_new_ptr(soap,0);


	capabilities->System = soap_new_tds__SystemCapabilities(soap);

	capabilities->System->DiscoveryResolve       = soap_new_ptr(soap,true);
	capabilities->System->DiscoveryBye           = soap_new_ptr(soap,true);
	capabilities->System->RemoteDiscovery        = soap_new_ptr(soap,true);
	capabilities->System->SystemBackup           = soap_new_ptr(soap,false);
	capabilities->System->SystemLogging          = soap_new_ptr(soap,false);
	capabilities->System->FirmwareUpgrade        = soap_new_ptr(soap,false);
	capabilities->System->HttpFirmwareUpgrade    = soap_new_ptr(soap,false);
	capabilities->System->HttpSystemBackup       = soap_new_ptr(soap,false);
	capabilities->System->HttpSystemLogging      = soap_new_ptr(soap,false);
	capabilities->System->HttpSupportInformation = soap_new_ptr(soap,false);
	capabilities->System->StorageConfiguration   = soap_new_ptr(soap,false);

	return capabilities;
}

trt__Capabilities * getMediaServiceCapabilities(soap * soap)
{
	trt__Capabilities *capabilities = soap_new_trt__Capabilities(soap);

	capabilities->SnapshotUri 				= soap_new_ptr(soap, true);
	capabilities->Rotation 					= soap_new_ptr(soap, true);
	capabilities->VideoSourceMode 			= soap_new_ptr(soap, false);
	capabilities->OSD 						= soap_new_ptr(soap, true);
	capabilities->EXICompression 			= soap_new_ptr(soap, false);

	capabilities->ProfileCapabilities = soap_new_trt__ProfileCapabilities(soap);
	capabilities->ProfileCapabilities->MaximumNumberOfProfiles = soap_new_ptr(soap, 4);

	capabilities->StreamingCapabilities = soap_new_trt__StreamingCapabilities(soap);
	capabilities->StreamingCapabilities->RTPMulticast = soap_new_ptr(soap, false);
	capabilities->StreamingCapabilities->RTP_USCORETCP = soap_new_ptr(soap, false);
	capabilities->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = soap_new_ptr(soap, true);
	capabilities->StreamingCapabilities->NonAggregateControl = soap_new_ptr(soap, true);

	return capabilities;
}

timg__Capabilities * getImagingServiceCapabilities(soap * soap)
{
	timg__Capabilities * capabilities = soap_new_timg__Capabilities(soap);

	capabilities->Presets = soap_new_ptr(soap, true);
	capabilities->AdaptablePreset = soap_new_ptr(soap, true);

	return capabilities;
}

tptz__Capabilities * getPTZServiceCapabilities(soap * soap)
{
	tptz__Capabilities * capabilities = soap_new_tptz__Capabilities(soap);

	capabilities->EFlip = soap_new_ptr(soap, false);
	capabilities->Reverse = soap_new_ptr(soap, true);
	capabilities->GetCompatibleConfigurations = soap_new_ptr(soap, true);
	capabilities->MoveStatus = soap_new_ptr(soap, true);
	capabilities->StatusPosition = soap_new_ptr(soap, true);

	return capabilities;
}

// trc__Capabilities * getRecordingServiceCapabilities(soap * soap)
// {
// 	trc__Capabilities * capabilities = soap_new_trc__Capabilities(soap);
// 	return capabilities;
// }
//
// tse__Capabilities * getSearchServiceCapabilities(soap * soap)
// {
// 	tse__Capabilities * capabilities = soap_new_tse__Capabilities(soap);
// 	return capabilities;
// }

trv__Capabilities * getReceiverServiceCapabilities(soap * soap)
{
	trv__Capabilities * capabilities = soap_new_trv__Capabilities(soap);

	capabilities->RTP_USCOREMulticast = soap_new_ptr(soap, false);
	capabilities->RTP_USCORETCP = soap_new_ptr(soap, false);
	capabilities->RTP_USCORERTSP_USCORETCP = soap_new_ptr(soap, true);
	capabilities->SupportedReceivers = 100;
	capabilities->MaximumRTSPURILength = soap_new_ptr(soap, 254);

	return capabilities;
}

// trp__Capabilities * getReplayServiceCapabilities(soap * soap)
// {
// 	trp__Capabilities * capabilities = soap_new_trp__Capabilities(soap);
// 	return capabilities;
// }
//
// tev__Capabilities * getEventServiceCapabilities(soap * soap)
// {
// 	tev__Capabilities * capabilities = soap_new_tev__Capabilities(soap);
// 	return capabilities;
// }
//
// tls__Capabilities * getDisplayServiceCapabilities(soap * soap)
// {
// 	tls__Capabilities * capabilities = soap_new_tls__Capabilities(soap);
// 	return capabilities;
// }
//
tmd__Capabilities * getDeviceIOServiceCapabilities(soap * soap)
{
	tmd__Capabilities * capabilities = soap_new_tmd__Capabilities(soap);
	return capabilities;
}

std::vector<tt__Profile *> getProfiles(soap * soap)
{
	std::vector<tt__Profile *> profiles;

	tt__Profile * main_profile = soap_new_tt__Profile(soap);
	main_profile->token = MAIN_PROFILE;
	main_profile->fixed = soap_new_ptr(soap, true);
	main_profile->Name = MAIN_PROFILE;

	main_profile->VideoSourceConfiguration = getVideoSourceConfigurations(soap)[0];
	main_profile->AudioSourceConfiguration = getAudioSourceConfigurations(soap)[0];

	if( system_get_rtsp_running_format() == VideoFormat::H264 )
		main_profile->VideoEncoderConfiguration = getVideoEncoderConfigurations(soap)[0];
	else
		main_profile->VideoEncoderConfiguration = getVideoEncoderConfigurations(soap)[1];

	main_profile->AudioEncoderConfiguration = getAudioEncoderConfigurations(soap)[0];
	if( ServiceContext::GetInstance()->has_ptz() )
		main_profile->PTZConfiguration = getPTZConfigurations(soap)[0];
	main_profile->MetadataConfiguration = getMetadataConfigurations(soap)[0];

	main_profile->Extension = soap_new_tt__ProfileExtension(soap);
	main_profile->Extension->AudioOutputConfiguration = getAudioOutputConfigurations(soap)[0];

	profiles.push_back(main_profile);

	return profiles;
}

std::vector<tt__PTZConfiguration *> getPTZConfigurations(soap * soap)
{
	std::vector<tt__PTZConfiguration *> configs;

	tt__PTZConfiguration * config = soap_new_tt__PTZConfiguration(soap);
	config->token = PTZ_CFG;
	config->Name = PTZ_CFG;
	config->UseCount = 1;
	config->MoveRamp = soap_new_ptr(soap, 1);
	config->PresetRamp = soap_new_ptr(soap, 1);
	config->PresetTourRamp = soap_new_ptr(soap, 2);
	config->NodeToken = PTZ_NODE;

	config->DefaultAbsolutePantTiltPositionSpace = new string("AbsolutePanTiltPositionSpace");
	config->DefaultAbsoluteZoomPositionSpace = new string("AbsoluteZoomPositionSpace");
	config->DefaultRelativePanTiltTranslationSpace = new string("RelativePanTiltTranslationSpace");
	config->DefaultRelativeZoomTranslationSpace = new string("RelativeZoomTranslationSpace");
	config->DefaultContinuousPanTiltVelocitySpace = new string("ContinuousPanTiltVelocitySpace");
	config->DefaultContinuousZoomVelocitySpace = new string("ContinuousZoomVelocitySpace");

	config->DefaultPTZSpeed = soap_new_tt__PTZSpeed(soap);
	config->DefaultPTZSpeed->PanTilt = soap_new_req_tt__Vector2D(soap, 5.0, 5.0);
	config->DefaultPTZSpeed->Zoom = soap_new_req_tt__Vector1D(soap, 3.0);

	// config->DefaultPTZTimeout = soap_new_ptr(soap, 3000);

	config->PanTiltLimits = soap_new_tt__PanTiltLimits(soap);
	config->PanTiltLimits->Range = soap_new_tt__Space2DDescription(soap);
	config->PanTiltLimits->Range->URI = "PanTiltLimitsRange";
	config->PanTiltLimits->Range->XRange = soap_new_req_tt__FloatRange(soap, 0.0f, 2600.0f);
	config->PanTiltLimits->Range->YRange = soap_new_req_tt__FloatRange(soap, 0.0f, 600.0f);

	config->ZoomLimits = soap_new_tt__ZoomLimits(soap);
	config->ZoomLimits->Range = soap_new_tt__Space1DDescription(soap);
	config->ZoomLimits->Range->URI = "ZoomLimitsRange";
	config->ZoomLimits->Range->XRange = soap_new_req_tt__FloatRange(soap, 0.0f, 5.0f);

	configs.push_back(config);

	return configs;
}

tt__PTZSpaces * getPTZSpaces(soap * soap)
{
	std::string AbsolutePanTiltPositionSpace = "AbsolutePanTiltPositionSpace";
	std::string AbsoluteZoomPositionSpace = "AbsoluteZoomPositionSpace";
	std::string RelativePanTiltTranslationSpace = "RelativePanTiltTranslationSpace";
	std::string RelativeZoomTranslationSpace = "RelativeZoomTranslationSpace";
	std::string ContinuousPanTiltVelocitySpace = "ContinuousPanTiltVelocitySpace";
	std::string ContinuousZoomVelocitySpace = "ContinuousZoomVelocitySpace";
	std::string PanTiltSpeedSpace = "PanTiltSpeedSpace";
	std::string ZoomSpeedSpace = "ZoomSpeedSpace";

	tt__PTZSpaces * spaces = soap_new_tt__PTZSpaces(soap);
	spaces->AbsolutePanTiltPositionSpace.push_back(
			soap_new_req_tt__Space2DDescription(soap, AbsolutePanTiltPositionSpace,
				soap_new_req_tt__FloatRange(soap, -1.0, 1.0),
				soap_new_req_tt__FloatRange(soap, -1.0, 1.0)
			)
	);
	spaces->AbsoluteZoomPositionSpace.push_back(
			soap_new_req_tt__Space1DDescription(soap, AbsoluteZoomPositionSpace,
				soap_new_req_tt__FloatRange(soap, 0.0, 1.0)
			)
	);
	spaces->RelativePanTiltTranslationSpace.push_back(
			soap_new_req_tt__Space2DDescription(soap,RelativePanTiltTranslationSpace,
				soap_new_req_tt__FloatRange(soap, -1.0, 1.0),
				soap_new_req_tt__FloatRange(soap, -1.0, 1.0)
			)
	);
	spaces->RelativeZoomTranslationSpace.push_back(
			soap_new_req_tt__Space1DDescription(soap, RelativeZoomTranslationSpace,
				soap_new_req_tt__FloatRange(soap, -1.0, 1.0)
			)
	);
	spaces->ContinuousPanTiltVelocitySpace.push_back(
			soap_new_req_tt__Space2DDescription(soap, ContinuousPanTiltVelocitySpace,
				soap_new_req_tt__FloatRange(soap, -1.0, 1.0),
				soap_new_req_tt__FloatRange(soap, -1.0, 1.0)
			)
	);
	spaces->ContinuousZoomVelocitySpace.push_back(
			soap_new_req_tt__Space1DDescription(soap, ContinuousZoomVelocitySpace,
				soap_new_req_tt__FloatRange(soap, -1.0, 1.0)
			)
	);
	spaces->PanTiltSpeedSpace.push_back(
			soap_new_req_tt__Space1DDescription(soap, PanTiltSpeedSpace,
				soap_new_req_tt__FloatRange(soap, 10.0, 900.0)
			)
	);
	spaces->ZoomSpeedSpace.push_back(
			soap_new_req_tt__Space1DDescription(soap, ZoomSpeedSpace,
				soap_new_req_tt__FloatRange(soap, 1.0, 5.0)
			)
	);

	return spaces;
}

std::vector<tt__PTZNode *> getPTZNodes(soap * soap)
{
	std::vector<tt__PTZNode *> nodes;

	tt__PTZNode * node = soap_new_tt__PTZNode(soap);
	node->token = PTZ_NODE;
	node->Name = new string(PTZ_NODE);
	node->FixedHomePosition = soap_new_ptr(soap, false);
	node->GeoMove = soap_new_ptr(soap, false);
	node->SupportedPTZSpaces = getPTZSpaces(soap);
	node->MaximumNumberOfPresets = 10;
	node->HomeSupported = true;

	nodes.push_back(node);

	return nodes;
}

/*---------------------------------------------------------
 * MEDIA CONFIGURATIONS
 *--------------------------------------------------------*/
std::vector<tt__VideoSource *> getVideoSources(soap * soap)
{
	std::vector<tt__VideoSource *> video_sources;

	VideoResolution res = system_get_rtsp_video_resolution();
	tt__VideoResolution * resolution = soap_new_req_tt__VideoResolution(soap, res.width, res.height);

	tt__VideoSource * videosrc = soap_new_tt__VideoSource(soap);
	videosrc->token = VIDEO_SRC;
	videosrc->Framerate = system_get_rtsp_framerate();
	videosrc->Resolution = resolution;
	videosrc->Imaging = soap_new_tt__ImagingSettings(soap);

	video_sources.push_back(videosrc);

	return video_sources;
}

std::vector<tt__VideoSourceConfiguration *> getVideoSourceConfigurations(soap * soap)
{
	std::vector<tt__VideoSourceConfiguration *> configs;

	VideoResolution res = system_get_rtsp_video_resolution();

	tt__VideoSourceConfiguration * vsc = soap_new_tt__VideoSourceConfiguration(soap);
	vsc->token = VIDEO_CFG;
	vsc->Name =  VIDEO_CFG;
	vsc->SourceToken = VIDEO_SRC;
	vsc->UseCount = 1;
	vsc->Bounds = soap_new_req_tt__IntRectangle(soap, 0, 0, res.width, res.height);
	configs.push_back(vsc);

	return configs;
}

std::vector<tt__VideoEncoderConfiguration *> getVideoEncoderConfigurations(soap * soap)
{
	std::vector<tt__VideoEncoderConfiguration *> configs;

	VideoResolution res = system_get_rtsp_video_resolution();
	tt__VideoResolution * resolution = soap_new_req_tt__VideoResolution(soap, res.width, res.height);
	tt__VideoRateControl * framerate = soap_new_req_tt__VideoRateControl(soap, system_get_rtsp_framerate(), 1, system_get_rtsp_video_bitrate());

	tt__VideoEncoderConfiguration * h264 = soap_new_tt__VideoEncoderConfiguration(soap);
	h264->token = VIDEO_ENC_H264_CFG;
	h264->Name =  VIDEO_ENC_H264_CFG;
	h264->Encoding = tt__VideoEncoding__H264;
	h264->UseCount = 1;
	h264->Resolution = resolution;
	h264->Quality = 0.0f;
	h264->RateControl = framerate;
	h264->H264 = soap_new_tt__H264Configuration(soap);
	h264->H264->GovLength = 150;
	h264->H264->H264Profile = tt__H264Profile__Baseline;
	h264->Multicast = getMulticastConfiguration(soap);
	h264->SessionTimeout = 30000;
	configs.push_back(h264);

	tt__VideoEncoderConfiguration * jpeg = soap_new_tt__VideoEncoderConfiguration(soap);
	jpeg->token = VIDEO_ENC_JPEG_CFG;
	jpeg->Name =  VIDEO_ENC_JPEG_CFG;
	jpeg->Encoding = tt__VideoEncoding__JPEG;
	jpeg->UseCount = 1;
	jpeg->Resolution = resolution;
	jpeg->Quality = 0.0f;
	jpeg->RateControl = framerate;
	h264->Multicast = getMulticastConfiguration(soap);
	h264->SessionTimeout = 30000;
	configs.push_back(jpeg);

	return configs;
}

std::vector<tt__AudioSourceConfiguration *> getAudioSourceConfigurations(soap * soap)
{
	std::vector<tt__AudioSourceConfiguration *> configs;

	tt__AudioSourceConfiguration * asc = soap_new_tt__AudioSourceConfiguration(soap);
	asc->token = AUDIO_CFG;
	asc->Name =  AUDIO_CFG;
	asc->SourceToken = AUDIO_SRC;
	asc->UseCount = 1;
	configs.push_back(asc);

	return configs;
}

std::vector<tt__AudioOutputConfiguration *> getAudioOutputConfigurations(soap * soap)
{
	std::vector<tt__AudioOutputConfiguration *> configs;

	tt__AudioOutputConfiguration * adc = soap_new_tt__AudioOutputConfiguration(soap);
	adc->token = AUDIO_OUT_CFG;
	adc->Name = AUDIO_OUT_CFG;
	adc->UseCount = 1;
	adc->OutputToken = AUDIO_OUTPUT;
	adc->OutputLevel = system_get_rtsp_audio_hw_volume();
	configs.push_back(adc);

	return configs;
}

std::vector<tt__AudioEncoderConfiguration *> getAudioEncoderConfigurations(soap * soap)
{
	std::vector<tt__AudioEncoderConfiguration *> configs;

	tt__AudioEncoderConfiguration * aec = soap_new_tt__AudioEncoderConfiguration(soap);

	aec->token = AUDIO_ENC_G711_CFG;
	aec->Name =  AUDIO_ENC_G711_CFG;
	aec->Encoding = tt__AudioEncoding__G711;
	aec->Bitrate = 1;
	aec->SampleRate = system_get_rtsp_audio_in_samplerate();
	aec->Multicast = getMulticastConfiguration(soap);
	aec->SessionTimeout = 30000;
	configs.push_back(aec);

	return configs;
}

std::vector<tt__MetadataConfiguration *> getMetadataConfigurations(soap * soap)
{
	std::vector<tt__MetadataConfiguration *> configs;

	tt__MetadataConfiguration * meta = soap_new_tt__MetadataConfiguration(soap);

	meta->token = META_DATA;
	meta->Name  = META_DATA;

	if( ServiceContext::GetInstance()->has_ptz() )
		meta->PTZStatus = soap_new_req_tt__PTZFilter(soap, true, true);

	configs.push_back(meta);

	return configs;
}

tt__MulticastConfiguration * getMulticastConfiguration( soap * soap )
{
	tt__MulticastConfiguration * multicast = soap_new_tt__MulticastConfiguration(soap);

	AddressPort addr = system_get_rtsp_multicast_dest();
	if( ! addr.port )
		return multicast;

	tt__IPAddress * tt_addr = soap_new_tt__IPAddress(soap);

	tt_addr->Type = tt__IPType__IPv4;
	tt_addr->IPv4Address = new string(addr.ip);

	multicast->Address = tt_addr;
	multicast->Port = addr.port;
	multicast->TTL = 30;
	multicast->AutoStart = false;

	return multicast;
}

tt__IPAddress * getIpAddress( soap * soap )
{
	tt__IPAddress * ip = soap_new_tt__IPAddress(soap);
	static std::string _address = "0.0.0.0:8554";

	ip->Type = tt__IPType__IPv4;
	ip->IPv4Address = &_address;

	return ip;
}

std::vector<tt__VideoResolution *> getVideoResolutions( soap * soap )
{
	std::vector<tt__VideoResolution *> resolutions;

	resolutions.push_back( soap_new_req_tt__VideoResolution(soap, 1920, 1080) );
	resolutions.push_back( soap_new_req_tt__VideoResolution(soap, 1600, 900) );
	resolutions.push_back( soap_new_req_tt__VideoResolution(soap, 1280, 720) );
	resolutions.push_back( soap_new_req_tt__VideoResolution(soap, 960, 540) );
	resolutions.push_back( soap_new_req_tt__VideoResolution(soap, 768, 432) );
	resolutions.push_back( soap_new_req_tt__VideoResolution(soap, 640, 360) );

	return resolutions;
}
