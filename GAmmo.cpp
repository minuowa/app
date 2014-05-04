#include "GGameDemoHeader.h"
#include "GAmmo.h"

GAmmo::GAmmo ( void )
{
    mXPos.mfSpeedMove = 150;

}

GAmmo::~GAmmo ( void )
{

}

void GAmmo::Shoot()
{
    m_bForceOnMap = true;
    mXPos.mbAutoMove = true;
    mXPos.mvSpeed = mXPos.mfSpeedMove * mXPos.mvDir;
}


bool GAmmo::Create()
{
    if ( !__super::Create() )
        return false;

    return true;
}

void GAmmo::SetShooter ( GNode *pShooter )
{

    mXPos.mTranslate = pShooter->mXPos.mTranslate;

    mXPos.mTranslate.y += 50.0f;
    mXPos.mTranslate.z += 5.0f;

    mXPos.mvDir = pShooter->mXPos.mvDir;
    mXPos.mvDir.y += 0.01f;

    D3DXVec3Normalize ( &mXPos.mvDir, &mXPos.mvDir );

    mXPos.mvRight = pShooter->mXPos.mvRight;

    D3DXVec3Cross ( &mXPos.mvUpon, &mXPos.mvDir, &mXPos.mvRight );

    D3DXVec3Normalize ( &mXPos.mvUpon, &mXPos.mvUpon );

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
