#include "GGameDemoHeader.h"
#include "XTerrain.h"
#include "XTerrainNode.h"
#include "GD9Device.h"


CXTerrain::CXTerrain(void)
	:mVertexBuffer(0)
	,mIndexBuffer(0)
{
}


CXTerrain::~CXTerrain(void)
{
}

bool CXTerrain::Init()
{
	mLevel = 8;
	mRoot = new CXTerrainNode(this,mLevel);
	return true;
}

bool CXTerrain::CreateIndexBuffer()
{
	IDirect3DDevice9 * DVC = D9DEVICE->GetDvc();

	CHECK_RETURN_BOOL(DVC);

	if (FAILED(DVC->CreateVertexBuffer(GetLineWidth()*GetLineWidth(),0,
		CXTerrainVertex::FVF,D3DPOOL_MANAGED,&mVertexBuffer,0))
		)
	{
		assert(0);
		return false;
	}
	return true;
}

bool CXTerrain::CreateVertexBuffer()
{
	IDirect3DDevice9 * DVC = D9DEVICE->GetDvc();

	CHECK_RETURN_BOOL(DVC);

	if (FAILED(DVC->CreateIndexBuffer(GetCellWidth()*GetCellWidth()*6,0,
		D3DFMT_INDEX32,D3DPOOL_MANAGED,&mIndexBuffer,0))
		)
	{
		assert(0);
		return false;
	}
	return true;
}
