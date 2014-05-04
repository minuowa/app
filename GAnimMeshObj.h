#pragma once
#include "GNode.h"
#include "GAmmo.h"
#include "XString.h"

struct FiLinker			//悬挂物品用的骨骼信息
{
    CharString Name;

    D3DXMATRIX* Transform;

};

struct D3DXFrameEX : public D3DXFRAME	//扩展的D3D框架
{
    D3DXMATRIX matCombinedTransformation;

    //组合变换矩阵；

    //(遍历父框架的Transformation:T1、T2、...，得到组合矩阵ParentMat=T1*T2*...，matCombined=matWorld*ParantMat*Transform)
};

struct D3DXMeshContainerEX: public D3DXMESHCONTAINER		//扩展的D3D网格容器
{
    /******************************************************************/

    //在CreateMeshContainer中取值的成员
    //Name;MeshData;pMaterials;NumMaterials;pSkinInfo

    ID3DXMesh *pOriginMesh;				//骨骼模型原有Mesh

    IDirect3DTexture9 **ppTexture;		//纹理组

    D3DXMATRIX *pBoneOffsetMatrices;	//初始偏移矩阵(相对于本地空间的)

    /******************************************************************/

    D3DXMATRIX**  ppBoneMatrixPtrs;		//在SetupBoneMatixPointer过程中设置，*ppBoneMatrixPtrs指向pFrameEx->matCombinedTransformation

    /******************************************************************/
    //在GenerateSkinnedMesh中取值的成员
    ID3DXBuffer *pBoneCombinationBufffer;//指向D3DXBONECOMBINATION数组的指针，可以获取BoneID

    DWORD NumInfl;						//影响每个需要实施蒙皮的顶点的骨骼最大数量

    DWORD NumAttributeGroups;			//骨骼混合表中SubMesh的数量

};


class CAllocateHierarchy: public ID3DXAllocateHierarchy	//扩展D3D分配层次类
{
    public:

        CAllocateHierarchy( IDirect3DDevice9 *DVC );

        ~CAllocateHierarchy();

    public:

        STDMETHOD( CreateFrame )( LPCSTR Name, LPD3DXFRAME *ppNewFrame );

        STDMETHOD( CreateMeshContainer )(
            LPCSTR Name,
            CONST D3DXMESHDATA *pMeshData,
            CONST D3DXMATERIAL *pMaterials,
            CONST D3DXEFFECTINSTANCE *pEffectInstances,
            DWORD NumMaterials,
            CONST DWORD *pAdjacency,
            LPD3DXSKININFO pSkinInfo,
            LPD3DXMESHCONTAINER *ppNewMeshContainer ) ;

        STDMETHOD( DestroyFrame )( LPD3DXFRAME pFrameToFree ) ;

        STDMETHOD( DestroyMeshContainer )( LPD3DXMESHCONTAINER pMeshContainerToFree ) ;

    private:

        HRESULT GenerateSkinnedMesh( D3DXMeshContainerEX *pMeshContainerEx );

};


typedef CXMap<CharString, FiLinker*> FiLinkerMap;

class GAnimMeshObj: public GRenderObject
{
	DeclareEditorType(GAnimMeshObj);

    public:

        GAnimMeshObj( void );

        ~GAnimMeshObj( void );

    public:

		bool Create( );

        int		AfterCreate();

        HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrame )  ;

        HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase );

        VOID	UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );

        void	DrawMeshContainer( D3DXMESHCONTAINER *pMeshContainerBase, D3DXFRAME *pFrameBase );

        void	DrawFrame( D3DXFRAME *pFrameBase );

        virtual eObjAnimState SetState( eObjAnimState os, bool bBack );

        int		Render( float fPass );

        bool	Pick( ID3DXMesh *pMesh, POINT pt );

        FiLinker *GetBoneInfo( CChar*sBoneName );

        virtual D3DXMATRIX GetWorldMatrixByBone( char *sBoneName, bool bForTrans = false );

    public:

        DWORD m_nAnimMeshID;

    private:

        int AddBoneInfo( CChar*sBoneName, D3DXMATRIX *pmat );

        char m_sXFileName[FILE_NAME_LENGTH];

        char m_sTextureName[FILE_NAME_LENGTH];

    private:

        LPD3DXANIMATIONSET mpAnimSet;						//当前动作集

        bool mbPlayDone;									//上一个动作集是否播放完成

        DWORD mdwOldAnimSetFrame;							//当前动画集上一次播放到的帧数

        DWORD mdwCurAnimSetFrame;							//当前动画集此次播放到的帧数

        FiLinkerMap mBoneInfos;

        int LnNowBoneInfoCount;

    private:

        D3DXFRAME *mpFrameRoot;									//根框架

        ID3DXAnimationController *mpAnimController;				//动画控制器

        IDirect3DTexture9* mpFace;								//纹理

    public:

        CAmmoParticles *mpAmmo;										//炮弹对象指针

};

/***************************************************************************

每个Frame最多有一个MeshContainer；

每个Meshcontainer有NumAttributeGroup个MeshSubSet；

每个MeshSubSet受NumInfl个骨骼的影响，

***************************************************************************/