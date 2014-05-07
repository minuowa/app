#pragma once
#include "GObject.h"
#include "GFactory.h"
enum eComponentType
{
    eComponentType_Trans,
    eComponentType_Mesh,
    eComponentType_Ani,
    eComponentType_Effect,
    eComponentType_Script,
	eComponentType_Box,
    eComponentType_Particles,
    eComponentType_Count,
};
class GComponentInterface :
    public GObject
{
public:
    GComponentInterface ( void );
    virtual ~GComponentInterface ( void );
protected:
    eComponentType	mComponentType;
    bool	mCanDetach;
public:
	inline eComponentType GetType()
	{
		return mComponentType;
	}
    inline const char* GetComponentName()
	{
		return CategoryName();
	}
    inline bool CanDetach() const
    {
        return mCanDetach;
    }
    inline void SetCanDetach ( bool can )
    {
        mCanDetach = can;
    }
};
template<eComponentType TYPE>
class GComponentBase: public GComponentInterface
{
public:
	enum{ComponentType=TYPE,};
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

	GComponentInterface* AttachComponent ( eComponentType type);
	GComponentInterface* AttachComponent ( const char* name);

	void DetachComponent ( const char* name );
	void DetachComponent ( eComponentType type );
protected:

    GComponentInterface* mCompoents[eComponentType_Count];
};



#define DeclareComponentCreator(className) \
	 public:\
	 static GComponentInterface* CreateComponent() \
{\
	return new className;\
}

#define DeclareComponentType(className)\
	DeclareCategoryName(className)\
	DeclareComponentCreator(className)

