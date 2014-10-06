#include "GGameDemoHeader.h"
#include "GFrameWork.h"

#include "XCursor.h"

#include "GGame.h"
#include "GD8Input.h"

GFrameWork::GFrameWork ( void )
    : mIsActive ( true )
    , mMainWin ( 0 )
    , mInst ( 0 )
{
}

GFrameWork::~GFrameWork ( void )
{
}


bool GFrameWork::initBase ( HWND mainWnd )
{
    mInst = GetModuleHandle ( NULL );
	DWORD errorCode=0;
	if ( !mainWnd )
    {
		const TCHAR* className=_T ( "1GameDemo" );
        //��ȡ��ʵ���Ľ���
        WNDCLASSEX wcex;
        wcex.cbClsExtra = 0;
        wcex.cbSize = sizeof ( wcex );
        wcex.cbWndExtra = 0;
        //wcex.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
        wcex.hbrBackground = ( HBRUSH ) GetStockObject ( BLACK_BRUSH );

        HCURSOR hCursor = gCursor.GetNowCursor();
        if ( hCursor != NULL )
        {
            wcex.hCursor = hCursor;
        }
        else
        {
            wcex.hCursor = LoadCursor ( NULL, MAKEINTRESOURCE ( IDC_ARROW ) );
        }

        wcex.hIcon = LoadIcon ( mInst, MAKEINTRESOURCE ( IDI_QUESTION ) );
        wcex.hIconSm = NULL;
        wcex.hInstance = mInst;
        wcex.lpfnWndProc = WndProc;
        wcex.lpszClassName = className;
        wcex.lpszMenuName = NULL;
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        if ( !RegisterClassEx ( &wcex ) )
        {
			DWORD errorCode=GetLastError();
            return false;
        }

        mainWnd = CreateWindowEx ( NULL, className, _T ( "GameDemo" ), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0,
                                WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, mInst, NULL );
		errorCode=GetLastError();
	}

	if (mainWnd)
	{
		mMainWin = mainWnd;
		ShowWindow ( mMainWin, SW_NORMAL );
		UpdateWindow ( mMainWin );
	}
    else
    {
		CXASSERT(0);
        return false;
    }

    return true;

}

void GFrameWork::active ( bool active )
{
    mIsActive = active;

    GSingletonD8Input::GetSingleton().Active ( active );
}

bool GFrameWork::isActive() const
{
    return mIsActive;
}

void GFrameWork::resize( int w,int h )
{
	Device->OnResize(w,h);
}



LRESULT CALLBACK WndProc ( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    //HIMC hIMC;
    //DWORD dwSize;
    //HGLOBAL hstr;
    //LPSTR lpstr;
    //int x = 3;
    switch ( message )
    {
    case WM_CLOSE:
        PostMessage(hWnd,WM_DESTROY,0,0);
        break;
    case WM_KILLFOCUS:
    case WA_INACTIVE:
        TheGame->active ( false );
        break;
    case WM_SETFOCUS:
    case WM_ACTIVATE:
        TheGame->active ( true );
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SIZE:
		{
			TheGame->resize(LOWORD(lParam),HIWORD(lParam));
		}
        break;
    case WM_IME_COMPOSITION:

        //if (lParam & GCS_RESULTSTR)
        //{
        //	hIMC = ImmGetContext(hWnd);

        //	dwSize = ImmGetCompositionString(hIMC, GCS_RESULTSTR, NULL, 0);

        //	dwSize += sizeof(WCHAR);

        //	ImmGetCompositionString(hIMC, GCS_RESULTSTR, lpstr, dwSize);

        //	ImmReleaseContext(hWnd, hIMC);
        //}

        break;

    default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
    }

    return 0;
}
