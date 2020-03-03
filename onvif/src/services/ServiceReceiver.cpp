/*
 --------------------------------------------------------------------------
 ServiceReceiver.cpp

 Implementation of functions (methods) for the service:
 ONVIF receiver.wsdl server side
-----------------------------------------------------------------------------
*/

#include "../../generated/soapReceiverBindingService.h"
#include "ServiceContext.h"
#include "soap_helpers.h"
#include "smacros.h"

/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
int ReceiverBindingService::GetServiceCapabilities(_trv__GetServiceCapabilities *trv__GetServiceCapabilities, _trv__GetServiceCapabilitiesResponse *trv__GetServiceCapabilitiesResponse)
{
	DEBUG_MSG("Receiver: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service operation 'GetReceivers' (returns error code or SOAP_OK)
int ReceiverBindingService::GetReceivers(_trv__GetReceivers *trv__GetReceivers, _trv__GetReceiversResponse *trv__GetReceiversResponse)
{
	DEBUG_MSG("Receiver: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service operation 'GetReceiver' (returns error code or SOAP_OK)
int ReceiverBindingService::GetReceiver(_trv__GetReceiver *trv__GetReceiver, _trv__GetReceiverResponse *trv__GetReceiverResponse)
{
	DEBUG_MSG("Receiver: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service operation 'CreateReceiver' (returns error code or SOAP_OK)
int ReceiverBindingService::CreateReceiver(_trv__CreateReceiver *trv__CreateReceiver, _trv__CreateReceiverResponse *trv__CreateReceiverResponse)
{
	DEBUG_MSG("Receiver: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service operation 'DeleteReceiver' (returns error code or SOAP_OK)
int ReceiverBindingService::DeleteReceiver(_trv__DeleteReceiver *trv__DeleteReceiver, _trv__DeleteReceiverResponse *trv__DeleteReceiverResponse)
{
	DEBUG_MSG("Receiver: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service operation 'ConfigureReceiver' (returns error code or SOAP_OK)
int ReceiverBindingService::ConfigureReceiver(_trv__ConfigureReceiver *trv__ConfigureReceiver, _trv__ConfigureReceiverResponse *trv__ConfigureReceiverResponse)
{
	DEBUG_MSG("Receiver: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service operation 'SetReceiverMode' (returns error code or SOAP_OK)
int ReceiverBindingService::SetReceiverMode(_trv__SetReceiverMode *trv__SetReceiverMode, _trv__SetReceiverModeResponse *trv__SetReceiverModeResponse)
{
	DEBUG_MSG("Receiver: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service operation 'GetReceiverState' (returns error code or SOAP_OK)
int ReceiverBindingService::GetReceiverState(_trv__GetReceiverState *trv__GetReceiverState, _trv__GetReceiverStateResponse *trv__GetReceiverStateResponse)
{
	DEBUG_MSG("Receiver: %s\n", __FUNCTION__);
	return SOAP_OK;
}
