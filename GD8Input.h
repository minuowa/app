#pragma once

#include "GD9Device.h"
#include "XSingleton.h"

#define DI_BUTTONNULL 0
#define DI_BUTTONTRUE 1 
#define DI_BUTTONUP 3	   
#define DI_BUTTONDOWN 5
#define DI_BUTTONCLICK 11
#define DI_BUTTONDBCLICK 27

enum ButtonType
{
    btLB,btRB,btMB
};

class GD8Input
{
	CXDeclareSingleton(GD8Input);
public:
    GD8Input(void);
	~GD8Input(void);
	void Reset();
	/////×´Ì¬
    //bool IsKeyDown(byte key);
    bool IskeyUp(byte key);
    bool IsPressKey(byte key);
    //bool IsPressing(byte key);

 
    ////////¶¯×÷
    bool IsPressingButton(ButtonType bt);
	byte GetButtonAction(ButtonType bt) const;
    byte GetKeyAction(int key);
	bool IsLeftButtonUp() const;
	bool IsMiddleButtonUp() const;
	bool IsRightButtonUp() const;

    //////////////////////////////////////////////////////////////////////////
    /////Î»ÖÃ
    bool IsWheelMButton();
    POINT GetMousePoint();
    POINT GetMouseMove();
    D3DVECTOR GetMouseMoveEX();
    int GetMouseWheel();

    void FreezeMouse(bool bFreeze);
    void Update();
	void Active(bool active);
	HRESULT Init(HINSTANCE hInst,HWND hWin);
private:
	void UpdateState();

    HINSTANCE mInst;
    HWND mHwin;
    LPDIRECTINPUT8 mDI;
    LPDIRECTINPUTDEVICE8 mKboard;
    LPDIRECTINPUTDEVICE8 mMouse;

    BYTE mKeyData[256];
    DIMOUSESTATE mMouseData;
    POINT mPosNowMouse;
    bool mIsFreeze;
    byte mOldMouseButtonState[3];
    byte mMouseButtonState[3];//0×ó¼ü£¬1ÓÒ¼ü£¬2ÖÐ¼ü
    byte mKBoardState[256],mOldKBoardState[256];
	bool mActive;
	bool mNeedClearMouseMoveDelta;
};
#define INPUTSYSTEM GD8Input::GetSingleton() 