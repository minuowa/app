#include "GGameDemoHeader.h"
#include "GAnimMeshObj.h"
#include "XCursor.h"

#include "Particles.h"

GAnimMeshObj gAnimMesh[4];

HRESULT AllocateName( LPCTSTR Name, LPTSTR *pNewName )
{
    //Name为空值，返回错误
    if ( Name == NULL )
    {
        *pNewName = NULL;


        return S_FALSE;
    }

    int nLen = lstrlen( Name ) + 1;

    *pNewName = new TCHAR[nLen];

    //内存不足，返回错误
    if ( *pNewName == NULL )
    {
        return E_OUTOFMEMORY;
    }

    memcpy( *pNewName, Name, nLen * sizeof( TCHAR ) );

    return S_OK;

}


GAnimMeshObj::GAnimMeshObj( void )
{
    ZeroMemory( m_sXFileName, sizeof( m_sXFileName ) );

    ZeroMemory( m_sTextureName, sizeof( m_sTextureName ) );

    m_nAnimMeshID = 0;

    mpAmmo = NULL;

    LnNowBoneInfoCount = -1;

    mbPlayDone = false;

    mpAnimSet = NULL;

    mpFace = NULL;

    mpFrameRoot = NULL;

    mpAnimController = NULL;
}

GAnimMeshObj::~GAnimMeshObj( void )
{

}



HRESULT GAnimMeshObj::SetupBoneMatrixPointers( LPD3DXFRAME pFrame )
{
    HRESULT hr = S_FALSE;

    if ( pFrame == NULL )
    {
        return S_FALSE;
    }

    if ( pFrame->pMeshContainer != NULL )
    {
        hr = SetupBoneMatrixPointersOnMesh( pFrame->pMeshContainer );

        if ( FAILED( hr ) )
        {
            return hr;
        }
    }

    if ( pFrame->pFrameSibling != NULL )
    {
        hr = SetupBoneMatrixPointers( pFrame->pFrameSibling );

        if ( FAILED( hr ) )
        {
            return hr;
        }
    }

    if ( pFrame->pFrameFirstChild != NULL )
    {
        hr = SetupBoneMatrixPointers( pFrame->pFrameFirstChild );

        if ( FAILED( hr ) )
        {
            return hr;
        }
    }

    return S_OK;
}

HRESULT GAnimMeshObj::SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase )
{
    D3DXFrameEX *pFrameEx = NULL;

    if ( pMeshContainerBase == NULL || pMeshContainerBase->pSkinInfo == NULL )
    {
        return S_FALSE;
    }

    D3DXMeshContainerEX *pMeshContainerEx = ( D3DXMeshContainerEX* )pMeshContainerBase;


    DWORD dwNumBone = pMeshContainerEx->pSkinInfo->GetNumBones();

    pMeshContainerEx->ppBoneMatrixPtrs = new D3DXMATRIX*[dwNumBone];

    if ( pMeshContainerEx->ppBoneMatrixPtrs == NULL )
    {
        return E_OUTOFMEMORY;
    }

    for ( DWORD i = 0; i < dwNumBone; i++ )
    {
        char sBoneName[32];
        ZeroMemory( sBoneName, sizeof( sBoneName ) );

        strcpy( sBoneName, pMeshContainerEx->pSkinInfo->GetBoneName( i ) );

        pFrameEx = ( D3DXFrameEX* )D3DXFrameFind( mpFrameRoot, sBoneName );

        if ( pFrameEx == NULL )
        {
            return S_FALSE;
        }

        AddBoneInfo( sBoneName, &( pFrameEx->matCombinedTransformation ) );


        pMeshContainerEx->ppBoneMatrixPtrs[i] = &( pFrameEx->matCombinedTransformation );
    }

    return S_OK;

}

