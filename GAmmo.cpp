#include "GGameDemoHeader.h"
#include "GAmmo.h"

GAmmo::GAmmo ( void )
{
    GetTrans().mfSpeedMove = 150;

}

GAmmo::~GAmmo ( void )
{

}

void GAmmo::Shoot()
{
    m_bForceOnMap = true;
    GetTrans().mbAutoMove = true;
    GetTrans().mvSpeed = GetTrans().mfSpeedMove * GetTrans().mvDir;
}


bool GAmmo::Create()
{
    if ( !__super::Create() )
        return false;

    return true;
}

void GAmmo::SetShooter ( GNode *pShooter )
{

    GetTrans().mTranslate = pShooter->GetTrans().mTranslate;

    GetTrans().mTranslate.y += 50.0f;
    GetTrans().mTranslate.z += 5.0f;

    GetTrans().mvDir = pShooter->GetTrans().mvDir;
    GetTrans().mvDir.y += 0.01f;

    D3DXVec3Normalize ( &GetTrans().mvDir, &GetTrans().mvDir );

    GetTrans().mvRight = pShooter->GetTrans().mvRight;

    D3DXVec3Cross ( &GetTrans().mvUpon, &GetTrans().mvDir, &GetTrans().mvRight );

    D3DXVec3Normalize ( &GetTrans().mvUpon, &GetTrans().mvUpon );

    mParent = NULL;
}


bool CAmmoParticles::Create()
{
    if ( !GAmmo::Create() )
        return false;

    mParticles.Create();

    //ForceOnMap(pForceMap,1.0f,ftUpWithMap);

    return true;

}

void CAmmoParticles::Update(  )

{
    m_bForceOnMap = false;
    GRenderObject::Update();

    mParticles.Update();
}

bool CAmmoParticles::Render()
{
    GAmmo::Render();

    return mParticles.Render();
}

CAmmoParticles::CAmmoParticles()
{

}

CAmmoParticles::~CAmmoParticles()
{

}
