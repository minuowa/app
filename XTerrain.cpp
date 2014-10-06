#include "GGameDemoHeader.h"
#include "XTerrain.h"
#include "XTerrainNode.h"
#include "GDevice_D3D.h"
#include "GIndexBuffer.h"
#include "GVertexBuffer.h"


CXTerrain::CXTerrain ( void )
    : mVertexBuffer ( 0 )
    , mIndexBuffer ( 0 )
{
}


CXTerrain::~CXTerrain ( void )
{
}

bool CXTerrain::init()
{
    mLevel = 8;
    mRoot = new CXTerrainNode ( this, mLevel );
    return true;
}

bool CXTerrain::createIndexBuffer()
{
    mIndexBuffer = Device->generateIndexBuffer();

    GIndexBufferParam param;
    param.mElementCount = getCellCountPerLine() * getCellCountPerLine() * 6;
    param.mIndex32 = true;
    param.mManaged = true;

    CXASSERT_RESULT_FALSE ( mIndexBuffer->create ( param ) );

    return true;
}

bool CXTerrain::createVertexBuffer()
{
    mVertexBuffer = Device->generateVertexBuffer();

    GVertexBufferParam param;
    param.mElementCount = getLineCount() * getLineCount();
    param.mElementSizeOfByte = sizeof ( CXTerrainVertex );
    param.mFVF = CXTerrainVertex::FVF;
    param.mManaged = true;

    CXASSERT_RESULT_FALSE ( mVertexBuffer->create ( param ) );

    return true;


}