VOID GAnimMeshObj::UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix )
{
    if ( pFrameBase == NULL )
    {
        return;
    }

    D3DXFrameEX *pFrameEx = ( D3DXFrameEX* )pFrameBase;

    if ( pParentMatrix != NULL )
    {
        D3DXMatrixMultiply( &pFrameEx->matCombinedTransformation, &pFrameEx->TransformationMatrix, pParentMatrix );
    }
    else
    {
        pFrameEx->matCombinedTransformation = pFrameEx->TransformationMatrix;
    }

    if ( pFrameEx->pFrameSibling != NULL )
    {
        UpdateFrameMatrices( pFrameEx->pFrameSibling, pParentMatrix );
    }

    if ( pFrameEx->pFrameFirstChild != NULL )
    {
        UpdateFrameMatrices( pFrameEx->pFrameFirstChild, &pFrameEx->matCombinedTransformation );
    }
}

void GAnimMeshObj::DrawMeshContainer( D3DXMESHCONTAINER *pMeshContainerBase, D3DXFRAME *pFrameBase )
{
    if ( pMeshContainerBase == NULL || pFrameBase == NULL )
    {
        return;
    }

    UINT iMatrixIndex = 0;

    D3DXBONECOMBINATION *pBoneComb = NULL;

    D3DXMATRIX matTmp;


    D3DXMeshContainerEX *pMeshContainerEx = ( D3DXMeshContainerEX* )pMeshContainerBase;
    D3DXFrameEX *pFrameEx = ( D3DXFrameEX* )pFrameBase;

    D3DCAPS9 d3dCap;
    ZeroMemory( &d3dCap, sizeof( d3dCap ) );
    D9DEVICE->GetDvc()->GetDeviceCaps( &d3dCap );

    if ( pMeshContainerEx->pBoneCombinationBufffer != NULL )
    {
        pBoneComb = ( D3DXBONECOMBINATION* )( pMeshContainerEx->pBoneCombinationBufffer->GetBufferPointer() );
    }


    D9DEVICE->OpenAllLight( true );

    DWORD dwAmbient = 0;
    D9DEVICE->GetDvc()->GetRenderState( D3DRS_AMBIENT, &dwAmbient );

    POINT pt = INPUTSYSTEM.GetMousePoint();

    bool bHit = false;

    if ( INPUTSYSTEM.IsPressingButton( btLB ) )
    {
        if ( pMeshContainerEx != NULL && pMeshContainerEx->MeshData.pMesh != NULL && !m_bHit )
        {
            m_bHit = Pick( pMeshContainerEx->MeshData.pMesh, pt );

            if ( m_bHit )
            {
                Toggle( mXPos.mbCanMoveStep );
            }
        }
    }

    for ( DWORD iAttr = 0; iAttr < pMeshContainerEx->NumAttributeGroups; iAttr++	)
    {
        if ( d3dCap.MaxVertexBlendMatrices >= pMeshContainerEx->NumInfl )
        {
            D9DEVICE->GetDvc()->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );

            for ( DWORD i = 0; i < pMeshContainerEx->NumInfl; ++i )
            {
                iMatrixIndex = pBoneComb[iAttr].BoneId[i];

                if ( iMatrixIndex != UINT_MAX )
                {
                    D3DXMatrixMultiply
                    (
                        &matTmp,
                        &pMeshContainerEx->pBoneOffsetMatrices[iMatrixIndex],
                        pMeshContainerEx->ppBoneMatrixPtrs[iMatrixIndex]
                    );

                    D9DEVICE->GetDvc()->SetTransform( D3DTS_WORLDMATRIX( i ), &matTmp );
                }
            }

            D9DEVICE->GetDvc()->SetRenderState( D3DRS_VERTEXBLEND, pMeshContainerEx->NumInfl - 1 );

            D9DEVICE->GetDvc()->SetMaterial( &pMeshContainerEx->pMaterials[pBoneComb[iAttr].AttribId].MatD3D );

            if ( m_bHit )
            {
                gCursor.SetNowCursor( curGrasp );
                D9DEVICE->GetDvc()->SetTexture( 0, NULL );
            }
            else
            {
                gCursor.SetNowCursor( curNormal );
                D9DEVICE->GetDvc()->SetTexture( 0, mpFace );
            }


            DWORD dwLight = 0;

            D9DEVICE->GetDvc()->GetRenderState( D3DRS_LIGHTING, &dwLight );

            pMeshContainerEx->MeshData.pMesh->DrawSubset( iAttr );

        }

        D9DEVICE->GetDvc()->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );

        D9DEVICE->GetDvc()->SetRenderState( D3DRS_VERTEXBLEND, FALSE );
    }

    D9DEVICE->OpenAllLight( false );

}

