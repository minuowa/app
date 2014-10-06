#include "GGameDemoHeader.h"
#include "GGame.h"
#include "XString.h"
#include <locale.h>
#include "XEvent.h"
#include "GDevice.h"

/******************************************************************/
//�����MeshBuffer�У�����͵�ͼ����
//��ա����桢��ͼ��������׶�ü�
/******************************************************************/
FiImplateSingleton ( GGame );

GGame::GGame ( void )
    : mSceneMgr ( NULL )
    , mpSelectObj ( NULL )
    , mpSelectAnim ( NULL )
    , mFinished ( false )
{
}

GGame::~GGame ( void )
{

}

void GGame::Loop()
{
    TIMER.Update();

    if ( mIsActive )
    {
        INPUTSYSTEM.Update();
        GetInput();
    }

    Update();

    Render();
}

void GGame::GetInput()
{
    float fPass = TIMER.GetFrameTimeSec();


    if ( INPUTSYSTEM.IskeyUp ( DIK_ESCAPE ) )
    {
        exit ( 0 );
    }

    static DWORD dwGetInput = 0;

    eGameScene gs = mSceneMgr->mSceneMachine.GetNowScene();

    switch ( gs )
    {
    case gsLoading:

        break;
    case gslogin:
        break;

    case  gsGame:

        if ( INPUTSYSTEM.GetKeyAction ( DIK_F9 ) == DI_BUTTONUP )
        {
        }

        //if (INPUTSYSTEM.GetKeyAction(DIK_TAB)==DI_BUTTONUP)
        //{
        //	dwGetInput++;
        //	if (0==dwGetInput%2)
        //	{
        //		mpSelectAnim=&gAnimMesh[0];
        //	}
        //	else
        //	{
        //		mpSelectAnim=&gAnimMesh[1];
        //	}
        //}

        if ( mpSelectAnim != NULL )
        {
            mpSelectAnim->GetInput ( fPass );
        }

        GMeshBaseObj *pMeshBaseObj = NULL;

        POINT pt = INPUTSYSTEM.GetMousePoint();

        bool bHit = false;

        static bool bFlag = true;

        //pMeshBaseObj = ( CXMeshBaseObj* )( GMeshManager::GetSingleton().GetInput( fPass ) );

        if ( pMeshBaseObj != NULL )
        {
            mpSelectObj = pMeshBaseObj;
            //        if ( mEditor )
            //        {
            //CXASSERT(0);
            //            //mEditor->SetObject ( pMeshBaseObj );
            //        }
        }

        if ( mpSelectObj != NULL )
        {
            if ( mpSelectAnim != NULL )
            {
                if ( !mpSelectAnim->GetTrans().mbCanMoveStep )
                {
                    mpSelectObj->GetInput ( fPass );
                }
                else
                {
                    mpSelectObj = NULL;
                }
            }

        }

        if ( mpSelectObj != NULL )
        {
            if ( INPUTSYSTEM.IsPressKey ( DIK_COMMA ) )
            {
                //>�����ٶ�
                mpSelectObj->GetTrans().mfSpeedMove += 0.8f;
            }

            if ( INPUTSYSTEM.IsPressKey ( DIK_PERIOD ) )
            {
                //<��С�ٶ�
                mpSelectObj->GetTrans().mfSpeedMove -= 0.8f;
            }

            if ( DI_BUTTONUP == INPUTSYSTEM.GetKeyAction ( DIK_DELETE ) )
            {
                mSceneMgr->Delete ( ( CGameStaticObj* ) mpSelectObj );

            }

        }


        //���Ŵ󣬡���С
        if ( INPUTSYSTEM.IsPressKey ( DIK_UPARROW ) )
        {
            if ( mpSelectObj != NULL )
            {
                mpSelectObj->GetTrans().mvZoom += D3DXVECTOR3 ( 0.01f, 0.01f, 0.01f );
            }

            if ( mpSelectAnim != NULL )
            {
                mpSelectAnim->GetTrans().mfSpeedMove += 0.5f;
            }

        }

        if ( INPUTSYSTEM.IsPressKey ( DIK_DOWNARROW ) )
        {
            if ( mpSelectObj != NULL )
            {
                mpSelectObj->GetTrans().mvZoom -= D3DXVECTOR3 ( 0.01f, 0.01f, 0.01f );
            }

            if ( mpSelectAnim != NULL )
            {
                mpSelectAnim->GetTrans().mfSpeedMove -= 0.5f;
            }

        }

        if ( INPUTSYSTEM.GetKeyAction ( DIK_F2 ) == DI_BUTTONUP )
        {

            if ( bFlag )
            {
                Device->GetDvc()->SetRenderState ( D3DRS_FILLMODE, D3DFILL_SOLID );
            }
            else
            {
                Device->GetDvc()->SetRenderState ( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
            }

            Toggle ( bFlag );

            //OPENCLOSE( mpNowMap->mLighting );
        }

        if ( INPUTSYSTEM.GetKeyAction ( DIK_F3 ) == DI_BUTTONUP )
        {
            //gAnimMesh[0].mXPos.mvPos = D3DXVECTOR3( 0, 0, 0 );
            //gAnimMesh[0].UpdateForForceOnMap();
        }

        mSceneMgr->GetInput ( fPass );
        break;
    }

}

void GGame::Update( )
{

    if ( INPUTSYSTEM.IskeyUp ( DIK_TAB ) )
    {
        mSceneMgr->SaveScene ( "gameSceneEditor.xml" );
        //DWORD FillMode = 0;
        //D9DEVICE->GetDvc()->GetRenderState( D3DRS_FILLMODE, &FillMode );
        //D9DEVICE->GetDvc()->SetRenderState( D3DRS_FILLMODE, FillMode == D3DFILL_WIREFRAME ? D3DFILL_SOLID : D3DFILL_WIREFRAME );
    }


    eGameScene gs = mSceneMgr->mSceneMachine.GetNowScene();

    float fPass = TIMER.GetFrameTimeSec();

    switch ( gs )
    {

    case gsLoading:

        break;

    case gsGame:
        mSceneMgr->Update ( fPass );
        //gAnimMesh[0].Update();
        //mSea.Update( fPass );

        break;
    }


}

void GGame::Render( )
{
    float fPass = TIMER.GetFrameTimeSec();

    eGameScene gs = mSceneMgr->mSceneMachine.GetNowScene();

    mSceneMgr->SetView();
    switch ( Device->TestDevice() )
    {
    case D3D_OK:
    case D3DERR_DEVICELOST:
        break;
    case D3DERR_DEVICENOTRESET:
    {
        Device->OnDeviceLost();
    }
    break;
    default:
    {
        CXASSERT ( 0 );
    }
    break;
    }
	if (Device->beginRender())
	{
		switch ( gs )
		{

		case gsLoading:

			//mpUIMgr->RenderLoading( fPass );

			break;

		case  gsGame:

			GameRender ( fPass );

			break;

		case gslogin:

			//mpUIMgr->RenderLogin(fPass);

			break;
		}

		Device->endRender();
	}

}

bool GGame::initBase ( HWND mainWnd )
{
    //setlocale( LC_ALL, "zh_CN.UTF-8" );

    //��ʼ�����500ms
    HRESULT hr = NULL;

    bool bSuccess = false;

    int nSuccess = ERR_INT;



    CoInitialize ( NULL );
    //��ʼ�����
    bSuccess = gCursor.Init();
    gCursor.SetNowCursor ( curNormal );

    //��ʼ�����
    CXASSERT_RETURN_FALSE ( __super::initBase ( mainWnd ) );
    //if ( mEditor )
    //{
    //    mEditor->SetWndProc ( ( void* ) WndProc );
    //}
    //��ʼ��D3D�豸
    //CXASSERT_RETURN_FALSE (
    //    GSingletonD9Device::GetSingletonPtr()->Init ( mMainWin )
    //);
	GDevice_D3D* pd3dDevice=new GDevice_D3D;
	CXASSERT_RETURN_FALSE(pd3dDevice->Init(mainWnd));
	GDevice::mInstance=pd3dDevice;
	CXASSERT_RETURN_FALSE()
    CXASSERT_RETURN_FALSE (
        GSingletonD8Input::GetSingleton().Init ( GSingletonD9Device::GetSingleton(),
                mInst, mMainWin )
    );

    //��ʼ������������
    mSceneMgr = new GSceneMgr;

    bSuccess = mSceneMgr->Init ( GSingletonD9Device::GetSingleton() );

    if ( !bSuccess )
        return false;

    GMeshManager::GetSingleton().Init();

    Device->ResetRenderState();

    //bSuccess = gEffect.Create ( "..\\Data\\Effect\\SimpleEffect.fx" );

    //gEffect.m_hWorldViewProj = gEffect.mD3DEffect->GetParameterByName ( 0, "matWorldViewProj" );

    //gEffect.m_hWorld = gEffect.mD3DEffect->GetParameterByName ( 0, "matWorld" );

    //gEffect.m_Tech = gEffect.mD3DEffect->GetTechniqueByName ( "TShader" );

    //gEffect.m_hUseMaterialOnly = gEffect.mD3DEffect->GetParameterByName ( 0, "bUseMaterialOnly" );

    //gEffect.m_hTexture = gEffect.mD3DEffect->GetParameterByName ( 0, "TexObj" );

    //gEffect.m_hMtrlAmbient = gEffect.mD3DEffect->GetParameterByName ( 0, "materialAmbient" );

    //gEffect.m_hMtrlDiffuse = gEffect.mD3DEffect->GetParameterByName ( 0, "materialDiffuse" );

    //gEffect.m_hOpenLight = gEffect.mD3DEffect->GetParameterByName ( 0, "bOpenLight" );


    //������Ⱦ������3000ms

    CoUninitialize();


    gEvent.Create ( "Load" );

    //gEvent.SetUsed();

    //mMTLoadObj.Init(&LoadObj,(LPVOID)this,true);
    LoadObj ( this );

    mSceneMgr->mSceneMachine.ChangeToScene ( gsGame );


    return true;
}

void GGame::ShutDown()
{

}

DWORD WINAPI LoadObj ( LPVOID pParam )
{
    CoInitialize ( NULL );

    GGame *pGame = ( GGame* ) pParam;

    if ( pGame == NULL )
    {
        return ERR_INT;
    }

    //����ͶӰ����
    TheSceneMgr->SetProj();

    //������������ϵ
    GWorldCorrd* corrd = new GWorldCorrd();
    corrd->SetNodeName ( "World Corrd" );
    CXASSERT_RETURN_FALSE ( corrd->Create() );
    TheSceneMgr->AddStaticObj ( corrd );

//   MeshPara seaPara( 0, 80.0f, 0, 64, "..\\Data\\res\\water\\BlueShort\\A21C_000.jpg", NULL );
//   pGame->mSea.CreateParam(seaPara);
    //pGame->mSea.Create( NULL, ( void* )&seaPara );
//   pGame->mSea.mLighting = true;
//   pGame->mSea.m_bCanSelect = false;
//   pGame->mSea.AddQuakePoint( 0, 0, 10.0f, 0.5f );

   GAnimMeshObj *pAnimMesh = new GAnimMeshObj;
   pAnimMesh->setMeshFile("..\\Data\\res\\Anim\\AnimMesh0002\\A0002.x");
   CXASSERT_RETURN_FALSE(pAnimMesh->ReCreate());
   TheSceneMgr->AddDynaObj ( pAnimMesh );
   //TheSceneMgr->mEye.InitTrack( &gAnimMesh[0] );

    //gEvent.WaitForUse(-1);

    TheSceneMgr->mSceneMachine.ChangeToScene ( gsGame );

    return TRUE_INT;
}




void GGame::GameRender ( float fPass )
{

    //GMeshManager::GetSingleton().Render( fPass );

    mSceneMgr->mSceneRootNode->Draw();

    //gAnimMesh[0].Render( fPass );

    RenderEye ( fPass );

    //mpUIMgr->Render( fPass );
}

void GGame::RenderEye ( float fPass )
{
    /**************************************************************************************

    ����Ļ����ʾ�۾���Ϣ

    **************************************************************************************/
    //RECT rcEye = {5, 100, 150, 150};

    //char strEye[128];

    //ZeroMemory( strEye, sizeof( strEye ) );

    ////sprintf(strEye,"�۾�λ��--X��%.2f,Y:%.2f,Z:%.2f",mpSceneMgr->mText.mEye.mXPos.mvPos.x,mpSceneMgr->mText.mEye.mXPos.mvPos.y,mpSceneMgr->mText.mEye.mXPos.mvPos.z);

    //sprintf( strEye, "������ٶȣ�%.2f", mSceneMgr->mEye.mXPos.mfSpeedMove );

    //mpUIMgr->mText.SetCurrentFontSize( fs12 );

    //mpUIMgr->mText.DrawTextInRect( strEye, &rcEye, 0xffff0000, otUI, tpLeft );



    //ZeroMemory( strEye, sizeof( strEye ) );

    //if ( mpSelectAnim != NULL )
    //{
    //    sprintf( strEye, "��ţλ��--X��%.2f,Y:%.2f,Z:%.2f���ٶȣ�%.2f", mpSelectAnim->mXPos.mvPos.x, mpSelectAnim->mXPos.mvPos.y, mpSelectAnim->mXPos.mvPos.z, mpSelectAnim->mXPos.mfSpeedMove );

    //    mpUIMgr->mText.SetCurrentFontSize( fs12 );

    //    rcEye.top += 50;
    //    rcEye.bottom += 50;
    //    mpUIMgr->mText.DrawTextInRect( strEye, &rcEye, 0xffff0000, otUI, tpLeft );
    //}


    //if ( mpSelectObj != NULL )
    //{
    //    ZeroMemory( strEye, sizeof( strEye ) );
    //    sprintf( strEye, "��ǰ����ID��%d", ( ( CGameStaticObj* )( mpSelectObj ) )->m_nObjID );
    //    mpUIMgr->mText.SetCurrentFontSize( fs12 );

    //    rcEye.top += 50;
    //    rcEye.bottom += 50;
    //    mpUIMgr->mText.DrawTextInRect( strEye, &rcEye, 0xffff0000, otUI, tpLeft );

    //    ZeroMemory( strEye, sizeof( strEye ) );
    //    sprintf( strEye, "��ǰģ��ID��%d", ( ( CGameStaticObj* )( mpSelectObj ) )->LnMeshID );
    //    mpUIMgr->mText.SetCurrentFontSize( fs12 );

    //    rcEye.top += 50;
    //    rcEye.bottom += 50;
    //    mpUIMgr->mText.DrawTextInRect( strEye, &rcEye, 0xffff0000, otUI, tpLeft );
    //}

    //ZeroMemory( strEye, sizeof( strEye ) );
    //sprintf( strEye, "��ǰ��Ⱦ������Ŀ��%d", GMeshBuffer::GetSingleton().mRenderObjNum + 1 );
    //mpUIMgr->mText.SetCurrentFontSize( fs12 );

    //rcEye.top += 50;
    //rcEye.bottom += 50;
    //mpUIMgr->mText.DrawTextInRect( strEye, &rcEye, 0xffff0000, otUI, tpLeft );

}

void GGame::Finish()
{
    mFinished = true;
}



