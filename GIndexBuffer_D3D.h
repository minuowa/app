#include "GIndexBuffer.h"
//-------------------------------------------------------------------------
/**
	@brief
	@author nnboy,29/9/2014  14:20
*/
class GIndexBuffer_D3D: public GIndexBuffer
{
public:
    GIndexBuffer_D3D ( void );
    ~GIndexBuffer_D3D ( void );

    virtual void set() override;

    virtual bool create ( const GIndexBufferParam& param ) override;

    virtual void copyToGPU() override;

protected:
    IDirect3DIndexBuffer9* mIB;
};

