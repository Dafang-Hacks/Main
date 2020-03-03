#include "ServiceContext.h"

ServiceContext * ServiceContext::Instance = 0;
ServiceContext * ServiceContext::CreateInstance()
{
	ServiceContext::Instance = new ServiceContext();
	return ServiceContext::Instance;
}
ServiceContext * ServiceContext::GetInstance()
{
	return ServiceContext::Instance;
}
