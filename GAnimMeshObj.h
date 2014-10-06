#pragma once
#include "GAmmo.h"
#include "XString.h"
#include "GRenderObject.h"

class GAnimationResource;

class GAnimMeshObj: public GRenderObject
{
	DeclareEditorType(GAnimMeshObj);

    public:
        GAnimMeshObj( void );
        ~GAnimMeshObj( void );
    public:
		virtual bool ReCreate();
		virtual void Update();
		void setMeshFile(const char* file);
        VOID	UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );

        void	DrawMeshContainer( D3DXMESHCONTAINER *pMeshContainerBase, D3DXFRAME *pFrameBase );

        void	DrawFrame( D3DXFRAME *pFrameBase );

        virtual eObjAnimState SetState( eObjAnimState os, bool bBack );

		virtual bool Render();

        bool	Pick( ID3DXMesh *pMesh, POINT pt );


        virtual D3DXMATRIX GetWorldMatrixByBone( char *sBoneName, bool bForTrans = false );
    public:
        char m_sTextureName[FILE_NAME_LENGTH];
    private:
        LPD3DXANIMATIONSET mpAnimSet;						//��ǰ������
        bool mbPlayDone;									//��һ���������Ƿ񲥷����
        DWORD mdwOldAnimSetFrame;							//��ǰ��������һ�β��ŵ���֡��
        DWORD mdwCurAnimSetFrame;							//��ǰ�������˴β��ŵ���֡��
        int LnNowBoneInfoCount;

    private:
		GAnimationResource*	mResource;
		ID3DXAnimationController *mCloneAnimationController;			//����������
        IDirect3DTexture9* mpFace;								//����
    public:
        CAmmoParticles *mpAmmo;										//�ڵ�����ָ��
public:
	inline GAnimationResource* GetResource() const
	{
		return mResource;
	}
};

/***************************************************************************

ÿ��Frame�����һ��MeshContainer��

ÿ��Meshcontainer��NumAttributeGroup��MeshSubSet��

ÿ��MeshSubSet��NumInfl��������Ӱ�죬

***************************************************************************/