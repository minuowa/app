#include "GGameDemoHeader.h"
#include "GMeshData.h"
#include "GD9Device.h"
#include "GTextureBuffer.h"


void GMetrialData::Render()
{
	D9DEVICE->GetDvc()->SetTexture(0,mTexture->GetTexture());
	D9DEVICE->GetDvc()->SetMaterial(&mMat);
}

void GMetrialData::SetTexture( const char* fileName )
{
	mTexture = TextureMgr.GetTexture(fileName);
	CXASSERT(mTexture);
}

void GMetrialData::SetMetiral( const D3DMATERIAL9& d9matrial )
{
	CXMemoryCopy(&mMat,(void*)&d9matrial,sizeof(d9matrial));
}

GMetrialData::GMetrialData( void )
	:mTexture(0)
{
	CXMemoryZero(&mMat,sizeof(mMat));
}

void GGraphVertexBuffer::Set()
{
	D9DEVICE->GetDvc()->SetStreamSource(0,mD9VertexBuffer,0,mDataSize);
	D9DEVICE->GetDvc()->SetFVF(mFVF);
}

void GGraphIndexBuffer::Set()
{
	D9DEVICE->GetDvc()->SetIndices(mD9IndexBuffer);
}
