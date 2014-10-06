#include "GGameDemoHeader.h"
#include "GMeshBaseObj.h"
#include "GGameMap.h"
#include "GComponentTrans.h"
#include "XEffect.h"

#include "GSceneMgr.h"
#include "GGame.h"

GMeshBaseObj::GMeshBaseObj ( void )
    : mMeshBufferNode ( 0 )
{
}

GMeshBaseObj::~GMeshBaseObj ( void )
{

}

bool GMeshBaseObj::Create()
{
    return __super::Create();
}


bool GMeshBaseObj::Render()
{
    if ( !__super::Render() )
        return false;

    //if ( mMeshBufferNode )
    //    return mMeshBufferNode->Render();
	DWORD cnt=mMeshResource.size();
	for (CXIndex i=0;i<cnt;++i)
	{
		GGraph* renderData=mMeshResource[i];
		renderData->draw();
	}
    return true;
    //D9DEVICE->OpenAllLightEx(m_bLightOn,mfDiffuseIntensity,ZEROFLOAT,m_bUseMatrialColor);

    //DWORD dwAmbient = 0;
    //D9DEVICE->GetDvc()->GetRenderState( D3DRS_AMBIENT, &dwAmbient );
    ////���Ʋ�͸������

    //gEffect.mD3DEffect->SetBool( gEffect.m_hOpenLight, ( BOOL )mLighting );
    //gEffect.mD3DEffect->SetBool( gEffect.m_hUseMaterialOnly, ( BOOL )m_bUseMatrialColor );

    //UINT iPass = 0;
    //gEffect.mD3DEffect->Begin( &iPass, 0 );


    //gEffect.mD3DEffect->SetTechnique( gEffect.m_Tech );


    //D3DXMATRIX matWorldViewProj = _matWorld * TheSceneMgr->mEye.matView * TheSceneMgr->mEye.matProj;

    //gEffect.mD3DEffect->SetMatrix( gEffect.m_hWorldViewProj, &matWorldViewProj );

    //gEffect.mD3DEffect->SetMatrix( gEffect.m_hWorld, &_matWorld );

    //for ( DWORD i = 0; i < mMeshBufferNode->SubSetCount(); i++ )
    //{
    //    if ( !( mMeshBufferNode->mpMat[i].Diffuse.a < 1.0f ) )
    //    {
    //        //��������ģ���Ӽ��Ĳ��ʺ�����������Ⱦ

    //        D3DMATERIAL9 *pTmp = &mMeshBufferNode->mpMat[i];

    //        D3DXVECTOR4 vDiffuse( pTmp->Diffuse.r, pTmp->Diffuse.g, pTmp->Diffuse.b, pTmp->Diffuse.a );
    //        D3DXVECTOR4 vAmbient( pTmp->Ambient.r, pTmp->Ambient.g, pTmp->Ambient.b, pTmp->Ambient.a );

    //        gEffect.mD3DEffect->SetVector( gEffect.m_hMtrlDiffuse, &vDiffuse );
    //        gEffect.mD3DEffect->SetVector( gEffect.m_hMtrlAmbient, &vAmbient );

    //        if ( ppFace != NULL )
    //        {
    //            //Alpha����:

    //            D9DEVICE->GetDvc()->SetRenderState( D3DRS_ALPHATESTENABLE, true );

    //            D9DEVICE->GetDvc()->SetRenderState( D3DRS_ALPHAREF, 0x00000000 );

    //            D9DEVICE->GetDvc()->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );

    //            if ( mChangeWhenHit )
    //            {
    //                D9DEVICE->GetDvc()->SetTexture( 0, NULL );
    //            }
    //            else
    //            {
    //                gEffect.mD3DEffect->SetTexture( gEffect.m_hTexture, ppFace[i] );
    //            }
    //        }

    //        gEffect.mD3DEffect->BeginPass( 0 );

    //        mMeshBufferNode->Mesh()->DrawSubset( i );

    //        gEffect.mD3DEffect->EndPass();

    //    }
    //    else
    //    {
    //        continue;
    //    }
    //}

    ////����͸������
    //D9DEVICE->OpenAlphaBlend( true );

    //for ( DWORD i = 0; i < mMeshBufferNode->LnAttrNum; i++ )
    //{
    //    if ( mMeshBufferNode->mpMat[i].Diffuse.a < 1.0f )
    //    {
    //        //D9DEVICE->GetDvc()->SetMaterial(&mpMeshBufferNode->mpMat[i]);

    //        D3DMATERIAL9 *pTmp = &mMeshBufferNode->mpMat[i];

    //        D3DXVECTOR4 vDiffuse( pTmp->Diffuse.r, pTmp->Diffuse.g, pTmp->Diffuse.b, pTmp->Diffuse.a );
    //        D3DXVECTOR4 vAmbient( pTmp->Ambient.r, pTmp->Ambient.g, pTmp->Ambient.b, pTmp->Ambient.a );

    //        gEffect.mD3DEffect->SetVector( gEffect.m_hMtrlDiffuse, &vDiffuse );
    //        gEffect.mD3DEffect->SetVector( gEffect.m_hMtrlAmbient, &vAmbient );

    //        if ( ppFace != NULL )
    //        {
    //            if ( mChangeWhenHit )
    //            {
    //                D9DEVICE->GetDvc()->SetTexture( 0, NULL );
    //            }
    //            else
    //            {
    //                //D9DEVICE->GetDvc()->SetTexture(0,ppFace[i]);
    //                gEffect.mD3DEffect->SetTexture( gEffect.m_hTexture, ppFace[i] );
    //            }
    //        }


    //        gEffect.mD3DEffect->BeginPass( 0 );

    //        //mpMeshBufferNode->mMeshNodeForLod.pMesh[LnLodNowLevel]->DrawSubset(i);
    //        mMeshBufferNode->mpRootMesh->DrawSubset( i );

    //        gEffect.mD3DEffect->EndPass();

    //    }
    //    else
    //    {
    //        continue;
    //    }
    //}

    //gEffect.mD3DEffect->End();

    //D9DEVICE->GetDvc()->SetRenderState( D3DRS_ALPHATESTENABLE, false );

    //D9DEVICE->OpenAlphaBlend( false );

    //D9DEVICE->OpenAllLight( false );
}




