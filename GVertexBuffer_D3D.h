#ifndef GVertexBuffer_D3D_h__
#define GVertexBuffer_D3D_h__
//-------------------------------------------------------------------------
/**
	@brief
	@author nnboy,29/9/2014  14:17
*/
class GVertexBuffer_D3D: public GVertexBuffer
{
public:
    GVertexBuffer_D3D ( void );
    ~GVertexBuffer_D3D ( void );

    virtual void set() override;

    virtual bool create ( const GVertexBufferParam& param ) override;

    virtual void copyToGPU() override;

protected:
    IDirect3DVertexBuffer9* mVB;
};

#endif // GVertexBuffer_D3D_h__
