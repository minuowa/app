#include "GGameDemoHeader.h"
#include "GGameMap.h"
#include "GMeshBuffer.h"
#include "GDevice_D3D.h"

GGameMap::GGameMap( void )
    : _pTexture( 0 )
    , _pMesh( 0 )
    , _pMat( 0 )
{
    ZeroMemory( mppStaticObj, sizeof( mppStaticObj ) );
}

GGameMap::~GGameMap( void )
{
    for ( unsigned int i = 0; i < _nNumFace; ++i )
    {
        if ( _pTexture )
        {
            SAFERELEASE( _pTexture[i] );
        }
    }

    _nNumFace = 0;
    SAFERELEASE( _pMesh );
    SAFED_ELETE( _pMat );
}

bool GGameMap::Create()
{
    if(!__super::Create(  ))
		return false;

    //��ȡ����

    //mMeshPara = ( MeshPara* )Para;

    //LnCellCount = mMeshPara->LnCellCount;
    //mfMaxHeight = mMeshPara->mfMaxHeight;
    //mfCellWidth = mMeshPara->mfCellWidth;
    //mstrFileName = mMeshPara->mstrFileName;
    //mstrHeightMap = mMeshPara->mstrHeightMap;

    AfterCreate();

    return true;
}

bool GGameMap::Render()
{
	if(!__super::Render())
		return false;
    D3DXMATRIX mat;
    D3DXMatrixIdentity( &mat );
    Device->GetDvc()->SetTransform( D3DTS_TEXTURE0, &mat );

    return true;

}


void GGameMap::LoadFromFile( char *sConfigName )
{
    if ( IsStrEmpty( sConfigName ) )
    {
        return;
    }

    char sTmp[128];
    ZeroMemory( sTmp, sizeof( sTmp ) );

    mstrHeightMap = new char[FILE_NAME_LENGTH];
    GetPrivateProfileStringA( "Map", "HeightMapFileName", "", mstrHeightMap, 128, sConfigName );

    mstrFileName = new char[FILE_NAME_LENGTH];
    GetPrivateProfileStringA( "Map", "TextureFileName", "", mstrFileName, 128, sConfigName );

    GetPrivateProfileStringA( "Map", "fMapCellWidth", "80.0", sTmp, 128, sConfigName );
    mfCellWidth = atof( sTmp );
    ZeroMemory( sTmp, sizeof( sTmp ) );

    GetPrivateProfileStringA( "Map", "fMapMaxHeight", "800.0", sTmp, 128, sConfigName );
    mfMaxHeight = atof( sTmp );
    ZeroMemory( sTmp, sizeof( sTmp ) );

    LnCellCount = GetPrivateProfileIntA( "Map", "nMapCellCount", 64, sConfigName );

    char sName[32];
    ZeroMemory( sName, sizeof( sName ) );

    for ( int i = 1; i < 10; i++ )
    {
        sprintf( sName, "OBJ%d", i );
        GetPrivateProfileStringA( "ObjList", sName, "", sTmp, 128, sConfigName );

        if ( strlen( sTmp ) == 0 )
        {
            break;
        }

        StrToMapObjInfo( sTmp, &mppStaticObj[i - 1] );

        ZeroMemory( sName, sizeof( sName ) );
        ZeroMemory( sTmp, sizeof( sTmp ) );
    }

}


bool GGameMap::StrToMapObjInfo( char *str, MapObjInfo *pObjInfo )
{
    if ( IsStrEmpty( str ) || pObjInfo == NULL )
    {
        return false;
    }

    char sTmp[128];
    ZeroMemory( sTmp, sizeof( sTmp ) );

    GetSubString( str, ",", 0, sTmp );

    pObjInfo->nObjId = StrToIntA( sTmp );

    ZeroMemory( sTmp, sizeof( sTmp ) );
    GetSubString( str, ",", 1, sTmp );
    pObjInfo->x = atof( sTmp );

    ZeroMemory( sTmp, sizeof( sTmp ) );
    GetSubString( str, ",", 2, sTmp );
    pObjInfo->z = atof( sTmp );

    return true;
}