void GAnimMeshObj::DrawFrame( D3DXFRAME *pFrameBase )
{
    if ( pFrameBase == NULL )
    {
        return ;
    }

    D3DXMESHCONTAINER *pMeshContainer = pFrameBase->pMeshContainer;

    while ( pMeshContainer != NULL )
    {
        DrawMeshContainer( pMeshContainer, pFrameBase );

        pMeshContainer = pMeshContainer->pNextMeshContainer;
    }

    if ( pFrameBase->pFrameSibling != NULL )
    {
        DrawFrame( pFrameBase->pFrameSibling );
    }

    if ( pFrameBase->pFrameFirstChild != NULL )
    {
        DrawFrame( pFrameBase->pFrameFirstChild );
    }

}

eObjAnimState GAnimMeshObj::SetState( eObjAnimState oas, bool bBack )
{
    ID3DXAnimationSet *pAS = NULL;

    HRESULT hr = S_FALSE;

    if ( oas == m_ObjAnimState )
    {
        return m_ObjAnimState;
    }

    mXPos.mbBack = bBack;

    if ( mpAnimController == NULL )
    {
        return oasNULL;
    }

    if ( m_ObjAnimState == oasAttack || oasRunAttack == m_ObjAnimState )
    {
        if ( !mbPlayDone )
        {
            return m_ObjAnimState;
        }
    }

    m_ObjAnimState = oas;

    //获取动作集
    switch ( oas )
    {
        case oasDead:

            mpAnimController->GetAnimationSetByName( "Death", &pAS );

            break;

        case oasMoving:

            mpAnimController->GetAnimationSetByName( "Move", &pAS );

            break;

        case oasStandBy:

            mpAnimController->GetAnimationSetByName( "Stand_By", &pAS );

            break;

        case oasBeAttack:

            mpAnimController->GetAnimationSetByName( "Be_Attacked", &pAS );
            break;

        case oasRunAttack:

            mpAnimController->GetAnimationSetByName( "RunAttact", &pAS );
            break;

        case oasAttack:

            mpAnimController->GetAnimationSetByName( "Attack", &pAS );

            SAFED_ELETE( mpAmmo );

            //mpAmmo = new CAmmoParticles;

            //mpAmmo->Create( this, D9DEVICE, 4000, mForceMap );

            //mpAmmo->Shoot();

            break;
    }

    if ( pAS != NULL )
    {
        //设置动作速度

        if ( mXPos.mbBack )
        {
            mpAnimController->SetTrackSpeed( 0, -1 );
        }
        else
        {
            mpAnimController->SetTrackSpeed( 0, 1 );
        }


        //设置动画集

        hr = mpAnimController->SetTrackAnimationSet( 0, pAS );

        //设置动画播放起点

        hr = mpAnimController->SetTrackPosition( 0, 0 );

        SAFERELEASE( mpAnimSet );

        mdwOldAnimSetFrame = 0;
        mdwCurAnimSetFrame = 0;

        mbPlayDone = false;

        mpAnimSet = pAS;

    }

    return m_ObjAnimState;
}

int GAnimMeshObj::Render( float fPass )
{
    GRenderObject::Render();

    if ( mpFrameRoot != NULL )
    {
        mpAnimController->AdvanceTime( fPass, NULL );

        if ( mpAnimSet != NULL )
        {
            D3DXTRACK_DESC trackDesc;

            mpAnimController->GetTrackDesc( 0, &trackDesc );

            double dbPassTime = mpAnimSet->GetPeriodicPosition( trackDesc.Position );

            mdwCurAnimSetFrame = dbPassTime * 300000;

            if ( mdwCurAnimSetFrame < mdwOldAnimSetFrame )
            {
                mbPlayDone = true;
            }

            mdwOldAnimSetFrame = mdwCurAnimSetFrame;
        }
    }

    UpdateFrameMatrices( mpFrameRoot, &_matWorld );

    DrawFrame( mpFrameRoot );

    if ( mpAmmo != NULL )
    {
        mpAmmo->Update();

        mpAmmo->Render();

        if ( !mpAmmo->mXPos.mbAutoMove )
        {
            SAFED_ELETE( mpAmmo );
        }
    }

    m_bHit = false;

    return TRUE_INT;

}

