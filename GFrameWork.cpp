#include "GGameDemoHeader.h"
#include "GFrameWork.h"

#include "XCursor.h"

#include "GGame.h"

GFrameWork::GFrameWork(void)
	:mIsActive(true)
	,mMainWin(0)
	,mInst(0)
{
}

GFrameWork::~GFrameWork(void)
{
}


bool GFrameWork::InitBase(HWND mainWnd)
{
	mInst=GetModuleHandle(NULL);
	if (!mainWnd)
	{
		//获取本实例的进程
		WNDCLASSEX wcex;
		wcex.cbClsExtra=0;
		wcex.cbSize=sizeof(wcex);
		wcex.cbWndExtra=0;
		//wcex.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
		wcex.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);

		HCURSOR hCursor=gCursor.GetNowCursor();
		if (hCursor!=NULL)
		{
			wcex.hCursor=hCursor;
		}
		else
		{
			wcex.hCursor=LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW));
		}

		wcex.hIcon=LoadIcon(mInst,MAKEINTRESOURCE(IDI_APPLICATION));
		wcex.hIconSm=NULL;
		wcex.hInstance=mInst;
		wcex.lpfnWndProc=WndProc;
		wcex.lpszClassName=_T("GameDemo");
		wcex.lpszMenuName=NULL;
		wcex.style=CS_HREDRAW|CS_VREDRAW;
		if (!RegisterClassEx(&wcex))
		{
			return false;
		}

		HWND hwnd=NULL;
		hwnd=CreateWindowEx(NULL,_T("GameDemo"),_T("GameDemo"),WS_OVERLAPPEDWINDOW|WS_VISIBLE,0,0,
			WINDOW_WIDTH,WINDOW_HEIGHT,NULL,NULL,mInst,NULL);
		if (hwnd==NULL)
		{
			return false;
		}
		else
		{
			mMainWin=hwnd;
		}

		ShowWindow(mMainWin,SW_NORMAL);
		UpdateWindow(mMainWin);
	}
	else
	{
		mMainWin=mainWnd;
	}

	return true;

}

void GFrameWork::Active( bool active )
{
	mIsActive = active;

	GD8Input::GetSingleton().Active(active);
}

bool GFrameWork::IsActive() const
{
	return mIsActive;
}



LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	HIMC hIMC;
	DWORD dwSize;
	HGLOBAL hstr;
	LPSTR lpstr;
	int x=3;
	switch (message)
	{
	case WM_CLOSE:
		//PostMessage(hWnd,WM_DESTROY,0,0);
		break;
	case WM_KILLFOCUS:
	case WA_INACTIVE:
		TheGame->Active(false);
		break;
	case WM_SETFOCUS:
	case WM_ACTIVATE:
		TheGame->Active(true);
		break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;

	case WM_SIZE:

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
		break;
		//return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
