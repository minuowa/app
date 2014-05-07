#pragma once
#include "GComponentInterface.h"
class GComponentBox: public GComponentBase<eComponentType_Box>
{
	DeclareComponentType(GComponentBox);
public:
    GComponentBox ( void );
    ~GComponentBox ( void );

    virtual void RegisterAll();

protected:
    int mXDim;
    int mYDim;
    int mZDim;
};

