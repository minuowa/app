#pragma once

#include "XList.h"
//#include "EasyList.h"
//纹理与Mesh是对应的
class GTexture
{
public:
	GTexture();
	bool Create();
	inline IDirect3DTexture9* GetTexture()
	{
		return mD3DTexture;
	}
public:
	CharString			FileName;
protected:
	IDirect3DTexture9*	mD3DTexture;
};
//class GTextureNode
//{
//    public:
//        GTextureNode() {};
//        ~GTextureNode() {};
//    public:
//
//        CXDynaArray<IDirect3DTexture9*> mFaceArr;
//
//        LPDIRECT3DTEXTURE9 *ppTexture;
//
//        int LnID;	    //对应的模型的ID
//        //
//        //int LnIndex;    //在模型中的索引
//
//        int LnCount;    //纹理图片的数量，与材质数量对应
//
//};
typedef CXMap<CharString,GTexture*> GTextureMap;
class GTextureBuffer
{
	CXDeclareSingleton(GTextureBuffer);
    public:
        GTextureBuffer( void );
        ~GTextureBuffer( void );
		GTexture* GetTexture(const char* fileName);
		IDirect3DTexture9* LoadFormFile(const char* fileName);
    public:
		GTextureMap		mTextureMap;
};
#define TextureMgr	GTextureBuffer::GetSingleton()