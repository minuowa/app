#pragma once
#include "XBase.h"
class CXTerrainNode;

static const XI32 G_TERRAIN_CHILD_NUM = 4;
static const XI32 G_TERRAIN_MIN_LEVEL = 1;

enum eTerrainNodePose
{
    eTNP_LEFT_BOTTOM,
    eTNP_RIGHT_BOTTOM,
    eTNP_RIGHT_TOP,
    eTNP_LEFT_TOP,
    eTNP_ROOT,
};
enum eTerrainNodeType
{
    eTNT_PARENT		= BIT( 0 ),
    eTNT_CHILD		= BIT( 1 ),
    eTNT_ROOT		= BIT( 2 ) | eTNT_PARENT,
    eTNT_NONE,
};
typedef CXStaticArray<CXTerrainNode*, G_TERRAIN_CHILD_NUM> CXTerrainChildren;
typedef CXStaticArray<XI32, G_TERRAIN_CHILD_NUM> CXTerrainVertexIndex;

class CXTerrain;
class CXTerrainNode
{
    public:
        CXTerrainNode( CXTerrain* pMaster, XI32 nLevel, CXTerrainNode* Parent = 0, eTerrainNodePose Pose = eTNP_ROOT );
        ~CXTerrainNode( void );
    protected:
        void BuildIndices();
    protected:
        XI32					mLevel;
        eTerrainNodePose		mPose;
        CXTerrainNode*			mParent;
        CXTerrain*				mTerrain;
        CXTerrainChildren		mChildren;
        CXTerrainVertexIndex	mIndices;
        eTerrainNodeType		mTerraninNodeType;
};