bool GAnimMeshObj::Pick( ID3DXMesh *pMesh, POINT pt )
{
    HRESULT hr = S_FALSE;

    BOOL bHit = false;

    D3DXMATRIX matProj, matView, matWorld;
    D3DXMATRIX matTmp;

    D9DEVICE->GetDvc()->GetTransform( D3DTS_PROJECTION, &matProj );
    D9DEVICE->GetDvc()->GetTransform( D3DTS_VIEW, &matView );
    D9DEVICE->GetDvc()->GetTransform( D3DTS_WORLD, &matWorld );

    D3DXVECTOR4 vOrigin( ZEROFLOAT, ZEROFLOAT, ZEROFLOAT, 1.0f );
    D3DXVECTOR4 vDir;

    //注意此处的2.0f与2的区别
    vDir.x = ( 2.0f * pt.x / D9DEVICE->mWidth - 1 ) / matProj._11;
    vDir.y = -( 2.0f * pt.y / D9DEVICE->mHeight - 1 ) / matProj._22;
    vDir.z = 1.0f;
    vDir.w = 0.0f;

    matTmp = matWorld * matView;
    D3DXMatrixInverse( &matTmp, NULL, &matTmp );

    D3DXVec4Transform( &vOrigin, &vOrigin, &matTmp );
    D3DXVec4Transform( &vDir, &vDir, &matTmp );

    hr = D3DXIntersect( pMesh, ( D3DXVECTOR3* )&vOrigin, ( D3DXVECTOR3* )&vDir, &bHit, NULL, NULL, NULL, NULL, NULL, NULL );

    if ( FAILED( hr ) )
    {
        return false;
    }

    return ( BOOL )bHit;
}

FiLinker * GAnimMeshObj::GetBoneInfo( CChar*sBoneName )
{
    FiLinker *pBoneInfo = NULL;

    CXASSERT( sBoneName );
    FiLinkerMap::iterator i = mBoneInfos.find( sBoneName );

    if ( i != mBoneInfos.end() )
    {
        return i->second;
    }

    return pBoneInfo;
}

int GAnimMeshObj::AddBoneInfo( CChar*sBoneName, D3DXMATRIX *pmat )
{
    CXASSERT( sBoneName );
    FiLinker*  linker = new FiLinker;
    linker->Name = sBoneName;
    linker->Transform = pmat;

    if ( !mBoneInfos.Insert( sBoneName, linker ) )
    {
        CXASSERT( 0 );
        return 0;
    }

    return mBoneInfos.size();
}

D3DXMATRIX GAnimMeshObj::GetWorldMatrixByBone( char *sBoneName, bool bForTrans/*=false*/ )
{
    if ( IsStrEmpty( sBoneName ) )
    {
        return GNode::GetWorldMatrixByBone( sBoneName, bForTrans );
    }

    FiLinker *pBoneInfo = GetBoneInfo( sBoneName );

    if ( pBoneInfo != NULL )
    {
        return *( pBoneInfo->Transform );
    }

    return GNode::GetWorldMatrixByBone( sBoneName, bForTrans );
}

int GAnimMeshObj::AfterCreate()
{
    CAllocateHierarchy Alloc( D9DEVICE->GetDvc() );

    HRESULT hr = S_FALSE;

    if ( !IsStrEmpty( m_sXFileName ) )
    {
        hr = D3DXLoadMeshHierarchyFromXA( m_sXFileName, D3DXMESH_MANAGED, D9DEVICE->GetDvc(), &Alloc, NULL, &mpFrameRoot, &mpAnimController );
    }

    if ( FAILED( hr ) )
    {
        return hr;
    }



    if ( !IsStrEmpty( m_sTextureName ) )
    {
        D3DXCreateTextureFromFileA( D9DEVICE->GetDvc(), m_sTextureName, &mpFace );
    }

    SetupBoneMatrixPointers( mpFrameRoot );

    SetState( oasStandBy, false );

    return _nID;
}

