#include "GGameDemoHeader.h"
#include "GCamera.h"
//#include "GXNode.h"
#include "GTimer.h"
//CEye gEye;

GCamera::GCamera ( void )
{
    mCanGetInput = true;
    mpTransMan = NULL;

    mbTraceMan = false;

    mfLenTransMan = LEN_TRACE_MAN;

    mpEyeCliper = NULL;

    matProj = NORMALMATRIX;

    matView = NORMALMATRIX;
}

GCamera::~GCamera ( void )
{

}

void GCamera::SetProj()
{
    float aspect = ( ( float ) ( D9DEVICE->mWidth ) ) / ( ( float ) ( D9DEVICE->mHeight ) );

    D3DXMatrixPerspectiveFovLH ( &matProj, D3DX_PI / 4.0f, aspect, 1.0f, Max_Eye_Distance );

    if ( D9DEVICE != NULL )
    {
        D9DEVICE->GetDvc()->SetTransform ( D3DTS_PROJECTION, &matProj );
    }

    mpEyeCliper = new CEyeCliper ( Max_Eye_Distance, D3DX_PI / 4.0f, aspect );

}


bool GCamera::Create()
{
    if ( !__super::Create() )
        return false;

    return true;
}

void GCamera::SetView()
{
    D3DXVECTOR3 vLookAtPos = mXPos.mTranslate + mXPos.mvDir * 100;

    D3DXMatrixLookAtLH ( &matView, &mXPos.mTranslate, &vLookAtPos, &mXPos.mvUpon );

    if ( D9DEVICE != NULL )
    {
        D9DEVICE->GetDvc()->SetTransform ( D3DTS_VIEW, &matView );
    }
}

void GCamera::GetInput ( DWORD frameTimeMs )
{
    if ( INPUTSYSTEM.IsPressKey ( DIK_ADD ) )
        mXPos.mfSpeedMove += 0.03f * frameTimeMs;

    if ( INPUTSYSTEM.IsPressKey ( DIK_SUBTRACT ) )
        mXPos.mfSpeedMove -= 0.03f * frameTimeMs;

    POINT pt = INPUTSYSTEM.GetMousePoint();

    if ( pt.x < 0 || pt.x > D9DEVICE->mWidth || pt.y < 0 || pt.y > D9DEVICE->mHeight )
    {
        return;			//鼠标在客户区外面就不执行GetInput
    }

    D3DXVECTOR3 vMove = INPUTSYSTEM.GetMouseMoveEX();

    if ( INPUTSYSTEM.IsPressKey ( DIK_A ) || INPUTSYSTEM.IsPressKey ( DIK_D ) || INPUTSYSTEM.IsPressKey ( DIK_S ) || INPUTSYSTEM.IsPressKey ( DIK_W ) )
    {
        mbTraceMan = true;
    }
    else
    {
        mbTraceMan = false;
    }

    TraceMan();

    if ( mbTraceMan )
    {
        float fDelta = vMove.z / 120 * 10.0f;

        if ( mfLenTransMan - fDelta > 5.0f )
        {
            mfLenTransMan -= fDelta;
        }

    }

    mXPos.MoveStep ( vMove.z / 120 * 5.0f );

    if ( INPUTSYSTEM.IsPressingButton ( btRB ) )
    {
        mXPos.TrunStepLeftRightWithUp ( -vMove.x / 800.0f );

        mXPos.TrunWithRight ( -vMove.y / 800.0f );
    }
}


void GCamera::TraceMan(  )
{
    if ( mpTransMan == NULL || !mbTraceMan )
    {
        return;
    }

    if ( !mpTransMan->mXPos.mbCanMoveStep )
    {
        return;
    }

    D3DXVECTOR3 Pos = ZEROVECTOR3;

    Pos = mpTransMan->mXPos.mTranslate - mpTransMan->mXPos.mvDir * mfLenTransMan * 2;

    Pos += mpTransMan->mXPos.mvUpon * mfLenTransMan;

    D3DXVECTOR3 vDist = Pos - mXPos.mTranslate;


    D3DXVECTOR3 Dir = mpTransMan->mXPos.mTranslate - Pos;

    mXPos.mTranslate = Pos;

    mXPos.mvDir = Dir;

    D3DXVec3Cross ( &mXPos.mvRight, & ( D3DXVECTOR3 ( 0, 1, 0 ) ), &mXPos.mvDir );

    D3DXVec3Cross ( &mXPos.mvUpon, &mXPos.mvDir, &mXPos.mvRight );

    D3DXVec3Normalize ( &mXPos.mvDir, &mXPos.mvDir );
    D3DXVec3Normalize ( &mXPos.mvUpon, &mXPos.mvUpon );
    D3DXVec3Normalize ( &mXPos.mvRight, &mXPos.mvRight );

}


bool GCamera::InitTrack ( GNode *pWorldObj )
{
    if ( pWorldObj != NULL )
    {
        mpTransMan = pWorldObj;

        return true;
    }
    else
    {
        return false;
    }
}

void GCamera::Update()
{
    __super::Update();
    if ( mpEyeCliper )
        mpEyeCliper->Update ( & ( GetWorldMatrix ( false ) ) );
}



