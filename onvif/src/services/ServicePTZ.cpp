/*
 --------------------------------------------------------------------------
 ServiceReceiver.cpp

 Implementation of functions (methods) for the service:
 ONVIF receiver.wsdl server side
-----------------------------------------------------------------------------
*/

#include "../../generated/soapPTZBindingService.h"
#include "ServiceContext.h"
#include "soap_helpers.h"
#include "smacros.h"

/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
int PTZBindingService::GetServiceCapabilities(_tptz__GetServiceCapabilities *tptz__GetServiceCapabilities, _tptz__GetServiceCapabilitiesResponse *tptz__GetServiceCapabilitiesResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	tptz__GetServiceCapabilitiesResponse->Capabilities = getPTZServiceCapabilities(this->soap);

	return SOAP_OK;
}

/// Web service operation 'GetConfigurations' (returns error code or SOAP_OK)
int PTZBindingService::GetConfigurations(_tptz__GetConfigurations *tptz__GetConfigurations, _tptz__GetConfigurationsResponse *tptz__GetConfigurationsResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	tptz__GetConfigurationsResponse->PTZConfiguration = getPTZConfigurations(this->soap);

	return SOAP_OK;
}

/// Web service operation 'GetPresets' (returns error code or SOAP_OK)
int PTZBindingService::GetPresets(_tptz__GetPresets *tptz__GetPresets, _tptz__GetPresetsResponse *tptz__GetPresetsResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	std::map<std::string, PTPosition *> presets = ServiceContext::GetInstance()->getPTZController()->presets();

	for( std::map<std::string, PTPosition *>::iterator it = presets.begin(); it != presets.end(); ++it )
	{
		if( it->first == "home" )
			continue;

		tt__PTZPreset * preset = soap_new_tt__PTZPreset(this->soap);
		preset->token = new string( it->first );
		preset->Name = new string( it->first );
		preset->PTZPosition = soap_new_tt__PTZVector(this->soap);
		preset->PTZPosition->PanTilt = soap_new_req_tt__Vector2D(this->soap, it->second->x, it->second->y);
		preset->PTZPosition->Zoom = soap_new_req_tt__Vector1D(this->soap, 0);
		tptz__GetPresetsResponse->Preset.push_back(preset);
	}

	return SOAP_OK;
}

/// Web service operation 'SetPreset' (returns error code or SOAP_OK)
int PTZBindingService::SetPreset(_tptz__SetPreset *tptz__SetPreset, _tptz__SetPresetResponse *tptz__SetPresetResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	ServiceContext::GetInstance()->getPTZController()->set_preset( * tptz__SetPreset->PresetName );

	return SOAP_OK;
}

/// Web service operation 'RemovePreset' (returns error code or SOAP_OK)
int PTZBindingService::RemovePreset(_tptz__RemovePreset *tptz__RemovePreset, _tptz__RemovePresetResponse *tptz__RemovePresetResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	ServiceContext::GetInstance()->getPTZController()->remove_preset( tptz__RemovePreset->PresetToken );

	return SOAP_OK;
}

/// Web service operation 'GotoPreset' (returns error code or SOAP_OK)
int PTZBindingService::GotoPreset(_tptz__GotoPreset *tptz__GotoPreset, _tptz__GotoPresetResponse *tptz__GotoPresetResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	ServiceContext::GetInstance()->getPTZController()->go_preset_position( tptz__GotoPreset->PresetToken );

	return SOAP_OK;
}

/// Web service operation 'GetStatus' (returns error code or SOAP_OK)
int PTZBindingService::GetStatus(_tptz__GetStatus *tptz__GetStatus, _tptz__GetStatusResponse *tptz__GetStatusResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	PTMotorDriver::MotorState state = ServiceContext::GetInstance()->getPTZController()->get_motor_state(); // system_get_ptz_state();
	tt__PTZStatus * status = soap_new_tt__PTZStatus(this->soap);

	status->Position = soap_new_tt__PTZVector(this->soap);
	status->Position->PanTilt = soap_new_req_tt__Vector2D(this->soap, (float)state.x, (float)state.y);

	status->MoveStatus = soap_new_tt__PTZMoveStatus(this->soap);
	status->MoveStatus->PanTilt = soap_new_ptr( this->soap, ( state.status == PTMotorDriver::motor_status::MOTOR_IS_STOP ) ? tt__MoveStatus__IDLE : tt__MoveStatus__MOVING );

	tptz__GetStatusResponse->PTZStatus = status;

	return SOAP_OK;
}

