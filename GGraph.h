#pragma once
#include "GVertexBuffer.h"
#include "GIndexBuffer.h"

class GMaterial;
class GTexture;
class GVertexBuffer;
class GIndexBuffer;

class GGraph
{
public:
    GGraph ( void );
    ~GGraph ( void );
    void setTexture ( const char* fileName );
    void setMetiral ( const D3DMATERIAL9& d9matrial );
    bool createVertexBuffer ( const GVertexBufferParam& param );
    bool createIndexBuffer ( const GIndexBufferParam& param );
    void copyVertexBufferToGPU();
    void copyIndexBufferToGPU();
    char* getVertexBufferPointer() const;
    char* getIndexBufferPointer() const;
    void computeNormal();
    void draw();
private:
    GTexture*			mTexture;
    GMaterial*			mMat;
    GVertexBuffer*		mVB;
    GIndexBuffer*		mIB;
};