bool GAnimMeshObj::Create()
{
    if(!__super::Create())
		return false;
    AfterCreate();

    return true;
}

STDMETHODIMP CAllocateHierarchy::CreateFrame( LPCSTR Name, LPD3DXFRAME *ppNewFrame )
{

    HRESULT hr = S_FALSE;

    *ppNewFrame = NULL;

    D3DXFrameEX *pFrameTmp;
    pFrameTmp = new D3DXFrameEX;

    if ( pFrameTmp == NULL )
    {
        delete( pFrameTmp );
        return E_OUTOFMEMORY;
    }

    hr = AllocateName( ( LPCTSTR )Name, ( LPTSTR * )( &pFrameTmp->Name ) );

    if ( FAILED( hr ) )
    {
        delete( pFrameTmp );
        return S_FALSE;
    }

    D3DXMatrixIdentity( &pFrameTmp->TransformationMatrix );

    D3DXMatrixIdentity( &pFrameTmp->matCombinedTransformation );

    pFrameTmp->pFrameFirstChild = NULL;

    pFrameTmp->pFrameSibling = NULL;

    pFrameTmp->pMeshContainer = NULL;

    *ppNewFrame = pFrameTmp;

    pFrameTmp = NULL;

    return S_OK;

}


STDMETHODIMP CAllocateHierarchy::CreateMeshContainer
(
    LPCSTR Name,
    CONST D3DXMESHDATA *pMeshData,
    CONST D3DXMATERIAL *pMaterials,
    CONST D3DXEFFECTINSTANCE *pEffectInstances,
    DWORD NumMaterials,
    CONST DWORD *pAdjacency,
    LPD3DXSKININFO pSkinInfo,
    LPD3DXMESHCONTAINER *ppNewMeshContainer
)
{

    *ppNewMeshContainer = NULL;

    ID3DXMesh *pMesh = NULL;

    HRESULT hr = S_FALSE;

    D3DXMeshContainerEX *pMeshContainerTmp = NULL;	//临时MeshContainer变量，为了给ppNewMeshContainer赋予完善的信息

    IDirect3DDevice9 *pD9Device = NULL;				//在创建纹理中要用到

    DWORD dwNumFace = 0;								//pMeshContainer->MeshData.pMesh中Mesh的面数

    DWORD dwNumBone = 0;								//影响该容器中Mesh的骨骼数量


    //获取MeshData
    if ( pMeshData->Type != D3DXMESHTYPE_MESH || pMeshData->pMesh == NULL )
    {
        return S_FALSE;
    }

    if ( pMeshData->pMesh->GetFVF() == 0 )
    {
        return S_FALSE;
    }

    pMeshContainerTmp = new D3DXMeshContainerEX;

    if ( pMeshContainerTmp == NULL )
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    ZeroMemory( pMeshContainerTmp, sizeof( D3DXMeshContainerEX ) );

    pMeshData->pMesh->GetDevice( &pD9Device );

    if ( pD9Device == NULL )
    {
        goto e_Exit;
    }

    pMesh = pMeshData->pMesh;

    if ( !( pMesh->GetFVF() && D3DFVF_NORMAL ) )	//没有法线信息为其添加一个
    {
        hr = pMeshData->pMesh->CloneMeshFVF(
                 pMeshData->pMesh->GetOptions(),
                 D3DFVF_NORMAL | pMeshData->pMesh->GetFVF(),
                 pD9Device,
                 &pMeshContainerTmp->MeshData.pMesh
             );

        if ( FAILED( hr ) )
        {
            goto e_Exit;
        }

        hr = D3DXComputeNormals( pMeshContainerTmp->MeshData.pMesh, 0 );

        if ( FAILED( hr ) )
        {
            goto e_Exit;
        }

        pMesh = pMeshContainerTmp->MeshData.pMesh;
    }
    else
    {
        pMeshContainerTmp->MeshData.pMesh = pMesh;

        pMesh->AddRef();
    }

    pMeshContainerTmp->MeshData.Type = D3DXMESHTYPE_MESH;

    //分配容器名字
    hr = AllocateName( ( LPCTSTR )Name, ( LPTSTR * )( &pMeshContainerTmp->Name ) );

    if ( FAILED( hr ) )
    {
        goto e_Exit;
    }

    //获取材质信息

    dwNumFace = pMeshContainerTmp->MeshData.pMesh->GetNumFaces();

    pMeshContainerTmp->NumMaterials = max( 1, NumMaterials );

    pMeshContainerTmp->pAdjacency = new DWORD[dwNumFace * 3];

    pMeshContainerTmp->ppTexture = new IDirect3DTexture9*[pMeshContainerTmp->NumMaterials];

    pMeshContainerTmp->pMaterials = new D3DXMATERIAL[pMeshContainerTmp->NumMaterials];

    if ( pMeshContainerTmp->ppTexture == NULL || pMeshContainerTmp->pMaterials == NULL )
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    memcpy( pMeshContainerTmp->pAdjacency, pAdjacency, sizeof( DWORD )*dwNumFace * 3 );
    ZeroMemory( pMeshContainerTmp->ppTexture, pMeshContainerTmp->NumMaterials * sizeof( IDirect3DTexture9* ) );

    if ( NumMaterials > 0 )
    {
        memcpy( pMeshContainerTmp->pMaterials, pMaterials, sizeof( D3DXMATERIAL )*NumMaterials );

        for ( DWORD i = 0; i < NumMaterials; i++ )
        {
            if ( pMeshContainerTmp->pMaterials[i].pTextureFilename != NULL )
            {
                hr = D3DXCreateTextureFromFileA( D9DEVICE->GetDvc(), pMeshContainerTmp->pMaterials[i].pTextureFilename, &pMeshContainerTmp->ppTexture[i] );
            }
        }
    }
    else	//没有材质信息就为其添加一个
    {
        pMeshContainerTmp->pMaterials[0].pTextureFilename = NULL;
        pMeshContainerTmp->pMaterials[0].MatD3D.Diffuse.r = 0.8f;
        pMeshContainerTmp->pMaterials[0].MatD3D.Diffuse.g = 0.8f;
        pMeshContainerTmp->pMaterials[0].MatD3D.Diffuse.b = 0.8f;

        pMeshContainerTmp->pMaterials[0].MatD3D.Specular = pMeshContainerTmp->pMaterials[0].MatD3D.Diffuse;
    }

    if ( pSkinInfo != NULL )
    {
        pMeshContainerTmp->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();

        pMeshContainerTmp->pOriginMesh = pMesh;
        pMesh->AddRef();

        dwNumBone = pMeshContainerTmp->pSkinInfo->GetNumBones();

        pMeshContainerTmp->pBoneOffsetMatrices = new D3DXMATRIX[dwNumBone];

        if ( pMeshContainerTmp->pBoneOffsetMatrices == NULL )
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        for ( DWORD i = 0; i < dwNumBone; i++ )
        {
            pMeshContainerTmp->pBoneOffsetMatrices[i] = *( pMeshContainerTmp->pSkinInfo->GetBoneOffsetMatrix( i ) );
        }

    }

    hr = GenerateSkinnedMesh( pMeshContainerTmp );

    *ppNewMeshContainer = pMeshContainerTmp;
    pMeshContainerTmp = NULL;

e_Exit:

    SAFERELEASE( pD9Device );

    if ( pMeshContainerTmp != NULL )
    {
        DestroyMeshContainer( pMeshContainerTmp );
    }

    return S_OK;
}

