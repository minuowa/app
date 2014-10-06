#pragma once
class GDevice;
class GDeviceResource
{
public:
	GDeviceResource(void);
	virtual ~GDeviceResource(void);
	virtual void set()=0;
	virtual void safeRelease();
protected:
	GDevice* mDevice;
};
class GWorldTransform:public GDeviceResource
{
public:
	virtual void set();
	D3DMATRIX mMatrix;
};
