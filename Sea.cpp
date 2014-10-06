#include "GGameDemoHeader.h"
#include "Sea.h"
#include "GMeshBuffer.h"
#include "GDevice_D3D.h"
#include "GTexture.h"
CSea::CSea( void )
	:mCreateParam(0)
{
    LnQuakeCount = 0;

    mpFace = NULL;

    mCellCount = 0;
    mfMaxHeight = 0;
    mfCellWidth = 0;
    mstrFileName = NULL;
	dSafeDelete(mCreateParam);
}

CSea::~CSea( void )
{
}

bool CSea::Create()
{
    if(!__super::Create())
		return false;

    HRESULT hr = S_FALSE;



    mMeshBufferNode = new GMeshBufferNode();
    mCellCount = mCreateParam->LnCellCount;
    mfMaxHeight = mCreateParam->mfMaxHeight;
    mfCellWidth = mCreateParam->mfCellWidth;
    mstrFileName = mCreateParam->mstrFileName;

    //SAFERELEASE(mMeshBufferNode->mpRootMesh);

	GGraph* renderData=new GGraph;
	mMeshResource.push_back(renderData);

	GVertexBufferParam vertexParam;
	vertexParam.mElementCount=( mCellCount + 1 ) * ( mCellCount + 1 );
	vertexParam.mElementSizeOfByte=sizeof(SeaVertex);
	vertexParam.mFVF=FVFSea;
	vertexParam.mManaged=false;

	CXASSERT_RETURN_FALSE(renderData->createVertexBuffer(vertexParam));

	GIndexBufferParam indexParam;
	indexParam.mElementCount= mCellCount*mCellCount*6;
	indexParam.mIndex32=true;
	indexParam.mManaged=true;

	CXASSERT_RETURN_FALSE(renderData->createIndexBuffer(indexParam));

	SeaVertex *pVertextBuffer=(SeaVertex*)renderData->getVertexBufferPointer();
	DWORD dwIndex=0;
    for ( int i = 0; i < mCellCount + 1; i++ )
    {
        for ( int j = 0; j < mCellCount + 1; j++ )
        {
            dwIndex = i * ( mCellCount + 1 ) + j;

            pVertextBuffer[dwIndex].vertex.x = ( j - mCellCount / 2.0f ) * mfCellWidth;
            pVertextBuffer[dwIndex].vertex.y = 0;
            pVertextBuffer[dwIndex].vertex.z = ( i - mCellCount / 2.0f ) * mfCellWidth;
            pVertextBuffer[dwIndex].u = j / 10.0f;
            pVertextBuffer[dwIndex].v = ( mCellCount - i ) / 10.0f;
        }
    }

	renderData->copyVertexBufferToGPU();

	DWORD *pIndexBuffer =(DWORD*)renderData->getVertexBufferPointer();

    DWORD dwBaseIndex = 0;

    for ( int i = 0; i < mCellCount; i++ )
    {
        for ( int j = 0; j < mCellCount; j++ )
        {
            pIndexBuffer[dwBaseIndex + 0] = i * ( mCellCount + 1 ) + j;
            pIndexBuffer[dwBaseIndex + 1] = ( i + 1 ) * ( mCellCount + 1 ) + j;
            pIndexBuffer[dwBaseIndex + 2] = ( i + 1 ) * ( mCellCount + 1 ) + j + 1;

            pIndexBuffer[dwBaseIndex + 3] = i * ( mCellCount + 1 ) + j;
            pIndexBuffer[dwBaseIndex + 4] = ( i + 1 ) * ( mCellCount + 1 ) + j + 1;;
            pIndexBuffer[dwBaseIndex + 5] = i * ( mCellCount + 1 ) + j + 1;

            dwBaseIndex += 6;
        }
    }

    renderData->copyIndexBufferToGPU();

	renderData->setTexture(mstrFileName);


    D3DMATERIAL9 mtrl;
    ZeroMemory( &mtrl, sizeof( mtrl ) );

    mtrl.Diffuse.r = 0.4f;
    mtrl.Diffuse.g = 0.4f;
    mtrl.Diffuse.b = 1.0f;
    mtrl.Diffuse.a = 0.6f;
    mtrl.Ambient.r = 0.2f;
    mtrl.Ambient.g = 0.2f;
    mtrl.Ambient.b = 0.2f;
    mtrl.Ambient.a = 0.5f;
    mtrl.Specular.r = 0.8f;
    mtrl.Specular.g = 0.8f;
    mtrl.Specular.b = 0.8f;
    mtrl.Specular.a = 0.5f;
    mtrl.Emissive.r = 0.2f;
    mtrl.Emissive.g = 0.2f;
    mtrl.Emissive.b = 0.6f;
    mtrl.Emissive.a = 0.5f;

    mtrl.Power = 9.0f;
	renderData->setMetiral(mtrl);

	renderData->computeNormal();

    return true;

}

