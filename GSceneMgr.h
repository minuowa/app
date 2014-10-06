#pragma once

#include "GGameMap.h"
#include "GameStaticObj.h"

#include "GNode.h"
#include "GMeshBuffer.h"
#include "GCamera.h"
#include "GDevice_D3D.h"
#include "XMeshObj.h"
#include "GFactory.h"

#define SCENENUM 5		//��������	


enum eGameScene
{
    gsNull,

    gsLoading,

    gsGame,

    gslogin,			//��¼
};

class CSceneMachine
{
public:

    CSceneMachine();

    ~CSceneMachine();
public:

    eGameScene GetNowScene();

    void ChangeToScene ( eGameScene gs );

public:

    eGameScene mgsNowScene;

    eGameScene mgsContainer[SCENENUM];

};

class GSceneMgr
{
public:
    GSceneMgr ( void );
    ~GSceneMgr ( void );
public:

    void CullObj();
	bool Init ( const GDevice_D3D& DVC );
    void* GetInput ( float fPass );
    void SetView();
    void SetProj();
    void Update ( float fPass );
    void Delete ( CGameStaticObj *pObj );
    bool SaveScene ( CChar* xmlFile );
    void AddStaticObj ( GNode* node );
    void AddDynaObj ( GNode* node );
    void ProcessEvent();
    void SelectObjByName ( const char* name );
    GNode* GetNodeByName ( const char* name );
public:
	void EditorSetObjectProperty(GNode* node);
	void EditorUpdatePopupMenu(GNode* node);
    void InitNodeFactory();
    void InitComponentFactory();
	//virtual bool OnNotify(const EditorEvent& event);

    int mRenderObjNum;

    GCamera* mCurCamera;
    CSceneMachine mSceneMachine;

    GNode* mSceneRootNode;
    GNode* mSceneStaticRootNode;
    GNode* mSceneDynamiRootNode;

    GFactory<GNode> mGameObjFactory;

	CXDelegate mDelegateAddObj;
};
