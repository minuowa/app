#include "GGameDemoHeader.h"
#include "GVertexBuffer_D3D.h"
#include "GDevice_D3D.h"


GVertexBuffer_D3D::GVertexBuffer_D3D ( void )
{
    mVB = 0;
}


GVertexBuffer_D3D::~GVertexBuffer_D3D ( void )
{
}

void GVertexBuffer_D3D::set()
{
    CXASSERT ( mDevice->getType() == GDeviceType_DX9 );
    GDevice_D3D* d3dDVC = static_cast<GDevice_D3D*> ( mDevice );
    CXASSERT ( d3dDVC );
    d3dDVC->getDVC()->SetStreamSource ( 0, mVB, 0, mParam.mElementCount );
}


bool GVertexBuffer_D3D::create ( const GVertexBufferParam& param )
{
    mParam = param;
    CXASSERT ( mDevice->getType() == GDeviceType_DX9 );
    GDevice_D3D* d3dDVC = static_cast<GDevice_D3D*> ( mDevice );
    CXASSERT ( d3dDVC );
    HRESULT hr = d3dDVC->getDVC()->CreateVertexBuffer ( mParam.lengthOfBytes(), 0, mParam.mFVF,
                 mParam.mManaged ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/,
                 &mVB, 0 );
	CXASSERT_RESULT_FALSE(hr);
	mBuffer.setElementSize(mParam.mElementSizeOfByte);
	mBuffer.reAllocate(mParam.mElementCount);
    return true;
}

void GVertexBuffer_D3D::copyToGPU()
{
	void* pVBCoord;
	CXASSERT_RESULT(mVB->Lock ( 0, mBuffer.length(), &pVBCoord, 0 ));
	mBuffer.copyTo(pVBCoord);
	mVB->Unlock();
}
