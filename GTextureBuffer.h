#pragma once


typedef CXMap<String,GTexture*> GTextureMap;
class GTextureBuffer
{
	CXDeclareSingleton(GTextureBuffer);
    public:
        GTextureBuffer( void );
        ~GTextureBuffer( void );
    public:
		GTextureMap		mTextureMap;
};