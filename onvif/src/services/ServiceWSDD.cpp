#include "../../generated/soapwsddService.h"
#include "ServiceContext.h"
#include "soap_helpers.h"
#include "smacros.h"

/// Web service one-way operation 'Hello' (return error code, SOAP_OK (no response), or send_Hello_empty_response())
int wsddService::Hello(struct wsdd__HelloType *wsdd__Hello)
{
	DEBUG_MSG("WSDD: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service one-way operation 'Bye' (return error code, SOAP_OK (no response), or send_Bye_empty_response())
int wsddService::Bye(struct wsdd__ByeType *wsdd__Bye)
{
	DEBUG_MSG("WSDD: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service one-way operation 'Probe' (return error code, SOAP_OK (no response), or send_Probe_empty_response())
int wsddService::Probe(struct wsdd__ProbeType *wsdd__Probe)
{
	DEBUG_MSG("WSDD: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service one-way operation 'ProbeMatches' (return error code, SOAP_OK (no response), or send_ProbeMatches_empty_response())
int wsddService::ProbeMatches(struct wsdd__ProbeMatchesType *wsdd__ProbeMatches)
{
	DEBUG_MSG("WSDD: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service one-way operation 'Resolve' (return error code, SOAP_OK (no response), or send_Resolve_empty_response())
int wsddService::Resolve(struct wsdd__ResolveType *wsdd__Resolve)
{
	DEBUG_MSG("WSDD: %s\n", __FUNCTION__);
	return SOAP_OK;
}

/// Web service one-way operation 'ResolveMatches' (return error code, SOAP_OK (no response), or send_ResolveMatches_empty_response())
int wsddService::ResolveMatches(struct wsdd__ResolveMatchesType *wsdd__ResolveMatches)
{
	DEBUG_MSG("WSDD: %s\n", __FUNCTION__);
	return SOAP_OK;
}
