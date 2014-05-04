#include "GGameDemoHeader.h"
#include "GComponentInterface.h"
#include "GComponentTrans.h"


GComponentInterface::GComponentInterface ( void )
    : mComponentType ( eComponentType_Count )
{
}


GComponentInterface::~GComponentInterface ( void )
{
}

eComponentType GComponentInterface::GetType()
{
    return mComponentType;
}

const char* GComponentInterface::GetComponentName()
{
    return CategoryName();
}

GComponentOwner::GComponentOwner()
{
    CXMemoryZeroArray ( mCompoents );
}

GComponentInterface* GComponentOwner::GetComponent ( const char* name ) const
{
    for ( int i = 0; i < eComponentType_Count; ++i )
    {
        if ( mCompoents[i] )
        {
            if ( !strcmp ( mCompoents[i]->CategoryName(), name ) )
            {
                return mCompoents[i];
            }
        }
    }
    return 0;
}

GComponentInterface* GComponentOwner::GetComponent ( eComponentType type ) const
{
    assert ( type < eComponentType_Count );
    return mCompoents[type];
}



GComponentInterface* GComponentOwner::AttachComponent ( const char* name )
{
    assert ( GetComponent ( name ) == 0 );
    for ( int i = 0; i < eComponentType_Count; ++i )
    {
        if ( mCompoents[i] )
        {
            if ( !strcmp ( mCompoents[i]->CategoryName(), name ) )
            {
                switch ( i )
                {
                case eComponentType_Trans:
                    mCompoents[i] = new GComponentTrans;
                    break;
                }
				if (mCompoents[i])
					return mCompoents[i];
            }
        }
    }
    return 0;
}

void GComponentOwner::DetachComponent ( const char* name )
{
    assert ( GetComponent ( name ) != 0 );
    for ( int i = 0; i < eComponentType_Count; ++i )
    {
        if ( mCompoents[i] )
        {
            if ( !strcmp ( mCompoents[i]->CategoryName(), name ) )
            {
                CXSafeDelete ( mCompoents[i] );
            }
        }
    }
}
