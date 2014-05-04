#pragma once
#include "EObject.h"
#include "GFactory.h"
enum eComponentType
{
    eComponentType_Trans,
    eComponentType_Mesh,
    eComponentType_Ani,
    eComponentType_Effect,
    eComponentType_Script,
    eComponentType_Particles,
    eComponentType_Count,
};
class GComponentInterface :
    public MObject
{
public:
    GComponentInterface ( void );
    virtual ~GComponentInterface ( void );
protected:
    eComponentType	mComponentType;
public:
    eComponentType GetType();
    const char* GetComponentName();
};
template<eComponentType TYPE>
class GComponentBase: public GComponentInterface
{
public:
    GComponentBase()
    {
        mComponentType = TYPE;
    }
};

class GComponentOwner
{
public:
    GComponentOwner();
    GComponentInterface* GetComponent ( const char* name ) const;
    GComponentInterface* GetComponent ( eComponentType type ) const;
	GComponentInterface* AttachComponent ( const char* name );
    void DetachComponent ( const char* name );
    GComponentInterface* AttachComponent ( eComponentType type );
protected:

    GComponentInterface* mCompoents[eComponentType_Count];
};

typedef CXSingleton<GFactory<GComponentInterface>> GComponentFactory;

#define DeclareComponentCreator(className) \
	 public:\
	 static GComponentInterface* CreateComponent() \
{\
	return new className;\
}

#define DeclareComponentType(className)\
	DeclareCategoryName(className)\
	DeclareComponentCreator(className)

