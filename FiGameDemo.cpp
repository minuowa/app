#include "GGameDemoHeader.h"
#include "FiGameDemo.h"
#include "GGame.h"
//extern "C"
//{
	FiGameDemo_API int FiGameDemo_Init ( HWND mainWnd )
	{
		return TheGame->initBase ( mainWnd);
	}
	FiGameDemo_API void FiGameDemo_Update()
	{
		TheGame->Loop();
	}
	FiGameDemo_API void FiGameDemo_ShutDown()
	{
		TheGame->ShutDown();
	}
//}