#ifndef GDevice_h__
#define GDevice_h__
#include "GType.h"
//-------------------------------------------------------------------------
/**
	@brief 
	@author nnboy,17/9/2014  22:43
*/
class GVertexBuffer;
class GIndexBuffer;

class GDevice
{
public:
	GDevice(void);
	~GDevice(void);

	virtual GDeviceType getType();

	virtual bool beginRender();
	virtual void endRender();

	virtual void setMaterial(D3DMATERIAL9* mat);
	virtual void setMatrix(eGMatrixType type,D3DMATRIX* mat);
	virtual void setTexture();

	virtual GVertexBuffer* generateVertexBuffer();
	virtual GIndexBuffer* generateIndexBuffer();

	virtual bool loadTexture(const char* fileName);
	virtual bool loadEffect(const char* fileName);
	virtual bool loadMesh(const char* fileName);
	virtual bool loadAnimation(const char* fileName);

	virtual void drawPrimate();
public:
	CXDelegate mOnLostDevice;
	CXDelegate mOnResetDevice;
public:
	static GDevice* mInstance;
};
#define  Device	GDevice::mInstance

#endif // GDevice_h__

