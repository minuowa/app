#pragma once

#include "GDevice_D3D.h"
#include "XSingleton.h"

#define DI_BUTTONNULL 0
#define DI_BUTTONTRUE 1
#define DI_BUTTONUP 3
#define DI_BUTTONDOWN 5
#define DI_BUTTONCLICK 11
#define DI_BUTTONDBCLICK 27

enum eButtonType
{
    eButtonType_LeftButton, 
	eButtonType_RightButton,
	eButtonType_MiddleButton,
	eButtonType_Count,
};
class GDevice_D3D;
class GD8Input
{
public:
	enum {KEY_COUNT=256,};

	GD8Input ( void );
    ~GD8Input ( void );
    void Reset();
    /////״̬
    //bool IsKeyDown(byte key);
    bool IskeyUp ( byte key );
    bool IsPressKey ( byte key );
    //bool IsPressing(byte key);


    ////////����
    bool IsPressingButton ( eButtonType bt );
    byte GetButtonAction ( eButtonType bt ) const;
    byte GetKeyAction ( int key );
    bool IsLeftButtonUp() const;
    bool IsMiddleButtonUp() const;
    bool IsRightButtonUp() const;

    //////////////////////////////////////////////////////////////////////////
    /////λ��
    bool IsWheelMButton();
    POINT GetMousePoint();
    POINT GetMouseMove();
    D3DVECTOR GetMouseMoveEX();
    int GetMouseWheel();

    void FreezeMouse ( bool bFreeze );
    void Update();
    void Active ( bool active );
    bool Init ( const GDevice_D3D& device, HINSTANCE hInst, HWND hWin );
private:
    void UpdateState();

    HINSTANCE mInstance;
    HWND mWinID;
    LPDIRECTINPUT8 mDI;
    LPDIRECTINPUTDEVICE8 mKboard;
    LPDIRECTINPUTDEVICE8 mMouse;

    DIMOUSESTATE mMouseData;
    POINT mCurMousePositon;
    byte mMouseButtonStateOld[eButtonType_Count];
    byte mMouseButtonState[eButtonType_Count];//0�����1�Ҽ���2�м�
    byte mKBoardState[KEY_COUNT];
    byte mKBoardStateOld[KEY_COUNT];
	BYTE mKeyData[KEY_COUNT];
    bool mActive;
    bool mNeedClearMouseMoveDelta;
};
typedef CXSingleton<GD8Input> GSingletonD8Input;
#define	INPUTSYSTEM GSingletonD8Input::GetSingleton()