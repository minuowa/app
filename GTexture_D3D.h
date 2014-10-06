#ifndef GTexture_D3D_h__
#define GTexture_D3D_h__
#include "GTexture.h"
//-------------------------------------------------------------------------
/**
	@brief 
	@author nnboy,30/9/2014  17:25
*/
class GTexture_D3D :
	public GTexture
{
public:
	GTexture_D3D(void);
	~GTexture_D3D(void);

	virtual bool createFromFile( const char* fileName );

	virtual void set();

	virtual void safeRelease();

	IDirect3DTexture9*	mD3DTexture;
};

#endif // GTexture_D3D_h__
