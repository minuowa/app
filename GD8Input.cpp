#include "GGameDemoHeader.h"

#include "GD8Input.h"

CXImpleteSingleton(GD8Input);

GD8Input::GD8Input(void)
	:mActive(true)
	,mNeedClearMouseMoveDelta(false)
{
	mKboard=NULL;
	mMouse=NULL;
	mDI=NULL;

	Reset();
}

GD8Input::~GD8Input(void)
{

}



HRESULT GD8Input::Init( HINSTANCE hInst,HWND hWin )
{
	mInst=hInst;
	mHwin=hWin;
	CoInitialize(NULL);
	HRESULT hr=DirectInput8Create(mInst,DIRECTINPUT_HEADER_VERSION,IID_IDirectInput8,(LPVOID*)&mDI,NULL);
	if (FAILED(hr))
	{
		return hr;
	}

	hr=mDI->CreateDevice(GUID_SysKeyboard,&mKboard,NULL);
	if (FAILED(hr))
	{
		return hr;
	}
	mKboard->SetDataFormat(&c_dfDIKeyboard);

#ifdef _DEBUG
	mKboard->SetCooperativeLevel(mHwin,DISCL_BACKGROUND|DISCL_NONEXCLUSIVE);
#else
	mKboard->SetCooperativeLevel(mHwin,DISCL_NONEXCLUSIVE|DISCL_BACKGROUND);
#endif

	//调试用DISCL_NONEXCLUSIVE|DISCL_BACKGROUND
	//正式版本用：DISCL_EXCLUSIVE|DISCL_
	hr=mDI->CreateDevice(GUID_SysMouse,&mMouse,NULL);
	if (FAILED(hr))
	{
		return hr;
	}
	mMouse->SetDataFormat(&c_dfDIMouse);
#ifdef _DEBUG
	//mMouse->SetCooperativeLevel(mHwin,DISCL_BACKGROUND|DISCL_NONEXCLUSIVE);
	mMouse->SetCooperativeLevel(mHwin,DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);
#else
	mMouse->SetCooperativeLevel(mHwin,DISCL_NONEXCLUSIVE|DISCL_BACKGROUND);
#endif
	mKboard->Acquire();
	mMouse->Acquire();
	return S_OK;
}

void GD8Input::Update()
{
	//自身先清零，然后获取当前状态

	if (!mActive)
		return;

	//-------------------------------------------------------------------------
	// 此处多做一次UpdateState()是为了清除鼠标中数据的增量
	//-------------------------------------------------------------------------
		
	if(mNeedClearMouseMoveDelta)
	{
		UpdateState();
		mNeedClearMouseMoveDelta=false;
	}

	UpdateState();
}

bool GD8Input::IsPressKey( byte key )
{
	return mKeyData[key]==0x80;
}



POINT GD8Input::GetMouseMove()
{
	POINT Pt;
	Pt.x=mMouseData.lX;
	Pt.y=mMouseData.lY;
	return Pt;
}

D3DVECTOR GD8Input::GetMouseMoveEX()
{
	D3DVECTOR vPos;
	vPos=Vector(mMouseData.lX,mMouseData.lY,mMouseData.lZ);
	return vPos;
}

POINT GD8Input::GetMousePoint()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(mHwin,&pt);

	RECT rcNow;
	GetClientRect(mHwin,&rcNow);

	float fWidth=(float)(rcNow.right-rcNow.left);
	float fHeight=(float)(rcNow.bottom-rcNow.top);

	pt.x*=((float)D9DEVICE->mWidth)/fWidth;
	pt.y*=((float)D9DEVICE->mHeight)/fHeight;

	return pt;
}

int GD8Input::GetMouseWheel()
{   
	return mMouseData.lZ;
}

byte GD8Input::GetButtonAction( ButtonType bt ) const
{
	return mMouseButtonState[bt];
}

bool GD8Input::IsPressingButton( ButtonType bt )
{
	if (bt==btLB)
	{
		return mMouseData.rgbButtons[0];
	}
	else if (bt==btMB)
	{
		return mMouseData.rgbButtons[2];
	}
	else
	{
		return mMouseData.rgbButtons[1];
	}
}

byte GD8Input::GetKeyAction( int key )
{
	return mKBoardState[key];
}

bool GD8Input::IskeyUp( byte key )
{
	return GetKeyAction(key)==DI_BUTTONUP;
}

bool GD8Input::IsLeftButtonUp() const
{
	return GetButtonAction(btLB)==DI_BUTTONUP;
}

bool GD8Input::IsMiddleButtonUp() const
{
	return GetButtonAction(btMB)==DI_BUTTONUP;
}

bool GD8Input::IsRightButtonUp() const
{
	return GetButtonAction(btRB)==DI_BUTTONUP;
}

void GD8Input::Active( bool active )
{
	mActive = active;
	mNeedClearMouseMoveDelta = true;
}

void GD8Input::Reset()
{
	mIsFreeze=false;
	ZeroMemory(&mMouseData,sizeof(mMouseData));
	ZeroMemory(mOldMouseButtonState,sizeof(mOldMouseButtonState));

	ZeroMemory(mKeyData,sizeof(mKeyData));
	ZeroMemory(mOldKBoardState,sizeof(mOldKBoardState));
	mPosNowMouse.x=0;
	mPosNowMouse.y=0;
}

void GD8Input::UpdateState()
{
	if (mKboard->GetDeviceState(256,mKeyData)==DIERR_INPUTLOST)
	{
		mKboard->Acquire();
		mKboard->GetDeviceState(256,mKeyData);
	}

	if (mMouse->GetDeviceState((sizeof(mMouseData)),&mMouseData)==DIERR_INPUTLOST)
	{
		mMouse->Acquire();
		mMouse->GetDeviceState((sizeof(mMouseData)),&mMouseData);
	}

	//////确定鼠标动作
	for (int i=0;i<3;i++)
	{
		if (!mOldMouseButtonState[i])
		{
			if (mMouseData.rgbButtons[i]==0x80)
			{
				mMouseButtonState[i]=DI_BUTTONDOWN;//发生按下动作
			}
			else
			{
				mMouseButtonState[i]=DI_BUTTONNULL;
			}
		}
		else
		{
			if (mMouseData.rgbButtons[i]==0x80)
			{
				mMouseButtonState[i]=DI_BUTTONNULL;
			}
			else
			{
				mMouseButtonState[i]=DI_BUTTONUP;//发生跳起动作
			}
		}
		mOldMouseButtonState[i]=(mMouseData.rgbButtons[i]==0x80);
	}

	//////确定键盘动作
	for (int i=0;i<256;i++)
	{
		if (!mOldKBoardState[i])
		{
			if (mKeyData[i]==0x80)
			{
				mKBoardState[i]=DI_BUTTONDOWN;//发生按下动作
			}
			else
			{
				mKBoardState[i]=DI_BUTTONNULL;
			}
		}
		else
		{
			if (mKeyData[i]==0x80)
			{
				mKBoardState[i]=DI_BUTTONNULL;
			}
			else
			{
				mKBoardState[i]=DI_BUTTONUP;//发生跳起动作
			}
		}
		mOldKBoardState[i]=(mKeyData[i]&0x80);
	}
}
