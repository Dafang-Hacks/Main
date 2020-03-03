#include "soapDeviceIOBindingService.h"
#include "ServiceContext.h"
#include "soap_helpers.h"
#include "smacros.h"

/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetServiceCapabilities(_tmd__GetServiceCapabilities *tmd__GetServiceCapabilities, _tmd__GetServiceCapabilitiesResponse *tmd__GetServiceCapabilitiesResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);

		tmd__GetServiceCapabilitiesResponse->Capabilities = getDeviceIOServiceCapabilities(this->soap);

		return SOAP_OK;
}

/// Web service operation 'GetRelayOutputOptions' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetRelayOutputOptions(_tmd__GetRelayOutputOptions *tmd__GetRelayOutputOptions, _tmd__GetRelayOutputOptionsResponse *tmd__GetRelayOutputOptionsResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetAudioSources' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetAudioSources(tmd__Get *tmd__GetAudioSources, tmd__GetResponse *tmd__GetAudioSourcesResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetAudioOutputs' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetAudioOutputs(tmd__Get *tmd__GetAudioOutputs, tmd__GetResponse *tmd__GetAudioOutputsResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetVideoSources' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetVideoSources(tmd__Get *tmd__GetVideoSources, tmd__GetResponse *tmd__GetVideoSourcesResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetVideoOutputs' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetVideoOutputs(_tmd__GetVideoOutputs *tmd__GetVideoOutputs, _tmd__GetVideoOutputsResponse *tmd__GetVideoOutputsResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetVideoSourceConfiguration' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetVideoSourceConfiguration(_tmd__GetVideoSourceConfiguration *tmd__GetVideoSourceConfiguration, _tmd__GetVideoSourceConfigurationResponse *tmd__GetVideoSourceConfigurationResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetVideoOutputConfiguration' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetVideoOutputConfiguration(_tmd__GetVideoOutputConfiguration *tmd__GetVideoOutputConfiguration, _tmd__GetVideoOutputConfigurationResponse *tmd__GetVideoOutputConfigurationResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetAudioSourceConfiguration' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetAudioSourceConfiguration(_tmd__GetAudioSourceConfiguration *tmd__GetAudioSourceConfiguration, _tmd__GetAudioSourceConfigurationResponse *tmd__GetAudioSourceConfigurationResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetAudioOutputConfiguration' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetAudioOutputConfiguration(_tmd__GetAudioOutputConfiguration *tmd__GetAudioOutputConfiguration, _tmd__GetAudioOutputConfigurationResponse *tmd__GetAudioOutputConfigurationResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'SetVideoSourceConfiguration' (returns error code or SOAP_OK)
int DeviceIOBindingService::SetVideoSourceConfiguration(_tmd__SetVideoSourceConfiguration *tmd__SetVideoSourceConfiguration, _tmd__SetVideoSourceConfigurationResponse *tmd__SetVideoSourceConfigurationResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'SetVideoOutputConfiguration' (returns error code or SOAP_OK)
int DeviceIOBindingService::SetVideoOutputConfiguration(_tmd__SetVideoOutputConfiguration *tmd__SetVideoOutputConfiguration, _tmd__SetVideoOutputConfigurationResponse *tmd__SetVideoOutputConfigurationResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'SetAudioSourceConfiguration' (returns error code or SOAP_OK)
int DeviceIOBindingService::SetAudioSourceConfiguration(_tmd__SetAudioSourceConfiguration *tmd__SetAudioSourceConfiguration, _tmd__SetAudioSourceConfigurationResponse *tmd__SetAudioSourceConfigurationResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'SetAudioOutputConfiguration' (returns error code or SOAP_OK)
int DeviceIOBindingService::SetAudioOutputConfiguration(_tmd__SetAudioOutputConfiguration *tmd__SetAudioOutputConfiguration, _tmd__SetAudioOutputConfigurationResponse *tmd__SetAudioOutputConfigurationResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetVideoSourceConfigurationOptions' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetVideoSourceConfigurationOptions(_tmd__GetVideoSourceConfigurationOptions *tmd__GetVideoSourceConfigurationOptions, _tmd__GetVideoSourceConfigurationOptionsResponse *tmd__GetVideoSourceConfigurationOptionsResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetVideoOutputConfigurationOptions' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetVideoOutputConfigurationOptions(_tmd__GetVideoOutputConfigurationOptions *tmd__GetVideoOutputConfigurationOptions, _tmd__GetVideoOutputConfigurationOptionsResponse *tmd__GetVideoOutputConfigurationOptionsResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetAudioSourceConfigurationOptions' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetAudioSourceConfigurationOptions(_tmd__GetAudioSourceConfigurationOptions *tmd__GetAudioSourceConfigurationOptions, _tmd__GetAudioSourceConfigurationOptionsResponse *tmd__GetAudioSourceConfigurationOptionsResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetAudioOutputConfigurationOptions' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetAudioOutputConfigurationOptions(_tmd__GetAudioOutputConfigurationOptions *tmd__GetAudioOutputConfigurationOptions, _tmd__GetAudioOutputConfigurationOptionsResponse *tmd__GetAudioOutputConfigurationOptionsResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetRelayOutputs' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetRelayOutputs(_tds__GetRelayOutputs *tds__GetRelayOutputs, _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'SetRelayOutputSettings' (returns error code or SOAP_OK)
int DeviceIOBindingService::SetRelayOutputSettings(_tmd__SetRelayOutputSettings *tmd__SetRelayOutputSettings, _tmd__SetRelayOutputSettingsResponse *tmd__SetRelayOutputSettingsResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'SetRelayOutputState' (returns error code or SOAP_OK)
int DeviceIOBindingService::SetRelayOutputState(_tds__SetRelayOutputState *tds__SetRelayOutputState, _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetDigitalInputs' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetDigitalInputs(_tmd__GetDigitalInputs *tmd__GetDigitalInputs, _tmd__GetDigitalInputsResponse *tmd__GetDigitalInputsResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetDigitalInputConfigurationOptions' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetDigitalInputConfigurationOptions(_tmd__GetDigitalInputConfigurationOptions *tmd__GetDigitalInputConfigurationOptions, _tmd__GetDigitalInputConfigurationOptionsResponse *tmd__GetDigitalInputConfigurationOptionsResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'SetDigitalInputConfigurations' (returns error code or SOAP_OK)
int DeviceIOBindingService::SetDigitalInputConfigurations(_tmd__SetDigitalInputConfigurations *tmd__SetDigitalInputConfigurations, _tmd__SetDigitalInputConfigurationsResponse *tmd__SetDigitalInputConfigurationsResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetSerialPorts' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetSerialPorts(_tmd__GetSerialPorts *tmd__GetSerialPorts, _tmd__GetSerialPortsResponse *tmd__GetSerialPortsResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetSerialPortConfiguration' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetSerialPortConfiguration(_tmd__GetSerialPortConfiguration *tmd__GetSerialPortConfiguration, _tmd__GetSerialPortConfigurationResponse *tmd__GetSerialPortConfigurationResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'SetSerialPortConfiguration' (returns error code or SOAP_OK)
int DeviceIOBindingService::SetSerialPortConfiguration(_tmd__SetSerialPortConfiguration *tmd__SetSerialPortConfiguration, _tmd__SetSerialPortConfigurationResponse *tmd__SetSerialPortConfigurationResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetSerialPortConfigurationOptions' (returns error code or SOAP_OK)
int DeviceIOBindingService::GetSerialPortConfigurationOptions(_tmd__GetSerialPortConfigurationOptions *tmd__GetSerialPortConfigurationOptions, _tmd__GetSerialPortConfigurationOptionsResponse *tmd__GetSerialPortConfigurationOptionsResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'SendReceiveSerialCommand' (returns error code or SOAP_OK)
int DeviceIOBindingService::SendReceiveSerialCommand(_tmd__SendReceiveSerialCommand *tmd__SendReceiveSerialCommand, _tmd__SendReceiveSerialCommandResponse *tmd__SendReceiveSerialCommandResponse)
{
		DEBUG_MSG("DeviceIO: %s\n", __FUNCTION__);
		return SOAP_OK;
}