ID3DXMesh * GMeshBaseObj::ResetVectorMesh()
{

    SAFERELEASE ( mpMeshForVector );

    if ( mMeshBufferNode->Mesh() != NULL )
    {
        mMeshBufferNode->Mesh()->CloneMeshFVF (
            mMeshBufferNode->Mesh()->GetOptions(),
            D3DFVF_XYZ, Device->GetDvc(), &mpMeshForVector
        );
    }

    return mpMeshForVector;
}


bool GMeshBaseObj::CheckIntersect ( const D3DXVECTOR4& vPos, const D3DXVECTOR4& vDir, bool bInsectInfo )
{
    HRESULT hr = S_FALSE;

    //��Pos��Dirת�������屾������ϵ��
    D3DXMATRIX matWorld = GetWorldMatrix ( false );
    D3DXMatrixInverse ( &matWorld, NULL, &matWorld );

    D3DXVec4Transform ( ( D3DXVECTOR4* ) &vDir, &vDir, &matWorld );
    D3DXVec3Normalize ( ( D3DXVECTOR3* ) &vDir, ( D3DXVECTOR3* ) &vDir );
    D3DXVec4Transform ( ( D3DXVECTOR4* ) &vPos, &vPos, &matWorld );

    if ( mpMeshForVector == NULL )
    {
        ResetVectorMesh();
    }

    BOOL bHit = FALSE;

    hr = D3DXIntersect (
             mpMeshForVector, ( D3DXVECTOR3* ) &vPos, ( D3DXVECTOR3* ) &vDir, &bHit,
             &m_InsectInfo.dwFaceIndex, &m_InsectInfo.u, &m_InsectInfo.v, &m_InsectInfo.fDist,
             NULL, NULL
         );

    m_bHit = ( bool ) bHit;

    DebugMsgBox ( hr, "��ײʧ�ܣ�" );

    if ( FAILED ( hr ) )
    {
        return false;
    }

    if ( bInsectInfo && m_bHit )
    {
        D3DXVECTOR3 v[3];
        DWORD dwIndex[3];

        //��Ҫ��ȡ������������ʽ
        LPDIRECT3DINDEXBUFFER9 pI = NULL;
        mpMeshForVector->GetIndexBuffer ( &pI );

        D3DINDEXBUFFER_DESC indexDesc;
        ZeroMemory ( &indexDesc, sizeof ( D3DINDEXBUFFER_DESC ) );

        if ( pI != NULL )
        {
            pI->GetDesc ( &indexDesc );
        }

        if ( indexDesc.Format == D3DFMT_INDEX16 )
        {
            WORD *pIndexData16;

            hr = mpMeshForVector->LockIndexBuffer ( D3DLOCK_READONLY, ( void** ) &pIndexData16 );

            dwIndex[0] = pIndexData16[m_InsectInfo.dwFaceIndex * 3 + 0];
            dwIndex[1] = pIndexData16[m_InsectInfo.dwFaceIndex * 3 + 1];
            dwIndex[2] = pIndexData16[m_InsectInfo.dwFaceIndex * 3 + 2];
        }
        else
        {
            DWORD *pIndexData32;

            hr = mpMeshForVector->LockIndexBuffer ( D3DLOCK_READONLY, ( void** ) &pIndexData32 );

            dwIndex[0] = pIndexData32[m_InsectInfo.dwFaceIndex * 3 + 0];
            dwIndex[1] = pIndexData32[m_InsectInfo.dwFaceIndex * 3 + 1];
            dwIndex[2] = pIndexData32[m_InsectInfo.dwFaceIndex * 3 + 2];
        }


        mpMeshForVector->UnlockIndexBuffer();

        D3DXVECTOR3 *pVertexData;

        hr = mpMeshForVector->LockVertexBuffer ( D3DLOCK_READONLY, ( void** ) &pVertexData );

        v[0] = pVertexData[dwIndex[0]];
        v[1] = pVertexData[dwIndex[1]];
        v[2] = pVertexData[dwIndex[2]];

        mpMeshForVector->UnlockVertexBuffer();

        D3DXVECTOR4 vNormal ( ZEROFLOAT, ZEROFLOAT, ZEROFLOAT, ZEROFLOAT );
        D3DXVECTOR4 vHitPos ( ZEROFLOAT, ZEROFLOAT, ZEROFLOAT, ZEROFLOAT );

        D3DXVECTOR3 vTmp1, vTmp2;
        vTmp1 = v[1] - v[0];
        vTmp2 = v[2] - v[0];

        vHitPos = ( D3DXVECTOR4 ) v[0] + m_InsectInfo.u * ( D3DXVECTOR4 ) vTmp1 + m_InsectInfo.v * ( D3DXVECTOR4 ) vTmp2;
        vHitPos.w = 1;

        matWorld = GetWorldMatrix ( false );
        D3DXVec4Transform ( &vHitPos, &vHitPos, &matWorld );
        m_InsectInfo.vHitPos = D3DXVECTOR3 ( vHitPos.x, vHitPos.y, vHitPos.z );

        D3DXVec3Cross ( ( D3DXVECTOR3* ) &vNormal, &vTmp1, &vTmp2 );
        vNormal.w = 0;


        D3DXVec4Transform ( &vNormal, &vNormal, &matWorld );
        D3DXVec3Normalize ( ( D3DXVECTOR3* ) &vNormal, ( D3DXVECTOR3* ) &vNormal );

        m_InsectInfo.vNormal = D3DXVECTOR3 ( vNormal.x, vNormal.y, vNormal.z );

    }

    return m_bHit;

}

