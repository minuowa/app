#pragma once
#include "XMap.h"
template<typename T>
class GResourceManager
{
public:
    typedef CXMap<String, T*> ResourceMap;
public:
    //GResourceManagerTemplete ( void );
    virtual ~GResourceManager ( void )
    {
        CXSafeDeleteMap2 ( mResourceMap );
    }
public:
    T* GetResource ( const char* name )
    {
        T* res = 0;
        if ( !mResourceMap.Get ( name, res ) )
        {
            res = new T();
            if ( !res->CreateFromFile ( name ) )
            {
                CXASSERT ( 0 );
                CXSafeDelete ( res );
            }
            else
            {
                mResourceMap.Insert ( name, res );
            }
        }
        return res;
    }
private:
    ResourceMap	mResourceMap;
};