CAllocateHierarchy::CAllocateHierarchy( IDirect3DDevice9 * DVC )
{
}

HRESULT CAllocateHierarchy::DestroyMeshContainer( LPD3DXMESHCONTAINER pMeshContainerToFree )
{
    D3DXMeshContainerEX *pMeshContainerTmp = ( D3DXMeshContainerEX * )pMeshContainerToFree;

    SAFE_DELETE_ARRAY( pMeshContainerTmp->Name );
    SAFE_DELETE_ARRAY( pMeshContainerTmp->pAdjacency );
    SAFE_DELETE_ARRAY( pMeshContainerTmp->pBoneOffsetMatrices );
    SAFE_DELETE_ARRAY( pMeshContainerTmp->pMaterials );

    if ( pMeshContainerTmp->ppTexture != NULL )
    {
        for ( DWORD i = 0; i < pMeshContainerTmp->NumMaterials; i++ )
        {
            SAFERELEASE( pMeshContainerTmp->ppTexture[i] );
        }
    }

    SAFE_DELETE_ARRAY( pMeshContainerTmp->ppTexture );
    SAFE_DELETE_ARRAY( pMeshContainerTmp->ppBoneMatrixPtrs );

    SAFERELEASE( pMeshContainerTmp->pOriginMesh );
    SAFERELEASE( pMeshContainerTmp->MeshData.pMesh );
    SAFERELEASE( pMeshContainerTmp->pSkinInfo );
    SAFERELEASE( pMeshContainerTmp->pBoneCombinationBufffer );

    SAFED_ELETE( pMeshContainerTmp );

    return S_OK;
}