int CSea::AddQuakePoint( float _x, float _z, float fAmplitude, float fAngVelocity )
{
    float t = timeGetTime() / 1000.0f;
    mQuakList[LnQuakeCount++].Init( _x, _z, fAmplitude, fAngVelocity, t );
    return LnQuakeCount;
}

void CSea::Update( float fpass )
{

    //海面纹理按时间切换

    static DWORD dwMainTime = 0;
    static DWORD dwPicIndex = 0;

    dwMainTime += ( DWORD )fpass;

    if ( dwMainTime > dwPicIndex * FaceSwitchTime )
    {
        dwPicIndex++;
    }

    if ( dwPicIndex > SEAPICNUM - 1 )
    {
        dwMainTime = 0;
        dwPicIndex = 0;
    }


    HRESULT hr = S_FALSE;


    DWORD dwIndex = 0;

	CXASSERT(mMeshResource.size()==1);
	GGraph* renderData=mMeshResource[0];
	
	SeaVertex *pVertexBuffer=(SeaVertex*)renderData->getVertexBufferPointer();

    for ( int i = 0; i < mCellCount + 1; i++ )
    {
        for ( int j = 0; j < mCellCount + 1; j++ )
        {
            dwIndex = i * ( mCellCount + 1 ) + j;
            float fDelta = 0;

            for ( int k = 0; k < LnQuakeCount; k++ )
            {
                fDelta += mQuakList[k].GetPointEffect( pVertexBuffer[dwIndex].vertex.x, pVertexBuffer[dwIndex].vertex.z, ( float )timeGetTime() / 1000.0f );
            }

            pVertexBuffer[dwIndex].vertex.y = fDelta;
        }
    }

	renderData->copyVertexBufferToGPU();

    //DWORD dwTime=timeGetTime();
    char sFileName[128];
    ZeroMemory( sFileName, sizeof( sFileName ) );
    //sprintf(sFileName,"res\\water\\BlueShort\\A21C_%03d.jpg",dwTime%SEAPICNUM);

    sprintf( sFileName, "..\\Data\\res\\water\\BlueShort\\A21C_%03d.jpg", dwPicIndex );
	if(mpFace)
		mpFace->safeRelease();
	mpFace=TextureMgr.getResource(sFileName);

}



MeshPara* CSea::CreateParam() const
{
	return mCreateParam;
}

void CSea::CreateParam( const MeshPara& val )
{
	dSafeDelete(mCreateParam);
	mCreateParam = new MeshPara;
	dMemoryCopy(mCreateParam,(void*)&val,sizeof(MeshPara));
}





void QuakePoint::Init( float _x, float _z, float amplitude, float AngVelocity, float _t )
{
    mfZ = _z;
    mfX = _x;

    mAmplitude = amplitude;
    mAngVelocity = AngVelocity;

    mInitTime = _t;
}

float QuakePoint::GetPointEffect( float _x, float _z, float _t )
{
    float len = D3DXVec3Length( &( D3DXVECTOR3( mfX, 0, mfZ ) - D3DXVECTOR3( _x, 0, _z ) ) );

    float dt = len / QUAKE_SPEED;

    float fDeltaY = mAmplitude * sinf( mAngVelocity * ( _t - mInitTime ) - dt );

    return fDeltaY;
}
