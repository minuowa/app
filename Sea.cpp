#include "GGameDemoHeader.h"
#include "Sea.h"
#include "GMeshBuffer.h"
#include "GD9Device.h"

CSea::CSea( void )
	:mCreateParam(0)
{
    LnQuakeCount = 0;

    mpFace = NULL;

    LnCellCount = 0;
    mfMaxHeight = 0;
    mfCellWidth = 0;
    mstrFileName = NULL;
	CXSafeDelete(mCreateParam);
}

CSea::~CSea( void )
{
}

bool CSea::Create()
{
    if(!__super::Create())
		return false;

    HRESULT hr = S_FALSE;

    SeaVertex *pVertextBuffer = NULL;	  //Mesh的顶点缓冲区

    DWORD *pIndexBuffer = NULL;	  //Mesh的索引缓冲区


    mMeshBufferNode = new GMeshBufferNode();
    LnCellCount = mCreateParam->LnCellCount;
    mfMaxHeight = mCreateParam->mfMaxHeight;
    mfCellWidth = mCreateParam->mfCellWidth;
    mstrFileName = mCreateParam->mstrFileName;

    //SAFERELEASE(mMeshBufferNode->mpRootMesh);

	ID3DXMesh* mesh=0;
    hr = D3DXCreateMeshFVF(
             LnCellCount * LnCellCount * 2,
             ( LnCellCount + 1 ) * ( LnCellCount + 1 ),
             D3DXMESH_32BIT | D3DXMESH_MANAGED, FVFSea, D9DEVICE->GetDvc(),
			 &mesh
         );
    DebugMsgBox( hr, "创建海面Mesh失败！" );

    DWORD dwIndex = 0;

    mesh->LockVertexBuffer( D3DLOCK_DISCARD, ( void** )&pVertextBuffer );

    for ( int i = 0; i < LnCellCount + 1; i++ )
    {
        for ( int j = 0; j < LnCellCount + 1; j++ )
        {
            dwIndex = i * ( LnCellCount + 1 ) + j;

            pVertextBuffer[dwIndex].vertex.x = ( j - LnCellCount / 2.0f ) * mfCellWidth;
            pVertextBuffer[dwIndex].vertex.y = 0;
            pVertextBuffer[dwIndex].vertex.z = ( i - LnCellCount / 2.0f ) * mfCellWidth;
            pVertextBuffer[dwIndex].u = j / 10.0f;
            pVertextBuffer[dwIndex].v = ( LnCellCount - i ) / 10.0f;
        }
    }

    mesh->UnlockVertexBuffer();

    mesh->LockIndexBuffer( D3DLOCK_DISCARD, ( void** )&pIndexBuffer );

    DWORD dwBaseIndex = 0;

    for ( int i = 0; i < LnCellCount; i++ )
    {
        for ( int j = 0; j < LnCellCount; j++ )
        {
            pIndexBuffer[dwBaseIndex + 0] = i * ( LnCellCount + 1 ) + j;
            pIndexBuffer[dwBaseIndex + 1] = ( i + 1 ) * ( LnCellCount + 1 ) + j;
            pIndexBuffer[dwBaseIndex + 2] = ( i + 1 ) * ( LnCellCount + 1 ) + j + 1;

            pIndexBuffer[dwBaseIndex + 3] = i * ( LnCellCount + 1 ) + j;
            pIndexBuffer[dwBaseIndex + 4] = ( i + 1 ) * ( LnCellCount + 1 ) + j + 1;;
            pIndexBuffer[dwBaseIndex + 5] = i * ( LnCellCount + 1 ) + j + 1;

            dwBaseIndex += 6;
        }
    }

    mesh->UnlockIndexBuffer();

	mMeshBufferNode->Mesh(mesh);
    mMeshBufferNode->SubSetCount(1);

    DWORD *pAdj = new DWORD[mesh->GetNumFaces() * 3];

    mesh->GenerateAdjacency( 1.0f, pAdj );
	GMetrialData* material=new GMetrialData;
	mMeshBufferNode->Add(material);
    mpFace = new LPDIRECT3DTEXTURE9[1];
	material->SetTexture(mstrFileName);

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
	material->SetMetiral(mtrl);

    SetNormal( mMeshBufferNode->Mesh(), D9DEVICE->GetDvc() );

    ResetVectorMesh();

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

    SeaVertex *pVertexBuffer;

    DWORD dwIndex = 0;

    mMeshBufferNode->Mesh()->LockVertexBuffer( D3DLOCK_DISCARD, ( void** )&pVertexBuffer );

    for ( int i = 0; i < LnCellCount + 1; i++ )
    {
        for ( int j = 0; j < LnCellCount + 1; j++ )
        {
            dwIndex = i * ( LnCellCount + 1 ) + j;
            float fDelta = 0;

            for ( int k = 0; k < LnQuakeCount; k++ )
            {
                fDelta += mQuakList[k].GetPointEffect( pVertexBuffer[dwIndex].vertex.x, pVertexBuffer[dwIndex].vertex.z, ( float )timeGetTime() / 1000.0f );
            }

            pVertexBuffer[dwIndex].vertex.y = fDelta;
        }
    }

    mMeshBufferNode->Mesh()->UnlockVertexBuffer();

    //DWORD dwTime=timeGetTime();
    char sFileName[128];
    ZeroMemory( sFileName, sizeof( sFileName ) );
    //sprintf(sFileName,"res\\water\\BlueShort\\A21C_%03d.jpg",dwTime%SEAPICNUM);

    sprintf( sFileName, "..\\Data\\res\\water\\BlueShort\\A21C_%03d.jpg", dwPicIndex );

    SAFERELEASE( mpFace[0] );

    hr = D3DXCreateTextureFromFileA( D9DEVICE->GetDvc(), sFileName, &mpFace[0] );

}



MeshPara* CSea::CreateParam() const
{
	return mCreateParam;
}

void CSea::CreateParam( const MeshPara& val )
{
	CXSafeDelete(mCreateParam);
	mCreateParam = new MeshPara;
	CXMemoryCopy(mCreateParam,(void*)&val,sizeof(MeshPara));
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
