#pragma once
#include "GMeshBaseObj.h"

class CXMeshObj :
    public GMeshBaseObj
{
public:
    CXMeshObj(void);
    ~CXMeshObj(void);

	bool Create();
};

class CGameObj:
    public CXMeshObj
{
public:
    CGameObj(){};
    ~CGameObj(){};

};
