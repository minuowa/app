#include "GGameDemoHeader.h"
#include "GIndexBuffer_D3D.h"
#include "GDevice_D3D.h"


GIndexBuffer_D3D::GIndexBuffer_D3D ( void )
{
    mIB = nullptr;
}


GIndexBuffer_D3D::~GIndexBuffer_D3D ( void )
{
}

void GIndexBuffer_D3D::set()
{
    CXASSERT ( mDevice->getType() == GDeviceType_DX9 );
    GDevice_D3D* d3dDVC = static_cast<GDevice_D3D*> ( mDevice );
    CXASSERT ( d3dDVC );
    d3dDVC->getDVC()->SetIndices ( mIB );
}

bool GIndexBuffer_D3D::create( const GIndexBufferParam& param )
{
	mParam=param;

	CXASSERT ( mDevice->getType() == GDeviceType_DX9 );
	GDevice_D3D* d3dDVC = static_cast<GDevice_D3D*> ( mDevice );
	CXASSERT ( d3dDVC );

	HRESULT hr=d3dDVC->getDVC()->CreateIndexBuffer ( mParam.length(), 0, mParam.mIndex32?D3DFMT_INDEX32:D3DFMT_INDEX16,
		 mParam.mManaged?D3DPOOL_MANAGED:D3DPOOL_DEFAULT, &mIB, 0 );
	CXASSERT_RESULT_FALSE(hr);
	CXASSERT_RESULT_FALSE(hr);
	mBuffer.setElementSize(mParam.getElementSizeOfByte());
	mBuffer.reAllocate(mParam.mElementCount);
	return true;

}

void GIndexBuffer_D3D::copyToGPU()
{
	void* pIB;
	HRESULT hr=mIB->Lock ( 0, mBuffer.length(), ( void** ) &pIB, 0 );
	CXASSERT_RESULT(hr);
	mBuffer.copyTo(pIB);
	mIB->Unlock();
}
