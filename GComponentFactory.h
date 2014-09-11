#pragma once
#include "GComponentInterface.h"

class GComponentFactory
{
public:
    typedef GComponentInterface* ( *ObjCrator ) ();

    struct ComponentCreator
    {
        int mType;
        ObjCrator mCreator;
    };

    typedef CXMap<String, ComponentCreator*> ComponentCreatorMap;
	~GComponentFactory();
public:
    void RegisterCreator ( const char* typeName, int type, ObjCrator creator );
    GComponentInterface* Create ( const char* typeName );
    ComponentCreator* GetCreator ( const char* typeName ) const;
    ComponentCreator* GetCreator ( eComponentType type ) const;

private:
    ComponentCreatorMap mComponentCreatorMap;
public:
    const ComponentCreatorMap& GetCreators() const
    {
        return mComponentCreatorMap;
    }
};

#define __RegisterComponentCreator(className) \
	CXSingleton<GComponentFactory>::GetSingleton().RegisterCreator(#className,className::ComponentType,className::CreateComponent)
