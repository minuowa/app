#include "GGameDemoHeader.h"
#include "GSceneMgr.h"
#include "GGameMap.h"
#include "GGame.h"
#include "GComponentTrans.h"
#include "GComponentMesh.h"
#include "GComponentFactory.h"
#include "GComponentBox.h"

GSceneMgr::GSceneMgr ( void )
    : mSceneDynamiRootNode ( 0 )
    , mSceneRootNode ( 0 )
    , mSceneStaticRootNode ( 0 )
    , mCurCamera ( 0 )
{

}

GSceneMgr::~GSceneMgr ( void )
{

}

bool GSceneMgr::Init( const GDevice_D3D& DVC )
{
    InitNodeFactory();
    InitComponentFactory();

    mSceneRootNode = new GNode();
    mSceneRootNode->SetNodeName ( "Scene Root" );

    //EditorEvent event;
    //event.mType = eSceneToEditor_Add;
    //event.mArgs.push_back ( mSceneRootNode->GetObjectName() );
    //event.mArgs.push_back ( "" );
    //Notify ( event );

    mSceneStaticRootNode = new GNode();
    mSceneDynamiRootNode = new GNode();
    mSceneStaticRootNode->SetNodeName ( "Scene Static Root" );
    mSceneDynamiRootNode->SetNodeName ( "Scene Dyna Root" );
    mSceneRootNode->AddChild ( mSceneStaticRootNode );
    mSceneRootNode->AddChild ( mSceneDynamiRootNode );

    mCurCamera = new GCamera;
    mCurCamera->SetNodeName ( "Current Camera" );
    if ( mCurCamera->Create()  )
    {
        //GComponentTrans* trans = get
        mCurCamera->GetTrans().mvDir = D3DXVECTOR3 ( ZEROFLOAT, -200.0f, 200.0f );
        mCurCamera->GetTrans().mvRight = D3DXVECTOR3 ( 1.0f, ZEROFLOAT, ZEROFLOAT );
        D3DXVec3Cross ( &mCurCamera->GetTrans().mvUpon, &mCurCamera->GetTrans().mvDir, &mCurCamera->GetTrans().mvRight );

        D3DXVec3Normalize ( &mCurCamera->GetTrans().mvDir, &mCurCamera->GetTrans().mvDir );
        D3DXVec3Normalize ( &mCurCamera->GetTrans().mvRight, &mCurCamera->GetTrans().mvRight );
        D3DXVec3Normalize ( &mCurCamera->GetTrans().mvUpon, &mCurCamera->GetTrans().mvUpon );

        mCurCamera->GetTrans().mfSpeedMove = 150.0f;

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
    //mCurCamera->Update();
	if (mSceneRootNode)
	{
		mSceneRootNode->Update();
	}
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
    CXRapidxmlNode* root = doc.allocate_node ( rapidxml::node_element );
    root->name ( "GameObects" );
    doc.append_node ( root );
    mSceneRootNode->LinkTo ( root );
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
    //if ( TheEditor )
    //{
    //    const EditorEventArr& eventArr = TheEditor->GetEventArr();
    //    if ( eventArr.size() )
    //    {
    //        EditorEventArr::const_iterator it ( eventArr.begin() );
    //        EditorEventArr::const_iterator iend ( eventArr.end() );
    //        for ( ; it != iend; ++it )
    //        {
    //            const EditorEvent& event = *it;
    //            HandleEvent ( event );
    //        }
    //        TheEditor->ClearEvent();
    //    }
    //}
}


GNode* GSceneMgr::GetNodeByName ( const char* name )
{
    if ( mSceneRootNode )
        return mSceneRootNode->GetNodeByName ( name );
    return 0;
}

#define __RegisterGameObjCreator(className) \
	mGameObjFactory.RegisterCreator(#className,className::CreateNode)

void GSceneMgr::InitNodeFactory()
{
    __RegisterGameObjCreator ( GNode );
    __RegisterGameObjCreator ( GAnimMeshObj );
    __RegisterGameObjCreator ( GMeshBaseObj );
    __RegisterGameObjCreator ( GRenderObject );

    //if ( TheEditor )
    //{
    //    typedef GFactory<GNode>::ObjCreatorMap GNodeCreatorMap;
    //    const GNodeCreatorMap& nodeCreatorMap = mGameObjFactory.GetCreators();
    //    GNodeCreatorMap::const_iterator walk = nodeCreatorMap.begin();
    //    GNodeCreatorMap::const_iterator end = nodeCreatorMap.end();
    //    CharStringArr gameobjectTypeArr;
    //    for ( ; walk != end; ++walk )
    //    {
    //        gameobjectTypeArr.push_back ( walk->first.c_str() );
    //    }
    //    TheEditor->InitObjectMenu ( gameobjectTypeArr );
    //}
}


void GSceneMgr::InitComponentFactory()
{
    __RegisterComponentCreator ( GComponentTrans );
    __RegisterComponentCreator ( GComponentMesh );
    __RegisterComponentCreator ( GComponentBox );

    //if ( TheEditor )
    //{
    //    typedef GComponentFactory::ComponentCreatorMap ComponentCreatorMap;
    //    const ComponentCreatorMap& nodeCreatorMap =
    //        CXSingleton<GComponentFactory>::GetSingleton().GetCreators();
    //    ComponentCreatorMap::const_iterator walk = nodeCreatorMap.begin();
    //    ComponentCreatorMap::const_iterator end = nodeCreatorMap.end();
    //    CharStringArr componentTypeArr;
    //    for ( ; walk != end; ++walk )
    //    {
    //        componentTypeArr.push_back ( walk->first.c_str() );
    //    }
    //    TheEditor->InitComponentMenu ( componentTypeArr );
    //}
}

void GSceneMgr::EditorSetObjectProperty ( GNode* node )
{
    //if ( node && TheEditor )
    //{
    //    if ( !node->IsRegisterAll() )
    //        node->RegisterAll();

    //    EPropertySheet* propSheet = TheEditor->GetPropertySheet();
    //    CXASSERT_RETURN ( propSheet );

    //    const CategoryPropertyMap& cateMap = node->GetPropertyMap();

    //    propSheet->ClearPropertyies();

    //    CategoryPropertyMap::const_iterator walk = cateMap.begin();
    //    CategoryPropertyMap::const_iterator end = cateMap.end();
    //    for ( ; walk != end; ++walk )
    //    {
    //        PropertyMap* propMap = walk->second;
    //        PropertyMap::iterator propIt = propMap->begin();
    //        PropertyMap::iterator propEnd = propMap->end();
    //        for ( ; propIt != propEnd; ++propIt )
    //        {
    //            propSheet->AddProperty ( walk->first.c_str(), propIt->first.c_str(), propIt->second );
    //        }
    //    }
    //}
}

void GSceneMgr::EditorUpdatePopupMenu ( GNode* node )
{
    //if ( TheEditor && node )
    //{
    //    TheEditor->ResetComponentMenuState();
    //    GComponentOwner& owner = node->GetComponentOwner();
    //    for ( int i = 0; i < eComponentType_Count; ++i )
    //    {
    //        GComponentInterface* component = owner.GetComponent ( ( eComponentType ) i );
    //        if ( component )
    //        {
    //            TheEditor->SetComponentMenuState ( component->GetComponentName(), true, component->CanDetach()  );
    //        }
    //    }
    //}
}

//bool GSceneMgr::OnNotify ( const EditorEvent& event )
//{
//    if ( !__super::OnNotify ( event ) )
//        return false;
//    switch ( event.mType )
//    {
//    case eEditorToScene_Add:
//    {
//        String typeName = event.mArgs[0];
//        GNode* node = mGameObjFactory.Create ( typeName.c_str() );
//        mSceneRootNode->AddChild ( node );
//    }
//    break;
//    case eEditorToSecne_Select:
//    {
//        String objName = event.mArgs[0];
//        GNode* node = GetNodeByName ( objName );
//        EditorSetObjectProperty ( node );
//        EditorUpdatePopupMenu ( node );
//    }
//    break;
//    case eEditorToScene_PropertyChange:
//    {
//        String objName = event.mArgs[0];
//        GNode* node = GetNodeByName ( objName );
//        if ( node )
//            node->OnPropertyChange ( event.mExtraData0, event.mExtraData1 );
//    }
//    break;
//    case eEditorToScene_ComponentAttach:
//    {
//        String objName = event.mArgs[0];
//        String componenttype = event.mArgs[1];
//        GNode* node = GetNodeByName ( objName );
//        if ( node )
//        {
//            node->AttachComponent ( componenttype );
//        }
//    }
//    break;
//    case eEditorToScene_ComponentDettach:
//    {
//        String objName = event.mArgs[0];
//        String componenttype = event.mArgs[1];
//        GNode* node = GetNodeByName ( objName );
//        if ( node )
//        {
//            node->DetachComponent ( componenttype );
//        }
//    }
//    break;
//    }
//    return true;
//}

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
