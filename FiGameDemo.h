#ifndef FiGameDemo_h__
#define FiGameDemo_h__

//#ifdef FiGameDemo_EXPORTS
//#define FiGameDemo_API __declspec(dllexport)
//#else
//#define FiGameDemo_API __declspec(dllimport)
//#endif
#define FiGameDemo_API
#include "EEditorSheetManager.h"
extern "C"
{
	FiGameDemo_API int FiGameDemo_Init(HWND mainWnd,EEditorManager* ptopertySheet);
	FiGameDemo_API void FiGameDemo_Update();
	FiGameDemo_API void FiGameDemo_ShutDown();
}



#endif // FiGameDemo_h__
