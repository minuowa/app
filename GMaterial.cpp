#include "GGameDemoHeader.h"
#include "GMaterial.h"
#include "GDeviceResource.h"
#include "GDevice.h"


GMaterial::GMaterial(void)
{
}


GMaterial::~GMaterial(void)
{
}

D3DMATERIAL9 GMaterial::mDefaultRedMatrial=
{
	{1.0f,0.0f,0.0f,0.5f,},
	{1.0f,0.0f,0.0f,0.5f,},
	{1.0f,0.0f,0.0f,0.5f,},
	{1.0f,0.0f,0.0f,0.5f,},
	32,
};

D3DMATERIAL9 GMaterial::mDefaultGrayMatrial=
{
	{0.5,0.5,0.5,0.5f,},
	{0.5,0.5,0.5,0.5f,},
	{0.5,0.5,0.5,0.5f,},
	{0.5,0.5,0.5,0.5f,},
	32,
};
D3DMATERIAL9 GMaterial::mDefaultWhiteMatrial=
{
	{1.0f,1.0f,1.0f,1.0f,},
	{1.0f,1.0f,1.0f,1.0f,},
	{1.0f,1.0f,1.0f,1.0f,},
	{1.0f,1.0f,1.0f,1.0f,},
	32,
};

void GMaterial::set()
{
	mDevice->setMaterial(&mData);
}
