#include "GGameDemoHeader.h"
#include "GComponentInterface.h"
#include "GComponentTrans.h"
#include "GFactory.h"
#include "GComponentFactory.h"
#include "GNode.h"


GComponentInterface::GComponentInterface ( void )
    : mComponentType ( eComponentType_Count )
    , mCanDetach ( true )
    , mTarget ( 0 )
{
}


GComponentInterface::~GComponentInterface ( void )
{
}

void GComponentInterface::SetTarget ( GNode* target )
{
    mTarget = target;
}

GNode* GComponentInterface::GetTarget() const
{
    return mTarget;
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




GComponentInterface* GComponentOwner::AttachComponent ( eComponentType type )
{
    assert ( type < eComponentType_Count );
    if ( !GetComponent ( type ) )
    {
        GComponentFactory::ComponentCreator* creator =
            CXSingleton<GComponentFactory>::GetSingleton().GetCreator ( type );
        CXASSERT_RESULT_FALSE ( creator );
        mCompoents[type] = creator->mCreator();
    }
    return mCompoents[type];
}

GComponentInterface* GComponentOwner::AttachComponent ( const char* name )
{
    GComponentInterface* component = GetComponent ( name );
    if ( component )
    {
        return component;
    }
    GComponentFactory::ComponentCreator* creator =
        CXSingleton<GComponentFactory>::GetSingleton().GetCreator ( name );
    CXASSERT_RESULT_FALSE ( creator );
    CXASSERT_RESULT_FALSE ( !mCompoents[creator->mType] );
    mCompoents[creator->mType] = creator->mCreator();
    return mCompoents[creator->mType];
}


void GComponentOwner::DetachComponent ( const char* name )
{
    for ( int i = 0; i < eComponentType_Count; ++i )
    {
        if ( mCompoents[i] )
        {
            if ( !strcmp ( mCompoents[i]->CategoryName(), name ) )
            {
                if ( mCompoents[i]->CanDetach() )
                {
                    CXSafeDelete ( mCompoents[i] );
                }
            }
        }
    }
}


