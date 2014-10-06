#include "GGameDemoHeader.h"
#include "GGame.h"
#include "FiGameDemo.h"

#ifndef _LIB
int WINAPI WinMain ( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
    if ( !FiGameDemo_Init ( 0 ) )
    {
        return false;
    }
    MSG msg;
    ZeroMemory ( &msg, sizeof ( msg ) );

    while ( msg.message != WM_QUIT )
    {
        if ( PeekMessage ( &msg, 0, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage ( &msg );
            DispatchMessage ( &msg );
        }
        else
        {
            FiGameDemo_Update();
        }
    }
    FiGameDemo_ShutDown();
    return true;
}
#endif // !_DLL
//BOOL APIENTRY DllMain( HMODULE hModule,
//	DWORD  ul_reason_for_call,
//	LPVOID lpReserved
//	)
//{
//	switch (ul_reason_for_call)
//	{
//	case DLL_PROCESS_ATTACH:
//	case DLL_THREAD_ATTACH:
//	case DLL_THREAD_DETACH:
//	case DLL_PROCESS_DETACH:
//		break;
//	}
//	return TRUE;
//}