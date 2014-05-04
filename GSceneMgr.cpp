#include "GGameDemoHeader.h"
#include "GSceneMgr.h"
#include "GGameMap.h"
#include "EditorSheetBase.h"
#include "GGame.h"
#include "EEditorSheetManager.h"
#include "GComponentTrans.h"


GSceneMgr::GSceneMgr ( void )
    : mSceneDynamiRootNode ( 0 )
    , mSceneRootNode ( 0 )
    , mSceneStaticRootNode ( 0 )
    , mCurCamera ( 0 )
{
    mSceneRootNode = new GNode();
    mSceneRootNode->SetNodeName ( "Scene Root" );

    EditorEvent event;
    event.mType = eSceneToEditor_Add;
    event.mArgs.push_back ( mSceneRootNode->GetEditName() );
    event.mArgs.push_back ( "" );
    SendEditorEvent ( event );

    mSceneStaticRootNode = new GNode();
    mSceneDynamiRootNode = new GNode();
    mSceneStaticRootNode->SetNodeName ( "Scene Static Root" );
    mSceneDynamiRootNode->SetNodeName ( "Scene Dyna Root" );
    mSceneRootNode->AddChild ( mSceneStaticRootNode );
    mSceneRootNode->AddChild ( mSceneDynamiRootNode );
}

GSceneMgr::~GSceneMgr ( void )
{

}

bool GSceneMgr::Init ( GD9Device *DVC )
{
    InitNodeFactory();
    InitComponentFactory();

    mCurCamera = new GCamera;
    mCurCamera->SetNodeName ( "Current Camera" );
    if ( mCurCamera->Create()  )
    {
        //GComponentTrans* trans = get
        mCurCamera->mXPos.mvDir = D3DXVECTOR3 ( ZEROFLOAT, -200.0f, 200.0f );
        mCurCamera->mXPos.mvRight = D3DXVECTOR3 ( 1.0f, ZEROFLOAT, ZEROFLOAT );
        D3DXVec3Cross ( &mCurCamera->mXPos.mvUpon, &mCurCamera->mXPos.mvDir, &mCurCamera->mXPos.mvRight );

        D3DXVec3Normalize ( &mCurCamera->mXPos.mvDir, &mCurCamera->mXPos.mvDir );
        D3DXVec3Normalize ( &mCurCamera->mXPos.mvRight, &mCurCamera->mXPos.mvRight );
        D3DXVec3Normalize ( &mCurCamera->mXPos.mvUpon, &mCurCamera->mXPos.mvUpon );

        mCurCamera->mXPos.mfSpeedMove = 150.0f;

        AddDynaObj ( mCurCamera );

        return true;
    }

    return false;
}

void* GSceneMgr::GetInput ( float fPass )
{
    mSceneDynamiRootNode->GetInput ( fPass );
    return 0;
}

void GSceneMgr::SetView()
{
    mCurCamera->SetView();
}

void GSceneMgr::SetProj()
{
    mCurCamera->SetProj();
}

void GSceneMgr::Update ( float fPass )
{
    ProcessEvent();

    mCurCamera->Update();

}

void GSceneMgr::Delete ( CGameStaticObj *pObj )
{
    char sConfigFile[FILE_NAME_LENGTH];
    ZeroMemory ( sConfigFile, sizeof ( sConfigFile ) );

    sprintf ( sConfigFile, "Data\\StaticObj\\Save\\StaticObj%03d.txt", pObj->m_nObjID );

    if ( IsFileExist ( sConfigFile ) )
    {
        DeleteFileA ( sConfigFile );
    }

}

bool GSceneMgr::SaveScene ( CChar* xmlFile )
{
    //version="1.0" encoding="UTF-8"
    CXRapidxmlDocument doc;
    doc.append_node (
        doc.allocate_node ( rapidxml::node_pi, doc.allocate_string ( "xml version=\"1.0\" encoding=\"UTF-8\"" ) )
    );
    mSceneRootNode->LinkTo ( &doc );
    CXRapidxmlWriter writer;
    writer.AppendChild ( &doc );
    writer.Write ( xmlFile );
    return true;
}

void GSceneMgr::AddStaticObj ( GNode* node )
{
    CXASSERT ( node );
    mSceneStaticRootNode->AddChild ( node );
}

void GSceneMgr::AddDynaObj ( GNode* node )
{
    CXASSERT ( node );
    mSceneDynamiRootNode->AddChild ( node );
}

