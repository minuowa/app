#include "GGameDemoHeader.h"
#include "GDevice.h"
#include "GIndexBuffer_D3D.h"
#include "GVertexBuffer_D3D.h"


GDevice::GDevice(void)
{
}


GDevice::~GDevice(void)
{
}

bool GDevice::beginRender()
{
	return true;
}

void GDevice::endRender()
{

}

void GDevice::setMaterial( D3DMATERIAL9* mat )
{
	assert(0);
}

GDeviceType GDevice::getType()
{
	CXASSERT(0);
	return GDeviceType_Max;
}

GVertexBuffer* GDevice::generateVertexBuffer()
{
	switch (getType())
	{
	case GDeviceType_DX9:
		{
			return new GVertexBuffer_D3D();
		}
	default:
		break;
	}
	return nullptr;
}

GIndexBuffer* GDevice::generateIndexBuffer()
{
	switch (getType())
	{
	case GDeviceType_DX9:
		{
			return new GIndexBuffer_D3D();
		}
	default:
		break;
	}
	return nullptr;
}

GDevice* GDevice::mInstance=nullptr;
