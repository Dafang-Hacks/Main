/*
 --------------------------------------------------------------------------
 ServiceReceiver.cpp

 Implementation of functions (methods) for the service:
 ONVIF receiver.wsdl server side
-----------------------------------------------------------------------------
*/

#include "../../generated/soapRemoteDiscoveryBindingService.h"
#include "ServiceContext.h"
#include "soap_helpers.h"
#include "smacros.h"

/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
/// Web service operation 'Hello' (returns error code or SOAP_OK)
int RemoteDiscoveryBindingService::Hello(struct wsdd__HelloType tdn__Hello, struct wsdd__ResolveType &tdn__HelloResponse)
{
	DEBUG_MSG("Remote Discovery: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service operation 'Bye' (returns error code or SOAP_OK)
int RemoteDiscoveryBindingService::Bye(struct wsdd__ByeType tdn__Bye, struct wsdd__ResolveType &tdn__ByeResponse)
{
	DEBUG_MSG("Remote Discovery: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service operation 'Probe' (returns error code or SOAP_OK)
int RemoteDiscoveryBindingService::Probe(struct wsdd__ProbeType tdn__Probe, struct wsdd__ProbeMatchesType &tdn__ProbeResponse)
{
	DEBUG_MSG("Remote Discovery: %s\n", __FUNCTION__);
	return SOAP_OK;
}
