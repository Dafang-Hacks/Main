/**
 * @Author: Sian Croser
 * @Date:   2020-03-03T11:37:51+10:30
 * @Email:  CQoute@gmail.com
 * @Filename: ServiceMedia.cpp
 * @Last modified by:   Sian Croser
 * @Last modified time: 2020-03-05T16:07:25+10:30
 * @License: GPL-3
 */


/*
 --------------------------------------------------------------------------
 ServiceMedia.cpp

 Implementation of functions (methods) for the service:
 ONVIF media.wsdl server side
-----------------------------------------------------------------------------
*/
#include <string>
#include <stdio.h>
#include <boost/lexical_cast.hpp>

#include "../../generated/soapMediaBindingService.h"
#include "ServiceContext.h"
#include "soap_helpers.h"
#include "smacros.h"
#include "../system_utils.h"

int MediaBindingService::GetServiceCapabilities(_trt__GetServiceCapabilities *trt__GetServiceCapabilities, _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetServiceCapabilitiesResponse->Capabilities = getMediaServiceCapabilities(this->soap);

	return SOAP_OK;
}

int MediaBindingService::GetVideoSources(_trt__GetVideoSources *trt__GetVideoSources, _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetVideoSourcesResponse->VideoSources = getVideoSources(this->soap);

	return SOAP_OK;
}

