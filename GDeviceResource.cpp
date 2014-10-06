#include "GGameDemoHeader.h"
#include "GDeviceResource.h"
#include "GDevice.h"


GDeviceResource::GDeviceResource(void)
{
	mDevice=GDevice::mInstance;
}


GDeviceResource::~GDeviceResource(void)
{
}

void GDeviceResource::safeRelease()
{

}

void GWorldTransform::set()
{
	mDevice->setMatrix(D3DTS_WORLD,&mMatrix);
}