bool GMeshBaseObj::Pick ( POINT pt )
{

    D3DXMATRIX	matView, matProj;

    Device->GetDvc()->GetTransform ( D3DTS_VIEW, &matView );
    Device->GetDvc()->GetTransform ( D3DTS_PROJECTION, &matProj );

    ////���ߵ����Ϊ�۾�������ͼ�ռ���Ϊ����ԭ�㣨0,0,0����
    D3DXVECTOR4 vOrigin ( 0, 0, 0, 1 );

    ////���߷���Ϊ�۾��������ĳ���
    D3DXVECTOR4 vDir;

    //�����λ�ô�2Dƽ��ת����3D����ͼ�ռ���
    vDir.x = ( ( ( 2.0f * pt.x ) / Device->mWidth ) - 1 ) / matProj._11;
    vDir.y = - ( ( ( 2.0f * pt.y ) / Device->mHeight ) - 1 ) / matProj._22;
    vDir.z =  1.0f;
    vDir.w =  0.0f;

    D3DXMatrixInverse ( &matView, NULL, &matView );

    //��vDir��vPosת������������ϵ��
    D3DXVec4Transform ( &vDir, &vDir, &matView );
    D3DXVec4Normalize ( &vDir, &vDir );

    D3DXVec4Transform ( &vOrigin, &vOrigin, &matView );

    return CheckIntersect ( vOrigin, vDir, false );

}



void GMeshBaseObj::RegisterAll()
{
    __super::RegisterAll();
}