void GSceneMgr::ProcessEvent()
{
    if ( TheEditor )
    {
        const EditorEventArr& eventArr = TheEditor->GetEventArr();
        if ( eventArr.size() )
        {
            EditorEventArr::const_iterator it ( eventArr.begin() );
            EditorEventArr::const_iterator iend ( eventArr.end() );
            for ( ; it != iend; ++it )
            {
                const EditorEvent& event = *it;
                HandleEvent ( event );
            }
            TheEditor->ClearEvent();
        }
    }
}

void GSceneMgr::HandleEvent ( const EditorEvent& event )
{
    switch ( event.mType )
    {
    case eEditorToScene_Add:
    {
        CharString typeName = event.mArgs[0];
        GNode* node = mGameObjFactory.Create ( typeName.c_str() );
        mSceneRootNode->AddChild ( node );
    }
    break;
    case eEditorToSecne_Select:
    {
        CharString objName = event.mArgs[0];
        GNode* node = GetNodeByName ( objName );
		EditorSelectObj(node);

    }
    break;
    case eEditorToScene_ComponentAttach:
    {
        CharString objName = event.mArgs[0];
        CharString componenttype = event.mArgs[1];
        GNode* node = GetNodeByName ( objName );
        if ( node )
        {
            node->AttachComponent ( componenttype );
        }
    }
    break;
    case eEditorToScene_ComponentDettach:
    {
        CharString objName = event.mArgs[0];
        CharString componenttype = event.mArgs[1];
        GNode* node = GetNodeByName ( objName );
        if ( node )
        {
            node->DetachComponent ( componenttype );
			EditorSelectObj(node);
        }
    }
    break;
    }
}

GNode* GSceneMgr::GetNodeByName ( const char* name )
{
    return mSceneRootNode->GetNodeByName ( name );
}

#define __RegisterGameObjCreator(className) \
	mGameObjFactory.RegisterCreator(#className,className::CreateNode)

void GSceneMgr::InitNodeFactory()
{
    __RegisterGameObjCreator ( GNode );
    __RegisterGameObjCreator ( GAnimMeshObj );
    __RegisterGameObjCreator ( GMeshBaseObj );
    __RegisterGameObjCreator ( GRenderObject );

    if ( TheEditor )
    {
        typedef GFactory<GNode>::ObjCreatorMap GNodeCreatorMap;
        const GNodeCreatorMap& nodeCreatorMap = mGameObjFactory.GetCreators();
        GNodeCreatorMap::const_iterator walk = nodeCreatorMap.begin();
        GNodeCreatorMap::const_iterator end = nodeCreatorMap.end();
		CharStringArr gameobjectTypeArr;
		for ( ; walk != end; ++walk )
		{
			gameobjectTypeArr.push_back ( walk->first.c_str() );
		}
		TheEditor->InitObjectMenu ( gameobjectTypeArr );
    }
}

#define __RegisterComponentCreator(className) \
	GComponentFactory::GetSingleton().RegisterCreator(#className,className::CreateComponent)

void GSceneMgr::InitComponentFactory()
{
    __RegisterComponentCreator ( GComponentTrans );

    if ( TheEditor )
    {
        typedef GFactory<GComponentInterface>::ObjCreatorMap GComponentCreatorMap;
        const GComponentCreatorMap& nodeCreatorMap = GComponentFactory::GetSingleton().GetCreators();
        GComponentCreatorMap::const_iterator walk = nodeCreatorMap.begin();
        GComponentCreatorMap::const_iterator end = nodeCreatorMap.end();
        CharStringArr componentTypeArr;
        for ( ; walk != end; ++walk )
        {
            componentTypeArr.push_back ( walk->first.c_str() );
        }
        TheEditor->InitComponentMenu ( componentTypeArr );
    }
}

void GSceneMgr::EditorSelectObj( GNode* node )
{
	if ( node && TheEditor )
	{
		if (!node->IsRegisterAll())
			node->RegisterAll();
		EPropertySheet* propSheet=TheEditor->GetObjectListSheet();
		const CategoryPropertyMap& cateMap=node->
		//const CategoryPropertyMap::const_iterator cateWalker=node.get
		//CategoryPropertyMap::const_iterator walk = propMap.begin();
		//CategoryPropertyMap::const_iterator end = propMap.end();
	}
}

CSceneMachine::CSceneMachine()
{
    mgsNowScene = gsNull;
    //ZeroMemory(&mgsContainer,sizeof(mgsContainer));
}

eGameScene CSceneMachine::GetNowScene()
{
    //return mgsContainer[0];
    return mgsNowScene;
}

void CSceneMachine::ChangeToScene ( eGameScene gs )
{
    if ( gs == mgsNowScene )
    {
        return;
    }

    mgsNowScene = gs;
}

CSceneMachine::~CSceneMachine()
{

}
