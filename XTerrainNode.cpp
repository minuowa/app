#include "GGameDemoHeader.h"
#include "XTerrainNode.h"
#include "XTerrain.h"


CXTerrainNode::CXTerrainNode( CXTerrain* pMaster, XI32 nLevel, CXTerrainNode* Parent /*= 0*/, eTerrainNodePose Pose /*= eTNP_ROOT*/ )
    : mTerrain( pMaster )
    , mLevel( nLevel )
    , mPose( Pose )
    , mParent( Parent )
{
    if ( mPose == eTNP_ROOT )
    {
        mTerraninNodeType = eTNT_ROOT;
    }
    else if ( mLevel == G_TERRAIN_MIN_LEVEL )
    {
        mTerraninNodeType = eTNT_CHILD;
    }
    else
    {
        mTerraninNodeType = eTNT_PARENT;
    }

    BuildIndices();
}


CXTerrainNode::~CXTerrainNode( void )
{
}

void CXTerrainNode::BuildIndices()
{
    static XI32 INDEX_WIDTH = mTerrain->GetCellWidth();
    static XI32 LINE_COUNT = mTerrain->GetLineWidth();
    static XI32 HALF_WIDTH = INDEX_WIDTH / 2;

    XI32 IDX[4] = {0,};
    XI32 IDX_SPACE = 0, IDX_HALF_SPACE = 0, IDX_CENTER = 0, IDX_CENTER_SPACE = 0;

    if ( mParent )
    {
        IDX[eTNP_LEFT_BOTTOM]	= mParent->mIndices[eTNP_LEFT_BOTTOM];
        IDX[eTNP_RIGHT_BOTTOM]	= mParent->mIndices[eTNP_RIGHT_BOTTOM];
        IDX[eTNP_RIGHT_TOP]		= mParent->mIndices[eTNP_RIGHT_TOP];
        IDX[eTNP_LEFT_TOP]		= mParent->mIndices[eTNP_LEFT_TOP];

        IDX_SPACE = IDX[eTNP_RIGHT_BOTTOM] - IDX[eTNP_LEFT_BOTTOM];
        IDX_HALF_SPACE = IDX_SPACE / 2;
        IDX_CENTER = IDX[eTNP_LEFT_BOTTOM] + IDX_HALF_SPACE * INDEX_WIDTH + IDX_HALF_SPACE;
        IDX_CENTER_SPACE = INDEX_WIDTH * IDX_HALF_SPACE;
    }

    switch ( mPose )
    {
        case eTNP_LEFT_BOTTOM:
            mIndices[eTNP_LEFT_BOTTOM]	= IDX[eTNP_LEFT_BOTTOM];
            mIndices[eTNP_RIGHT_BOTTOM] = IDX[eTNP_LEFT_BOTTOM] + IDX_HALF_SPACE;
            mIndices[eTNP_RIGHT_TOP]	= IDX_CENTER;
            mIndices[eTNP_LEFT_TOP]		= IDX_CENTER - IDX_HALF_SPACE;
            mIndices[eTNP_ROOT]			= IDX_CENTER -  IDX_CENTER_SPACE - IDX_HALF_SPACE;
            break;

        case eTNP_RIGHT_BOTTOM:
            mIndices[eTNP_LEFT_BOTTOM]	= IDX[eTNP_RIGHT_BOTTOM] - IDX_HALF_SPACE;
            mIndices[eTNP_RIGHT_BOTTOM] = IDX[eTNP_RIGHT_BOTTOM];
            mIndices[eTNP_RIGHT_TOP]	= IDX_CENTER + IDX_HALF_SPACE;
            mIndices[eTNP_LEFT_TOP]		= IDX_CENTER;
            mIndices[eTNP_ROOT]			= IDX_CENTER - IDX_CENTER_SPACE - IDX_HALF_SPACE;
            break;

        case eTNP_RIGHT_TOP:
            mIndices[eTNP_LEFT_BOTTOM]	= IDX_CENTER;
            mIndices[eTNP_RIGHT_BOTTOM] = IDX_CENTER + IDX_HALF_SPACE;
            mIndices[eTNP_RIGHT_TOP]	= IDX[eTNP_RIGHT_TOP];
            mIndices[eTNP_LEFT_TOP]		= IDX[eTNP_RIGHT_TOP] - IDX_HALF_SPACE;
            mIndices[eTNP_ROOT]			= IDX_CENTER + IDX_CENTER_SPACE + IDX_HALF_SPACE;
            break;

        case eTNP_LEFT_TOP:
            mIndices[eTNP_LEFT_BOTTOM]	= IDX_CENTER - IDX_HALF_SPACE;
            mIndices[eTNP_RIGHT_BOTTOM] = IDX_CENTER;
            mIndices[eTNP_RIGHT_TOP]	= IDX[eTNP_LEFT_TOP] + IDX_HALF_SPACE;
            mIndices[eTNP_LEFT_TOP]		= IDX[eTNP_LEFT_TOP] ;
            mIndices[eTNP_ROOT]			= IDX_CENTER + IDX_CENTER_SPACE - IDX_HALF_SPACE;
            break;

        case eTNP_ROOT:
            mIndices[eTNP_LEFT_BOTTOM] = 0;
            mIndices[eTNP_RIGHT_BOTTOM] = INDEX_WIDTH;
            mIndices[eTNP_RIGHT_TOP] = INDEX_WIDTH * INDEX_WIDTH;
            mIndices[eTNP_LEFT_TOP] = INDEX_WIDTH * ( INDEX_WIDTH - 1 );
            mIndices[eTNP_ROOT] = INDEX_WIDTH * HALF_WIDTH;
            break;

        default:
            break;
    }

    if ( mLevel > G_TERRAIN_MIN_LEVEL )
    {
        for ( XI32 i = 0; i < eTNP_ROOT; ++i )
            mChildren[i] = new CXTerrainNode( mTerrain, mLevel - 1, this, ( eTerrainNodePose )i );
    }
}
