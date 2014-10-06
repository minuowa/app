#include "GGameDemoHeader.h"
#include "GGraph.h"
#include "GDevice_D3D.h"
#include "GTextureBuffer.h"
#include "GMaterial.h"
#include "GVertexBuffer.h"
#include "GIndexBuffer.h"
#include "GTexture.h"

void GGraph::draw()
{
    if ( mTexture )
        mTexture->set();
    if ( mMat )
        mMat->set();
    CXASSERT ( mVB );
    mVB->set();
	CXASSERT ( mIB );
	mIB->set();
}

void GGraph::setTexture ( const char* fileName )
{
    mTexture = TextureMgr.getResource ( fileName );
    CXASSERT ( mTexture );
}

void GGraph::setMetiral ( const D3DMATERIAL9& d9matrial )
{
    //CXMemoryCopy(&mMat,(void*)&d9matrial,sizeof(d9matrial));
}

GGraph::GGraph ( void )
    : mTexture ( 0 )
{
    dMemoryZero ( &mMat, sizeof ( mMat ) );
}

bool GGraph::createVertexBuffer ( const GVertexBufferParam& param )
{
    if ( mVB == nullptr )
    {
        mVB = Device->generateVertexBuffer();
    }
    return mVB->create ( param );
}

bool GGraph::createIndexBuffer ( const GIndexBufferParam& param )
{
    if ( mIB == nullptr )
    {
        mIB = Device->generateIndexBuffer();
    }
    return mIB->create ( param );
}

GGraph::~GGraph ( void )
{
    dSafeDelete ( mVB );
    dSafeDelete ( mIB );
}

void GGraph::copyVertexBufferToGPU()
{
	mVB->copyToGPU();
}

void GGraph::copyIndexBufferToGPU()
{
	mIB->copyToGPU();
}

char* GGraph::getIndexBufferPointer() const
{
	CXASSERT(mIB);
	return mIB->getBuffer().getPointer();
}

char* GGraph::getVertexBufferPointer() const
{
	CXASSERT(mVB);
	return mVB->getBuffer().getPointer();
}

void GGraph::computeNormal()
{

}








