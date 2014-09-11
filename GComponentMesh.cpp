#include "GGameDemoHeader.h"
#include "GComponentMesh.h"
#include "GNode.h"


GComponentMesh::GComponentMesh ( void )
{
}


GComponentMesh::~GComponentMesh ( void )
{
}

void GComponentMesh::RegisterAll()
{
    __RegisterProperty ( mMeshFile );
}

void GComponentMesh::OnPropertyChange ( void* pre, void* changed )
{
    if ( pre == &mMeshFile )
    {
        String changedName;
        CXCast ( changedName, changed );
        if ( changedName != mMeshFile && mTarget )
        {
            mMeshFile = changedName;
            bool res = mTarget->ReCreate();
            CXASSERT ( res );
        }
    }
}