/// Web service operation 'GetConfiguration' (returns error code or SOAP_OK)
int PTZBindingService::GetConfiguration(_tptz__GetConfiguration *tptz__GetConfiguration, _tptz__GetConfigurationResponse *tptz__GetConfigurationResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	tptz__GetConfigurationResponse->PTZConfiguration = getPTZConfigurations(this->soap)[0];

	return SOAP_OK;
}

/// Web service operation 'GetNodes' (returns error code or SOAP_OK)
int PTZBindingService::GetNodes(_tptz__GetNodes *tptz__GetNodes, _tptz__GetNodesResponse *tptz__GetNodesResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	tptz__GetNodesResponse->PTZNode = getPTZNodes(this->soap);

	return SOAP_OK;
}

/// Web service operation 'GetNode' (returns error code or SOAP_OK)
int PTZBindingService::GetNode(_tptz__GetNode *tptz__GetNode, _tptz__GetNodeResponse *tptz__GetNodeResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	tptz__GetNodeResponse->PTZNode = getPTZNodes(this->soap)[0];

	return SOAP_OK;
}

/// Web service operation 'SetConfiguration' (returns error code or SOAP_OK)
int PTZBindingService::SetConfiguration(_tptz__SetConfiguration *tptz__SetConfiguration, _tptz__SetConfigurationResponse *tptz__SetConfigurationResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	return SOAP_OK;
}

/// Web service operation 'GetConfigurationOptions' (returns error code or SOAP_OK)
int PTZBindingService::GetConfigurationOptions(_tptz__GetConfigurationOptions *tptz__GetConfigurationOptions, _tptz__GetConfigurationOptionsResponse *tptz__GetConfigurationOptionsResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions = soap_new_tt__PTZConfigurationOptions(this->soap);

	// tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->PTZRamps = soap_new_tt__IntAttrList(this->soap);

	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces = getPTZSpaces(this->soap);

	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->PTZTimeout = soap_new_req_tt__DurationRange(this->soap, 1, 30);

	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->PTControlDirection = soap_new_tt__PTControlDirectionOptions(this->soap);

	return SOAP_OK;
}

/// Web service operation 'GotoHomePosition' (returns error code or SOAP_OK)
int PTZBindingService::GotoHomePosition(_tptz__GotoHomePosition *tptz__GotoHomePosition, _tptz__GotoHomePositionResponse *tptz__GotoHomePositionResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	ServiceContext::GetInstance()->getPTZController()->go_home_position();

	return SOAP_OK;
}

/// Web service operation 'SetHomePosition' (returns error code or SOAP_OK)
int PTZBindingService::SetHomePosition(_tptz__SetHomePosition *tptz__SetHomePosition, _tptz__SetHomePositionResponse *tptz__SetHomePositionResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	ServiceContext::GetInstance()->getPTZController()->set_home_preset();

	return SOAP_OK;
}

/// Web service operation 'ContinuousMove' (returns error code or SOAP_OK)
int PTZBindingService::ContinuousMove(_tptz__ContinuousMove *tptz__ContinuousMove, _tptz__ContinuousMoveResponse *tptz__ContinuousMoveResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	if( tptz__ContinuousMove->Velocity )
	{
		if( tptz__ContinuousMove->Velocity->PanTilt )
			ServiceContext::GetInstance()->getPTZController()->continuous_move(
				tptz__ContinuousMove->Velocity->PanTilt->x,
				tptz__ContinuousMove->Velocity->PanTilt->y
			);
	}

	return SOAP_OK;
}

/// Web service operation 'RelativeMove' (returns error code or SOAP_OK)
int PTZBindingService::RelativeMove(_tptz__RelativeMove *tptz__RelativeMove, _tptz__RelativeMoveResponse *tptz__RelativeMoveResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	ServiceContext::GetInstance()->getPTZController()->relative_move(
		tptz__RelativeMove->Translation->PanTilt->x,
		tptz__RelativeMove->Translation->PanTilt->y
	);

	return SOAP_OK;
}

