#include "GGameDemoHeader.h"
#include "GTextureBuffer.h"
#include "GD9Device.h"

CXImpleteSingleton(GTextureBuffer);
GTextureBuffer::GTextureBuffer(void)
{
}

GTextureBuffer::~GTextureBuffer(void)
{
}

GTexture* GTextureBuffer::GetTexture( const char* fileName )
{
	GTexture* texture=0;
	if (!mTextureMap.Get(fileName,texture))
	{
		IDirect3DTexture9* d9texture=LoadFormFile(fileName);
		CXASSERT_RETURN_FALSE(d9texture);
		texture=new GTexture;
		texture->FileName=fileName;
		mTextureMap.Insert(fileName,texture);
	}
	return texture;
}

IDirect3DTexture9* GTextureBuffer::LoadFormFile( const char* fileName )
{
	IDirect3DTexture9* texture=0;
		CXASSERT_RESULT_FALSE(
			D3DXCreateTextureFromFileA( D9DEVICE->GetDvc(), fileName, &texture )
			);
	return texture;
}

GTexture::GTexture()
	:mD3DTexture(0)
{

}

bool GTexture::Create()
{
	mD3DTexture = TextureMgr.LoadFormFile(FileName);
	return mD3DTexture!=0;
}
