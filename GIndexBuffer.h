#ifndef GIndexBuffer_h__
#define GIndexBuffer_h__

#include "GDeviceResource.h"
struct GIndexBufferParam
{
	size_t mElementCount;
	bool mManaged;
	bool mIndex32;
	DWORD getElementSizeOfByte()
	{
		return mIndex32?sizeof(DWORD):sizeof(WORD);
	}
	DWORD length()
	{
		return getElementSizeOfByte()*mElementCount;
	}
	GIndexBufferParam()
	{
		dMemoryZero ( this, sizeof ( *this ) );
	}
};
class GIndexBuffer: public GDeviceResource
{
	GIndexBuffer(void);
	friend class GDevice;
public:
	~GIndexBuffer(void);

	virtual void set();
	virtual bool create ( const GIndexBufferParam& param );
	virtual void copyToGPU();
	inline CXBuffer& getBuffer();
public:
	GIndexBufferParam mParam;
	CXBuffer mBuffer;
};

inline CXBuffer& GIndexBuffer::getBuffer()
{
	return mBuffer;
}

#endif // GIndexBuffer_h__
