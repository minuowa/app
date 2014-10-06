#pragma once
#include "XBase.h"
#include "XVector3.h"
#include "GNode.h"
class CXTerrainNode;
class GVertexBuffer;
class GIndexBuffer;
struct CXTerrainVertex
{
    CXVector3	Pos;
    XI32		Color;
    XF32		TU;
    XF32		TV;
    static const XI32 FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
};
class CXTerrain :
    public GNode
{
        CXDeclareSingleton( CXTerrain );
    public:
        CXTerrain( void );
        ~CXTerrain( void );

        bool init();
        XI32 getLineCount() const;
        XI32 getCellCountPerLine() const;
    private:
        bool createIndexBuffer();
        bool createVertexBuffer();

    private:
        int	mLevel;
        int mWidth;
        GVertexBuffer* mVertexBuffer;
        GIndexBuffer*	mIndexBuffer;
        CXTerrainNode* mRoot;
};
inline
XI32 CXTerrain::getCellCountPerLine() const
{
    return ::pow( 2.0f, mLevel );
}
inline
XI32 CXTerrain::getLineCount() const
{
    return getCellCountPerLine() + 1;
}

