#pragma once

#include "XEffect.h"
#include "GAmmo.h"
#include "Particles.h"
#include "XCursor.h"
#include "GAnimMeshObj.h"
#include "Sea.h"
#include "GMeshBuffer.h"
#include "GWorldCorrd.h"
#include "GGameMap.h"
#include "GMeshBaseObj.h"
#include "XMeshObj.h"
#include "GFrameWork.h"
#include "GTimer.h"
#include "GNode.h"
#include "GCamera.h"
#include "GSceneMgr.h"
#include "XMultiThread.h"

extern DWORD WINAPI LoadObj(LPVOID pParam);
class GGame:public GFrameWork
{
public:
	typedef GFrameWork Super;
	FiDeclareSingleton( GGame );
public:
    GGame(void);
    ~GGame(void);

	virtual bool initBase(HWND mainWnd);

    virtual void Loop();

    void ShutDown();

	void Finish();
public:

	GSceneMgr* GetSceneMgr( ) { return mSceneMgr;}

private:

    void GetInput();

    void Update();

    void Render( );

public:

	CXMultiThread mMTLoadObj;

	GSceneMgr *mSceneMgr;

	GMeshBaseObj *mpSelectObj;

	GAnimMeshObj *mpSelectAnim;

private:

	void GameRender(float fPass);

	void RenderEye(float fPass);

	bool	mFinished;
};

#define TheGame GGame::GetSingletonPtr() 
#define TheSceneMgr GGame::GetSingleton().GetSceneMgr()
