#include "GGameDemoHeader.h"
#include "GTexture_D3D.h"
#include "GDevice_D3D.h"


GTexture_D3D::GTexture_D3D(void)
{
}


GTexture_D3D::~GTexture_D3D(void)
{
}

bool GTexture_D3D::createFromFile( const char* fileName )
{
	CXASSERT ( mDevice->getType() == GDeviceType_DX9 );
	GDevice_D3D* d3dDVC = static_cast<GDevice_D3D*> ( mDevice );
	CXASSERT ( d3dDVC );
	CXASSERT_RESULT_FALSE(
		D3DXCreateTextureFromFileA( d3dDVC->getDVC(), fileName, &mD3DTexture )
		);
	return mD3DTexture!=nullptr;
}

void GTexture_D3D::set()
{
	CXASSERT ( mDevice->getType() == GDeviceType_DX9 );
	GDevice_D3D* d3dDVC = static_cast<GDevice_D3D*> ( mDevice );
	CXASSERT ( d3dDVC );
	d3dDVC->setTexture(mStage,mD3DTexture);
}

void GTexture_D3D::safeRelease()
{
	SAFERELEASE(mD3DTexture);
}