/// Web service operation 'SendAuxiliaryCommand' (returns error code or SOAP_OK)
int PTZBindingService::SendAuxiliaryCommand(_tptz__SendAuxiliaryCommand *tptz__SendAuxiliaryCommand, _tptz__SendAuxiliaryCommandResponse *tptz__SendAuxiliaryCommandResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	return SOAP_OK;
}

/// Web service operation 'AbsoluteMove' (returns error code or SOAP_OK)
int PTZBindingService::AbsoluteMove(_tptz__AbsoluteMove *tptz__AbsoluteMove, _tptz__AbsoluteMoveResponse *tptz__AbsoluteMoveResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	ServiceContext::GetInstance()->getPTZController()->absolute_move(
		tptz__AbsoluteMove->Position->PanTilt->x,
		tptz__AbsoluteMove->Position->PanTilt->y
	);

	return SOAP_OK;
}

/// Web service operation 'GeoMove' (returns error code or SOAP_OK)
int PTZBindingService::GeoMove(_tptz__GeoMove *tptz__GeoMove, _tptz__GeoMoveResponse *tptz__GeoMoveResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	return SOAP_OK;
}

/// Web service operation 'Stop' (returns error code or SOAP_OK)
int PTZBindingService::Stop(_tptz__Stop *tptz__Stop, _tptz__StopResponse *tptz__StopResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	ServiceContext::GetInstance()->getPTZController()->stop();

	return SOAP_OK;
}

/// Web service operation 'GetPresetTours' (returns error code or SOAP_OK)
int PTZBindingService::GetPresetTours(_tptz__GetPresetTours *tptz__GetPresetTours, _tptz__GetPresetToursResponse *tptz__GetPresetToursResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	return SOAP_OK;
}

/// Web service operation 'GetPresetTour' (returns error code or SOAP_OK)
int PTZBindingService::GetPresetTour(_tptz__GetPresetTour *tptz__GetPresetTour, _tptz__GetPresetTourResponse *tptz__GetPresetTourResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	return SOAP_OK;
}

/// Web service operation 'GetPresetTourOptions' (returns error code or SOAP_OK)
int PTZBindingService::GetPresetTourOptions(_tptz__GetPresetTourOptions *tptz__GetPresetTourOptions, _tptz__GetPresetTourOptionsResponse *tptz__GetPresetTourOptionsResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	return SOAP_OK;
}

/// Web service operation 'CreatePresetTour' (returns error code or SOAP_OK)
int PTZBindingService::CreatePresetTour(_tptz__CreatePresetTour *tptz__CreatePresetTour, _tptz__CreatePresetTourResponse *tptz__CreatePresetTourResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	return SOAP_OK;
}

/// Web service operation 'ModifyPresetTour' (returns error code or SOAP_OK)
int PTZBindingService::ModifyPresetTour(_tptz__ModifyPresetTour *tptz__ModifyPresetTour, _tptz__ModifyPresetTourResponse *tptz__ModifyPresetTourResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	return SOAP_OK;
}

/// Web service operation 'OperatePresetTour' (returns error code or SOAP_OK)
int PTZBindingService::OperatePresetTour(_tptz__OperatePresetTour *tptz__OperatePresetTour, _tptz__OperatePresetTourResponse *tptz__OperatePresetTourResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	return SOAP_OK;
}

/// Web service operation 'RemovePresetTour' (returns error code or SOAP_OK)
int PTZBindingService::RemovePresetTour(_tptz__RemovePresetTour *tptz__RemovePresetTour, _tptz__RemovePresetTourResponse *tptz__RemovePresetTourResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	return SOAP_OK;
}

/// Web service operation 'GetCompatibleConfigurations' (returns error code or SOAP_OK)
int PTZBindingService::GetCompatibleConfigurations(_tptz__GetCompatibleConfigurations *tptz__GetCompatibleConfigurations, _tptz__GetCompatibleConfigurationsResponse *tptz__GetCompatibleConfigurationsResponse)
{
	DEBUG_MSG("PTZ: %s\n", __FUNCTION__);
	if( ! ServiceContext::GetInstance()->has_ptz() )
		return SOAP_OK;

	tptz__GetCompatibleConfigurationsResponse->PTZConfiguration = getPTZConfigurations(this->soap);

	return SOAP_OK;
}
