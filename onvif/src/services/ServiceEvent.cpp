#include "soapPullPointSubscriptionBindingService.h"
#include "ServiceContext.h"
#include "soap_helpers.h"
#include "smacros.h"


/// Web service operation 'PullMessages' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::PullMessages(_tev__PullMessages *tev__PullMessages, _tev__PullMessagesResponse *tev__PullMessagesResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'Seek' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::Seek(_tev__Seek *tev__Seek, _tev__SeekResponse *tev__SeekResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'SetSynchronizationPoint' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::SetSynchronizationPoint(_tev__SetSynchronizationPoint *tev__SetSynchronizationPoint, _tev__SetSynchronizationPointResponse *tev__SetSynchronizationPointResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'Unsubscribe' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::Unsubscribe(_wsnt__Unsubscribe *wsnt__Unsubscribe, _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetServiceCapabilities' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::GetServiceCapabilities(_tev__GetServiceCapabilities *tev__GetServiceCapabilities, _tev__GetServiceCapabilitiesResponse *tev__GetServiceCapabilitiesResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'CreatePullPointSubscription' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::CreatePullPointSubscription(_tev__CreatePullPointSubscription *tev__CreatePullPointSubscription, _tev__CreatePullPointSubscriptionResponse *tev__CreatePullPointSubscriptionResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetEventProperties' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::GetEventProperties(_tev__GetEventProperties *tev__GetEventProperties, _tev__GetEventPropertiesResponse *tev__GetEventPropertiesResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'Renew' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::Renew(_wsnt__Renew *wsnt__Renew, _wsnt__RenewResponse *wsnt__RenewResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'Unsubscribe' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::Unsubscribe_(_wsnt__Unsubscribe *wsnt__Unsubscribe, _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'Subscribe' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::Subscribe(_wsnt__Subscribe *wsnt__Subscribe, _wsnt__SubscribeResponse *wsnt__SubscribeResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetCurrentMessage' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::GetCurrentMessage(_wsnt__GetCurrentMessage *wsnt__GetCurrentMessage, _wsnt__GetCurrentMessageResponse *wsnt__GetCurrentMessageResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service one-way operation 'Notify' (return error code, SOAP_OK (no response), or send_Notify_empty_response())
int PullPointSubscriptionBindingService::Notify(_wsnt__Notify *wsnt__Notify)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'GetMessages' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::GetMessages(_wsnt__GetMessages *wsnt__GetMessages, _wsnt__GetMessagesResponse *wsnt__GetMessagesResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'DestroyPullPoint' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::DestroyPullPoint(_wsnt__DestroyPullPoint *wsnt__DestroyPullPoint, _wsnt__DestroyPullPointResponse *wsnt__DestroyPullPointResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service one-way operation 'Notify' (return error code, SOAP_OK (no response), or send_Notify_empty_response())
int PullPointSubscriptionBindingService::Notify_(_wsnt__Notify *wsnt__Notify)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'CreatePullPoint' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::CreatePullPoint(_wsnt__CreatePullPoint *wsnt__CreatePullPoint, _wsnt__CreatePullPointResponse *wsnt__CreatePullPointResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'Renew' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::Renew_(_wsnt__Renew *wsnt__Renew, _wsnt__RenewResponse *wsnt__RenewResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'Unsubscribe' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::Unsubscribe__(_wsnt__Unsubscribe *wsnt__Unsubscribe, _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'PauseSubscription' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::PauseSubscription(_wsnt__PauseSubscription *wsnt__PauseSubscription, _wsnt__PauseSubscriptionResponse *wsnt__PauseSubscriptionResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}

/// Web service operation 'ResumeSubscription' (returns error code or SOAP_OK)
int PullPointSubscriptionBindingService::ResumeSubscription(_wsnt__ResumeSubscription *wsnt__ResumeSubscription, _wsnt__ResumeSubscriptionResponse *wsnt__ResumeSubscriptionResponse)
{
		DEBUG_MSG("Event: %s\n", __FUNCTION__);
		return SOAP_OK;
}
