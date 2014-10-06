#pragma once
#include "GDeviceResource.h"
class GMaterial:public GDeviceResource
{
public:
	GMaterial(void);
	~GMaterial(void);

	virtual void set();
public:
	D3DMATERIAL9 mData;

	static D3DMATERIAL9 mDefaultRedMatrial;
	static D3DMATERIAL9 mDefaultGrayMatrial;
	static D3DMATERIAL9 mDefaultWhiteMatrial;
};