HRESULT CAllocateHierarchy::GenerateSkinnedMesh( D3DXMeshContainerEX *pMeshContainerEx )
{

    HRESULT hr = S_FALSE;

    D3DCAPS9 d3dCap;
    ZeroMemory( &d3dCap, sizeof( d3dCap ) );

    D9DEVICE->GetDvc()->GetDeviceCaps( &d3dCap );

    if ( pMeshContainerEx->pSkinInfo == NULL )
    {
        return S_FALSE;
    }

    SAFERELEASE( pMeshContainerEx->MeshData.pMesh );
    SAFERELEASE( pMeshContainerEx->pBoneCombinationBufffer );

    hr = pMeshContainerEx->pSkinInfo->ConvertToBlendedMesh
         (
             pMeshContainerEx->pOriginMesh,
             D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
             pMeshContainerEx->pAdjacency,
             NULL, NULL, NULL,
             &pMeshContainerEx->NumInfl,
             &pMeshContainerEx->NumAttributeGroups,
             &pMeshContainerEx->pBoneCombinationBufffer,
             &pMeshContainerEx->MeshData.pMesh
         );

    if ( FAILED( hr ) )
    {
        return hr;
    }

    D3DXBONECOMBINATION *pBoneCombinations = ( D3DXBONECOMBINATION* )( pMeshContainerEx->pBoneCombinationBufffer->GetBufferPointer() );

    DWORD dwIndex;

    for ( dwIndex = 0; dwIndex < pMeshContainerEx->NumAttributeGroups; dwIndex++ )
    {
        DWORD dwInfl = 0;

        for ( DWORD i = 0; i < pMeshContainerEx->NumInfl; i++ )
        {
            if ( pBoneCombinations[dwIndex].BoneId[i] != UINT_MAX )
            {
                dwInfl++;
            }
        }


        if ( dwInfl > d3dCap.MaxVertexBlendMatrices )	//执行顶点混合时，设备可以支持的矩阵复合的最大数量
        {
            break;
        }
    }

    if ( dwIndex < pMeshContainerEx->NumAttributeGroups )
    {
        ID3DXMesh *pMeshTmp = NULL;

        hr = pMeshContainerEx->MeshData.pMesh->CloneMeshFVF
             (
                 pMeshContainerEx->MeshData.pMesh->GetOptions() | D3DXMESH_SOFTWAREPROCESSING,
                 pMeshContainerEx->MeshData.pMesh->GetFVF(),
                 D9DEVICE->GetDvc(), &pMeshTmp
             );

        if ( FAILED( hr ) )
        {
            return hr;
        }

        pMeshContainerEx->MeshData.pMesh->Release();
        pMeshContainerEx->MeshData.pMesh = pMeshTmp;

        pMeshTmp = NULL;

    }

    return S_OK;

}

CAllocateHierarchy::~CAllocateHierarchy()
{

}

STDMETHODIMP CAllocateHierarchy::DestroyFrame( LPD3DXFRAME pFrameToFree )
{
    return S_OK;
}
