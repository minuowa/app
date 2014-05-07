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
    D3DXVECTOR3 vLookAtPos = GetTrans().mTranslate + GetTrans().mvDir * 100;

    D3DXMatrixLookAtLH ( &matView, &GetTrans().mTranslate, &vLookAtPos, &GetTrans().mvUpon );

    if ( D9DEVICE != NULL )
    {
        D9DEVICE->GetDvc()->SetTransform ( D3DTS_VIEW, &matView );
    }
}

void GCamera::GetInput ( DWORD frameTimeMs )
{
    if ( INPUTSYSTEM.IsPressKey ( DIK_ADD ) )
        GetTrans().mfSpeedMove += 0.03f * frameTimeMs;

    if ( INPUTSYSTEM.IsPressKey ( DIK_SUBTRACT ) )
        GetTrans().mfSpeedMove -= 0.03f * frameTimeMs;

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

    GetTrans().MoveStep ( vMove.z / 120 * 5.0f );

    if ( INPUTSYSTEM.IsPressingButton ( btRB ) )
    {
        GetTrans().TrunStepLeftRightWithUp ( -vMove.x / 800.0f );

        GetTrans().TrunWithRight ( -vMove.y / 800.0f );
    }
}


void GCamera::TraceMan(  )
{
    if ( mpTransMan == NULL || !mbTraceMan )
    {
        return;
    }

    if ( !mpTransMan->GetTrans().mbCanMoveStep )
    {
        return;
    }

    D3DXVECTOR3 Pos = ZEROVECTOR3;

    Pos = mpTransMan->GetTrans().mTranslate - mpTransMan->GetTrans().mvDir * mfLenTransMan * 2;

    Pos += mpTransMan->GetTrans().mvUpon * mfLenTransMan;

    D3DXVECTOR3 vDist = Pos - GetTrans().mTranslate;


    D3DXVECTOR3 Dir = mpTransMan->GetTrans().mTranslate - Pos;

    GetTrans().mTranslate = Pos;

    GetTrans().mvDir = Dir;

    D3DXVec3Cross ( &GetTrans().mvRight, & ( D3DXVECTOR3 ( 0, 1, 0 ) ), &GetTrans().mvDir );

    D3DXVec3Cross ( &GetTrans().mvUpon, &GetTrans().mvDir, &GetTrans().mvRight );

    D3DXVec3Normalize ( &GetTrans().mvDir, &GetTrans().mvDir );
    D3DXVec3Normalize ( &GetTrans().mvUpon, &GetTrans().mvUpon );
    D3DXVec3Normalize ( &GetTrans().mvRight, &GetTrans().mvRight );

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



