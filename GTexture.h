#ifndef GTexture_h__
#define GTexture_h__
#include "GDeviceResource.h"
#include "GResourceManager.h"
//-------------------------------------------------------------------------
/**
	@brief
	@author nnboy,30/9/2014  17:09
*/
class GTexture: public GDeviceResource
{
public:
    GTexture();
    virtual bool createFromFile ( const char* fileName );

    virtual void set();

    inline void setStage ( int stage );
    inline int getStage() const;
public:
    String mFileName;
    int mStage;
};
inline void GTexture::setStage ( int stage )
{
    mStage = stage;
}
inline int GTexture::getStage() const
{
    return mStage;
}


typedef CXSingleton<GResourceManager<GTexture>> GTextureManager;

#define TextureMgr	GTextureManager::GetSingleton()

#endif // GTexture_h__
