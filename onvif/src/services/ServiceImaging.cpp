/*
 --------------------------------------------------------------------------
 ServiceReceiver.cpp

 Implementation of functions (methods) for the service:
 ONVIF receiver.wsdl server side
-----------------------------------------------------------------------------
*/

#include "../../generated/soapImagingBindingService.h"
#include "ServiceContext.h"
#include "soap_helpers.h"
#include "smacros.h"

/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
int ImagingBindingService::GetServiceCapabilities(_timg__GetServiceCapabilities *timg__GetServiceCapabilities, _timg__GetServiceCapabilitiesResponse *timg__GetServiceCapabilitiesResponse)
{
	DEBUG_MSG("Imaging: %s\n", __FUNCTION__);

	timg__GetServiceCapabilitiesResponse->Capabilities = getImagingServiceCapabilities(this->soap);

	return SOAP_OK;
}

/// Web service operation 'GetImagingSettings' (returns error code or SOAP_OK)
int ImagingBindingService::GetImagingSettings(_timg__GetImagingSettings *timg__GetImagingSettings, _timg__GetImagingSettingsResponse *timg__GetImagingSettingsResponse)
{
	DEBUG_MSG("Imaging: %s\n", __FUNCTION__);

	timg__GetImagingSettingsResponse->ImagingSettings = soap_new_tt__ImagingSettings20(this->soap);
	timg__GetImagingSettingsResponse->ImagingSettings->Brightness = soap_new_ptr(this->soap, 0.5f);
	timg__GetImagingSettingsResponse->ImagingSettings->Contrast = soap_new_ptr(this->soap, 0.5f);
	if( system_auto_night_mode() )
		timg__GetImagingSettingsResponse->ImagingSettings->IrCutFilter = soap_new_ptr(this->soap, tt__IrCutFilterMode__AUTO);
	else
		timg__GetImagingSettingsResponse->ImagingSettings->IrCutFilter = soap_new_ptr(this->soap, ( system_night_mode() ) ? tt__IrCutFilterMode__ON : tt__IrCutFilterMode__OFF );

	return SOAP_OK;
}

/// Web service operation 'SetImagingSettings' (returns error code or SOAP_OK)
int ImagingBindingService::SetImagingSettings(_timg__SetImagingSettings *timg__SetImagingSettings, _timg__SetImagingSettingsResponse *timg__SetImagingSettingsResponse)
{
	DEBUG_MSG("Imaging: %s\n", __FUNCTION__);

	if( timg__SetImagingSettings->ImagingSettings->IrCutFilter )
	{
		tt__IrCutFilterMode * ir_value = timg__SetImagingSettings->ImagingSettings->IrCutFilter;
		if( *ir_value == tt__IrCutFilterMode__ON || *ir_value == tt__IrCutFilterMode__OFF )
		{
			system_auto_night_mode(false);
			system_night_mode(*ir_value == tt__IrCutFilterMode__ON);
			// system_ircut(*ir_value == tt__IrCutFilterMode__ON);
		} else {
			system_auto_night_mode(true);
		}
	}

	return SOAP_OK;
}

/// Web service operation 'GetOptions' (returns error code or SOAP_OK)
int ImagingBindingService::GetOptions(_timg__GetOptions *timg__GetOptions, _timg__GetOptionsResponse *timg__GetOptionsResponse)
{
	DEBUG_MSG("Imaging: %s\n", __FUNCTION__);

	timg__GetOptionsResponse->ImagingOptions = soap_new_tt__ImagingOptions20(this->soap);
	timg__GetOptionsResponse->ImagingOptions->Brightness = soap_new_req_tt__FloatRange(this->soap, 0.0f, 1.0f);
	timg__GetOptionsResponse->ImagingOptions->Contrast = soap_new_req_tt__FloatRange(this->soap, 0.0f, 1.0f);
	timg__GetOptionsResponse->ImagingOptions->IrCutFilterModes = { tt__IrCutFilterMode__ON, tt__IrCutFilterMode__OFF, tt__IrCutFilterMode__AUTO };

	return SOAP_OK;
}

/// Web service operation 'Move' (returns error code or SOAP_OK)
int ImagingBindingService::Move(_timg__Move *timg__Move, _timg__MoveResponse *timg__MoveResponse)
{
	DEBUG_MSG("Imaging: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service operation 'Stop' (returns error code or SOAP_OK)
int ImagingBindingService::Stop(_timg__Stop *timg__Stop, _timg__StopResponse *timg__StopResponse)
{
	DEBUG_MSG("Imaging: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service operation 'GetStatus' (returns error code or SOAP_OK)
int ImagingBindingService::GetStatus(_timg__GetStatus *timg__GetStatus, _timg__GetStatusResponse *timg__GetStatusResponse)
{
	DEBUG_MSG("Imaging: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service operation 'GetMoveOptions' (returns error code or SOAP_OK)
int ImagingBindingService::GetMoveOptions(_timg__GetMoveOptions *timg__GetMoveOptions, _timg__GetMoveOptionsResponse *timg__GetMoveOptionsResponse)
{
	DEBUG_MSG("Imaging: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service operation 'GetPresets' (returns error code or SOAP_OK)
int ImagingBindingService::GetPresets(_timg__GetPresets *timg__GetPresets, _timg__GetPresetsResponse *timg__GetPresetsResponse)
{
	DEBUG_MSG("Imaging: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service operation 'GetCurrentPreset' (returns error code or SOAP_OK)
int ImagingBindingService::GetCurrentPreset(_timg__GetCurrentPreset *timg__GetCurrentPreset, _timg__GetCurrentPresetResponse *timg__GetCurrentPresetResponse)
{
	DEBUG_MSG("Imaging: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service operation 'SetCurrentPreset' (returns error code or SOAP_OK)
int ImagingBindingService::SetCurrentPreset(_timg__SetCurrentPreset *timg__SetCurrentPreset, _timg__SetCurrentPresetResponse *timg__SetCurrentPresetResponse)
{
	DEBUG_MSG("Imaging: %s\n", __FUNCTION__);
	return SOAP_OK;
}
