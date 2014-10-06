#ifndef GVertexBuffer_h__
#define GVertexBuffer_h__
#include "GDeviceResource.h"
//-------------------------------------------------------------------------
/**
	@brief 
	@author nnboy,29/9/2014  14:53
*/
struct GVertexBufferParam
{
	DWORD mFVF;
	WORD mElementSizeOfByte;
	size_t mElementCount;
	bool mManaged;

	DWORD lengthOfBytes()
	{
		return mElementCount * mElementSizeOfByte;
	}
	GVertexBufferParam()
	{
		dMemoryZero ( this, sizeof ( *this ) );
	}
};
class GVertexBuffer: public GDeviceResource
{
	friend class GDevice;
protected:
	GVertexBuffer();
public:
	virtual void set();
	virtual bool create ( const GVertexBufferParam& param );
	virtual void copyToGPU();
	inline CXBuffer& getBuffer();
	inline DWORD getRenderVertexCount() const;
	inline void	setRenderVertexCount ( WORD cnt );
protected:
	GVertexBufferParam mParam;
	WORD mRenderCount;
	CXBuffer mBuffer;
};

inline CXBuffer& GVertexBuffer::getBuffer()
{
	return mBuffer;
}
inline DWORD GVertexBuffer::getRenderVertexCount() const
{
	return mRenderCount;
}
inline void GVertexBuffer::setRenderVertexCount ( WORD cnt )
{
	mRenderCount = cnt;
}

#endif // GVertexBuffer_h__