int GGameMap::AfterCreate()
{

    IDirect3DTexture9 *pHeightMap = NULL;	  //�߶�ͼ����

    D3DSURFACE_DESC surfaceDesc;    //�߶�ͼ�������

    D3DLOCKED_RECT rcLock;	//�����ı���

    MAPVERTEX *pVertextBuffer = NULL;	  //Mesh�Ķ��㻺����

    DWORD *pIndexBuffer = NULL;	  //Mesh������������

    DWORD dwWidth = 0;	  //������
    DWORD dwHeight = 0;	  //����߶�

    HRESULT hr = -1;

    //����Mesh

    SAFERELEASE( _pMesh );

    hr = D3DXCreateMeshFVF(
             LnCellCount * LnCellCount * 2,
             ( LnCellCount + 1 ) * ( LnCellCount + 1 ),
             D3DXMESH_32BIT | D3DXMESH_MANAGED, FVFMAP, Device->GetDvc(), &_pMesh
         );

    if ( FAILED( hr ) )
    {
        return -1;
    }

    _nNumFace = 1;
    _pMat = new D3DMATERIAL9[_nNumFace];

    _pTexture = new LPDIRECT3DTEXTURE9[_nNumFace];

    //���ò���
    ZeroMemory( _pMat, _nNumFace * sizeof( D3DMATERIAL9 ) );

    _pMat[0].Diffuse.r = 0.4f;
    _pMat[0].Diffuse.g = 0.4f;
    _pMat[0].Diffuse.b = 0.4f;
    _pMat[0].Diffuse.a = 1.0f;
    _pMat[0].Ambient.r = 0.3f;
    _pMat[0].Ambient.g = 0.3f;
    _pMat[0].Ambient.b = 0.3f;
    _pMat[0].Ambient.a = 1.0f;
    _pMat[0].Specular.r = 0.6f;
    _pMat[0].Specular.g = 0.6f;
    _pMat[0].Specular.b = 0.6f;
    _pMat[0].Specular.a = 1.0f;
    _pMat[0].Emissive.r = 0.2f;
    _pMat[0].Emissive.g = 0.2f;
    _pMat[0].Emissive.b = 0.2f;
    _pMat[0].Emissive.a = 1.0f;
    _pMat[0].Power = 9.0f;

    //��������
    if ( mstrFileName != NULL )
    {
        hr = D3DXCreateTextureFromFileA( Device->GetDvc(), mstrFileName, &_pTexture[0] );
    }

    //��ȡ�߶�ͼ
    if ( mstrHeightMap != NULL )
    {
        hr = D3DXCreateTextureFromFileA( Device->GetDvc(), mstrHeightMap, &pHeightMap );
    }

    pHeightMap->GetLevelDesc( 0, &surfaceDesc );

    dwHeight = surfaceDesc.Height;
    dwWidth = surfaceDesc.Width;

    hr = pHeightMap->LockRect( 0, &rcLock, NULL, 0 );

    DWORD dwPitch = rcLock.Pitch / sizeof( DWORD );

    DWORD *pBit = ( DWORD* )rcLock.pBits;

    _pMesh->LockVertexBuffer( D3DLOCK_DISCARD, ( void** )&pVertextBuffer );

    DWORD dwRow, dwLine;
    //���ö�������
    DWORD dwIndex;

    for ( int i = 0; i < LnCellCount + 1; i++ )
    {
        for ( int j = 0; j < LnCellCount + 1; j++ )
        {

            dwLine = dwWidth * j / ( LnCellCount + 1 );
            dwRow = dwHeight * ( LnCellCount + 1 - i ) / ( LnCellCount + 1 );

            DWORD dwInd = dwRow * dwWidth + dwLine;

            DWORD dwcolor = pBit[dwInd];
            dwcolor = dwcolor << 8;
            dwcolor = dwcolor >> 24;

            float fHeight = ( float )dwcolor;

            dwIndex = i * ( LnCellCount + 1 ) + j;

            pVertextBuffer[dwIndex].vVertex.x = ( j - LnCellCount / 2.0f ) * mfCellWidth;
            pVertextBuffer[dwIndex].vVertex.y = ( fHeight - 255 * SEA_DEPTH_FACTOR ) / 255.0f * mfMaxHeight;
            pVertextBuffer[dwIndex].vVertex.z = ( i - LnCellCount / 2.0f ) * mfCellWidth;
            pVertextBuffer[dwIndex].u = j;
            pVertextBuffer[dwIndex].v = ( LnCellCount - i );
        }
    }

    DWORD dwVerNum = LnCellCount + 1;

    //ƽ������
    for ( int i = 1; i < LnCellCount; i++ )
    {
        for ( int j = 1; j < LnCellCount; j++ )
        {
            pVertextBuffer[i * ( LnCellCount + 1 ) + j].vVertex.y = 0.125f * (
                        pVertextBuffer[( i - 1 ) * dwVerNum + j - 1].vVertex.y +
                        pVertextBuffer[( i - 1 ) * dwVerNum + j].vVertex.y +
                        pVertextBuffer[( i - 1 ) * dwVerNum + j - 1].vVertex.y +
                        pVertextBuffer[i * dwVerNum + j - 1].vVertex.y +
                        pVertextBuffer[i * dwVerNum + j + 1].vVertex.y +
                        pVertextBuffer[( i + 1 ) * dwVerNum + j - 1].vVertex.y +
                        pVertextBuffer[( i + 1 ) * dwVerNum + j].vVertex.y +
                        pVertextBuffer[( i + 1 ) * dwVerNum + j + 1].vVertex.y
                    );
        }
    }

    pHeightMap->UnlockRect( 0 );

    SAFERELEASE( pHeightMap );

    _pMesh->UnlockVertexBuffer();

    //������������
    _pMesh->LockIndexBuffer( D3DLOCK_DISCARD, ( void** )&pIndexBuffer );

    DWORD dwBaseIndex = 0;

    for ( int i = 0; i < LnCellCount; i++ )
    {
        for ( int j = 0; j < LnCellCount; j++ )
        {
            pIndexBuffer[dwBaseIndex + 0] = i * ( LnCellCount + 1 ) + j;
            pIndexBuffer[dwBaseIndex + 1] = ( i + 1 ) * ( LnCellCount + 1 ) + j;
            pIndexBuffer[dwBaseIndex + 2] = ( i + 1 ) * ( LnCellCount + 1 ) + j + 1;

            pIndexBuffer[dwBaseIndex + 3] = i * ( LnCellCount + 1 ) + j;
            pIndexBuffer[dwBaseIndex + 4] = ( i + 1 ) * ( LnCellCount + 1 ) + j + 1;
            pIndexBuffer[dwBaseIndex + 5] = i * ( LnCellCount + 1 ) + j + 1;

            dwBaseIndex += 6;
        }
    }

    _pMesh->UnlockIndexBuffer();

    DWORD dwNum = _pMesh->GetNumFaces() * 3;
    DWORD *pAdj = new DWORD[_pMesh->GetNumFaces() * 3];

    mMeshBufferNode = new GMeshBufferNode();
    mMeshBufferNode->SubSetCount( 1);
    mMeshBufferNode->Mesh(_pMesh);
    mLighting = true;

	GGraph* data = new GGraph;
	data->setTexture(mstrFileName);
	data->setMetiral(*_pMat);
	mMeshBufferNode->Add(data);
    _pMesh->GenerateAdjacency( 1.0f, pAdj );

    mMeshBufferNode->MakeLod( pAdj );

    SetNormal( mMeshBufferNode->Mesh(), Device->GetDvc() );

    ResetVectorMesh();

    return _nID;
}

bool GGameMap::IsBlock( D3DXVECTOR3 vPos, int *pCounter )
{
    bool bBlock = false;

    for ( int i = 0; i < 1000; i++ )
    {
        if ( mppStaticObj[i].nObjId < 1 )
        {
            return false;
        }

        D3DXVECTOR3 vMin( mppStaticObj[i].vMin ), vMax( mppStaticObj[i].vMax );

        if ( vPos.x > vMin.x && vPos.x < vMax.x && vPos.z > vMin.z && vPos.z < vMax.z )
        {
            if ( pCounter != NULL )
            {
                *pCounter = i;
            }

            return true;
        }
    }

    return false;
}