int MediaBindingService::GetAudioSources(_trt__GetAudioSources *trt__GetAudioSources, _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetAudioOutputs(_trt__GetAudioOutputs *trt__GetAudioOutputs, _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::CreateProfile(_trt__CreateProfile *trt__CreateProfile, _trt__CreateProfileResponse *trt__CreateProfileResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__CreateProfileResponse->Profile = getProfiles(this->soap)[0];

	return SOAP_OK;
}

int MediaBindingService::GetProfile(_trt__GetProfile *trt__GetProfile, _trt__GetProfileResponse *trt__GetProfileResponse)
{
	DEBUG_MSG("Media: %s   get profile:%s\n", __FUNCTION__, trt__GetProfile->ProfileToken.c_str());

	trt__GetProfileResponse->Profile = getProfiles(this->soap)[0];
	if( trt__GetProfileResponse->Profile )
		return SOAP_OK;

	return SOAP_FAULT;
}

int MediaBindingService::GetProfiles(_trt__GetProfiles *trt__GetProfiles, _trt__GetProfilesResponse *trt__GetProfilesResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetProfilesResponse->Profiles = getProfiles(this->soap);

	return SOAP_OK;
}

int MediaBindingService::AddVideoEncoderConfiguration(_trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration, _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::AddVideoSourceConfiguration(_trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration, _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::AddAudioEncoderConfiguration(_trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration, _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::AddAudioSourceConfiguration(_trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration, _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::AddPTZConfiguration(_trt__AddPTZConfiguration *trt__AddPTZConfiguration, _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::AddVideoAnalyticsConfiguration(_trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration, _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::AddMetadataConfiguration(_trt__AddMetadataConfiguration *trt__AddMetadataConfiguration, _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::AddAudioOutputConfiguration(_trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration, _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::AddAudioDecoderConfiguration(_trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration, _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::RemoveVideoEncoderConfiguration(_trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration, _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::RemoveVideoSourceConfiguration(_trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration, _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::RemoveAudioEncoderConfiguration(_trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration, _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::RemoveAudioSourceConfiguration(_trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration, _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::RemovePTZConfiguration(_trt__RemovePTZConfiguration *trt__RemovePTZConfiguration, _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::RemoveVideoAnalyticsConfiguration(_trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration, _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::RemoveMetadataConfiguration(_trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration, _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::RemoveAudioOutputConfiguration(_trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration, _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::RemoveAudioDecoderConfiguration(_trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration, _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::DeleteProfile(_trt__DeleteProfile *trt__DeleteProfile, _trt__DeleteProfileResponse *trt__DeleteProfileResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	// ServiceContext::GetInstance()->delete_profile( trt__DeleteProfile->ProfileToken );

	return SOAP_OK;
}

int MediaBindingService::GetVideoSourceConfigurations(_trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations, _trt__GetVideoSourceConfigurationsResponse *trt__GetVideoSourceConfigurationsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetVideoSourceConfigurationsResponse->Configurations = getVideoSourceConfigurations(this->soap);

	return SOAP_OK;
}

int MediaBindingService::GetVideoEncoderConfigurations(_trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations, _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetVideoEncoderConfigurationsResponse->Configurations = getVideoEncoderConfigurations(this->soap);

	return SOAP_OK;
}

int MediaBindingService::GetAudioSourceConfigurations(_trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations, _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetAudioSourceConfigurationsResponse->Configurations = getAudioSourceConfigurations(this->soap);

	return SOAP_OK;
}

int MediaBindingService::GetAudioEncoderConfigurations(_trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations, _trt__GetAudioEncoderConfigurationsResponse *trt__GetAudioEncoderConfigurationsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetAudioEncoderConfigurationsResponse->Configurations = getAudioEncoderConfigurations(this->soap);

	return SOAP_OK;
}

int MediaBindingService::GetVideoAnalyticsConfigurations(_trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations, _trt__GetVideoAnalyticsConfigurationsResponse *trt__GetVideoAnalyticsConfigurationsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetMetadataConfigurations(_trt__GetMetadataConfigurations *trt__GetMetadataConfigurations, _trt__GetMetadataConfigurationsResponse *trt__GetMetadataConfigurationsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetMetadataConfigurationsResponse->Configurations = getMetadataConfigurations(this->soap);

	return SOAP_OK;
}

int MediaBindingService::GetAudioOutputConfigurations(_trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations, _trt__GetAudioOutputConfigurationsResponse *trt__GetAudioOutputConfigurationsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetAudioOutputConfigurationsResponse->Configurations = getAudioOutputConfigurations(this->soap);

	return SOAP_OK;
}

int MediaBindingService::GetAudioDecoderConfigurations(_trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations, _trt__GetAudioDecoderConfigurationsResponse *trt__GetAudioDecoderConfigurationsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetVideoSourceConfiguration(_trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration, _trt__GetVideoSourceConfigurationResponse *trt__GetVideoSourceConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetVideoSourceConfigurationResponse->Configuration = getVideoSourceConfigurations(this->soap)[0];

	return SOAP_OK;
}

int MediaBindingService::GetVideoEncoderConfiguration(_trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration, _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetVideoEncoderConfigurationResponse->Configuration = getVideoEncoderConfigurations(this->soap)[0];

	return SOAP_OK;
}

int MediaBindingService::GetAudioSourceConfiguration(_trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration, _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetAudioSourceConfigurationResponse->Configuration = getAudioSourceConfigurations(this->soap)[0];

	return SOAP_OK;
}

int MediaBindingService::GetAudioEncoderConfiguration(_trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration, _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetAudioEncoderConfigurationResponse->Configuration = getAudioEncoderConfigurations(this->soap)[0];

	return SOAP_OK;
}

int MediaBindingService::GetVideoAnalyticsConfiguration(_trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration, _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetMetadataConfiguration(_trt__GetMetadataConfiguration *trt__GetMetadataConfiguration, _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetMetadataConfigurationResponse->Configuration = getMetadataConfigurations(this->soap)[0];

	return SOAP_OK;
}

int MediaBindingService::GetAudioOutputConfiguration(_trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration, _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetAudioOutputConfigurationResponse->Configuration = getAudioOutputConfigurations(this->soap)[0];

	return SOAP_OK;
}

int MediaBindingService::GetAudioDecoderConfiguration(_trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration, _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetCompatibleVideoEncoderConfigurations(_trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations, _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetCompatibleVideoSourceConfigurations(_trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations, _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetCompatibleAudioEncoderConfigurations(_trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations, _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetCompatibleAudioSourceConfigurations(_trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations, _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetCompatibleVideoAnalyticsConfigurations(_trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations, _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetCompatibleMetadataConfigurations(_trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations, _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetCompatibleAudioOutputConfigurations(_trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations, _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetCompatibleAudioDecoderConfigurations(_trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations, _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::SetVideoSourceConfiguration(_trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration, _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::SetVideoEncoderConfiguration(_trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	if( trt__SetVideoEncoderConfiguration->Configuration->Resolution )
	{
		tt__VideoResolution * resolution = trt__SetVideoEncoderConfiguration->Configuration->Resolution;
		char buffer[56];
		sprintf( buffer, "-W%i -H%i", resolution->Width, resolution->Height );
		system_write_config("rtspserver", "RTSPH264OPTS", buffer);
	}

	if( trt__SetVideoEncoderConfiguration->Configuration->RateControl )
	{
		// system_write_config("rtspserver", "BITRATE", boost::lexical_cast<std::string>(
		// 	trt__SetVideoEncoderConfiguration->Configuration->RateControl->BitrateLimit
		// ));
		system_write_config("rtspserver", "FRAMERATE_NUM", boost::lexical_cast<std::string>(
			trt__SetVideoEncoderConfiguration->Configuration->RateControl->FrameRateLimit
		));
	}

	if( trt__SetVideoEncoderConfiguration->Configuration->Quality )
	{
		system_write_config("rtspserver", "BITRATE", boost::lexical_cast<std::string>(
			trt__SetVideoEncoderConfiguration->Configuration->Quality * 50
		));
	}

	system_restart_rtsp(this->soap);

	return SOAP_OK;
}

int MediaBindingService::SetAudioSourceConfiguration(_trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration, _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::SetAudioEncoderConfiguration(_trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration, _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::SetVideoAnalyticsConfiguration(_trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration, _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::SetMetadataConfiguration(_trt__SetMetadataConfiguration *trt__SetMetadataConfiguration, _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::SetAudioOutputConfiguration(_trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration, _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::SetAudioDecoderConfiguration(_trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration, _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetVideoSourceConfigurationOptions(_trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions, _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetVideoSourceConfigurationOptionsResponse->Options = soap_new_tt__VideoSourceConfigurationOptions(this->soap);

	return SOAP_OK;
}

int MediaBindingService::GetVideoEncoderConfigurationOptions(_trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetVideoEncoderConfigurationOptionsResponse->Options = soap_new_tt__VideoEncoderConfigurationOptions(this->soap);

	trt__GetVideoEncoderConfigurationOptionsResponse->Options->QualityRange = soap_new_req_tt__IntRange(this->soap, 1, 100);

	trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264 = soap_new_tt__H264Options(this->soap);
	trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264->ResolutionsAvailable = getVideoResolutions( this->soap );
	trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264->FrameRateRange = soap_new_req_tt__IntRange(this->soap, 5, 30);
	trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264->EncodingIntervalRange = soap_new_req_tt__IntRange(this->soap, 1, 10);
	trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264->H264ProfilesSupported.push_back(tt__H264Profile__Baseline);

	trt__GetVideoEncoderConfigurationOptionsResponse->Options->JPEG = soap_new_tt__JpegOptions(this->soap);
	trt__GetVideoEncoderConfigurationOptionsResponse->Options->JPEG->ResolutionsAvailable = getVideoResolutions( this->soap );
	trt__GetVideoEncoderConfigurationOptionsResponse->Options->JPEG->FrameRateRange = soap_new_req_tt__IntRange(this->soap, 5, 30);
	trt__GetVideoEncoderConfigurationOptionsResponse->Options->JPEG->EncodingIntervalRange = soap_new_req_tt__IntRange(this->soap, 1, 1);

	return SOAP_OK;
}

int MediaBindingService::GetAudioSourceConfigurationOptions(_trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions, _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetAudioEncoderConfigurationOptions(_trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions, _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	tt__AudioEncoderConfigurationOption * option = soap_new_tt__AudioEncoderConfigurationOption(this->soap);

	option->BitrateList = soap_new_tt__IntItems(this->soap);
	option->BitrateList->Items = { 320 };
	option->SampleRateList = soap_new_tt__IntItems(this->soap);
	option->SampleRateList->Items = { 8000, 16000, 24000, 44100, 48000 };

	trt__GetAudioEncoderConfigurationOptionsResponse->Options = soap_new_tt__AudioEncoderConfigurationOptions(this->soap);
	trt__GetAudioEncoderConfigurationOptionsResponse->Options->Options.push_back(option);

	return SOAP_OK;
}

int MediaBindingService::GetMetadataConfigurationOptions(_trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions, _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetAudioOutputConfigurationOptions(_trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions, _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetAudioOutputConfigurationOptionsResponse->Options = soap_new_tt__AudioOutputConfigurationOptions(this->soap);
	trt__GetAudioOutputConfigurationOptionsResponse->Options->OutputLevelRange = soap_new_req_tt__IntRange(this->soap, 1, 10);

	return SOAP_OK;
}

int MediaBindingService::GetAudioDecoderConfigurationOptions(_trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions, _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetGuaranteedNumberOfVideoEncoderInstances(_trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances, _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse->TotalNumber = 2048;
	trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse->H264 = soap_new_ptr(this->soap, 1024);
	trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse->JPEG = soap_new_ptr(this->soap, 1024);

	return SOAP_OK;
}

int MediaBindingService::GetStreamUri(_trt__GetStreamUri *trt__GetStreamUri, _trt__GetStreamUriResponse *trt__GetStreamUriResponse)
{
	DEBUG_MSG("Media: %s   for profile:%s\n", __FUNCTION__, trt__GetStreamUri->ProfileToken.c_str());

	std::string ip = ServiceContext::GetInstance()->getServerIpFromClient();
	std::string uri = "rtsp://" + ip + ":" + system_read_config("rtspserver", "PORT") + "/unicast";

	trt__GetStreamUriResponse->MediaUri = soap_new_req_tt__MediaUri(this->soap, uri, true, true, 0);

	DEBUG_MSG( "Media: Stream Uri: %s\n", uri.c_str() );
	return SOAP_OK;
}

int MediaBindingService::StartMulticastStreaming(_trt__StartMulticastStreaming *trt__StartMulticastStreaming, _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::StopMulticastStreaming(_trt__StopMulticastStreaming *trt__StopMulticastStreaming, _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::SetSynchronizationPoint(_trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetSnapshotUri(_trt__GetSnapshotUri *trt__GetSnapshotUri, _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);

	char buffer[256];
	sprintf(buffer, "http://%s:%i/cgi-bin/currentpic.cgi", ServiceContext::GetInstance()->getServerIpFromClient().c_str(), system_get_http_port());
	std::string uri = string(buffer);
	trt__GetSnapshotUriResponse->MediaUri = soap_new_req_tt__MediaUri(this->soap, uri, false, false, 0);

	DEBUG_MSG( "Media: Snapshot Uri: %s\n", buffer );

	return SOAP_OK;
}

int MediaBindingService::GetVideoSourceModes(_trt__GetVideoSourceModes *trt__GetVideoSourceModes, _trt__GetVideoSourceModesResponse *trt__GetVideoSourceModesResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::SetVideoSourceMode(_trt__SetVideoSourceMode *trt__SetVideoSourceMode, _trt__SetVideoSourceModeResponse *trt__SetVideoSourceModeResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetOSDs(_trt__GetOSDs *trt__GetOSDs, _trt__GetOSDsResponse *trt__GetOSDsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetOSD(_trt__GetOSD *trt__GetOSD, _trt__GetOSDResponse *trt__GetOSDResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::GetOSDOptions(_trt__GetOSDOptions *trt__GetOSDOptions, _trt__GetOSDOptionsResponse *trt__GetOSDOptionsResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::SetOSD(_trt__SetOSD *trt__SetOSD, _trt__SetOSDResponse *trt__SetOSDResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::CreateOSD(_trt__CreateOSD *trt__CreateOSD, _trt__CreateOSDResponse *trt__CreateOSDResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}

int MediaBindingService::DeleteOSD(_trt__DeleteOSD *trt__DeleteOSD, _trt__DeleteOSDResponse *trt__DeleteOSDResponse)
{
	DEBUG_MSG("Media: %s\n", __FUNCTION__);
	return SOAP_OK;
}
