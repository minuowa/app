#include "GGameDemoHeader.h"
#include "XEvent.h"

CXEvent gEvent;

CXEvent::CXEvent(void)
{
	mGHandle=NULL;
}

CXEvent::~CXEvent(void)
{
}



GHandle CXEvent::Create( char *sEventName )
{
	GHandle GHandle=OpenEventA(EVENT_ALL_ACCESS,FALSE,sEventName);

	if (GHandle==NULL)
	{
		GHandle=CreateEventA(NULL,TRUE,TRUE,sEventName);
	}

	if (GHandle!=NULL)
	{
		mGHandle=GHandle;
	}

	return mGHandle;
}

void CXEvent::SetUsed()
{
	if (mGHandle!=NULL)
	{
		ResetEvent(mGHandle);
	}
}

void CXEvent::SetNoUse()
{
	if (mGHandle!=NULL)
	{
		SetEvent(mGHandle);
	}
}

void CXEvent::WaitForUse( DWORD dwWaitTime )
{
	if (mGHandle!=NULL)
	{
		WaitForSingleObject(mGHandle,dwWaitTime);
	}
}
