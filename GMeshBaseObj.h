#pragma once
#include "GBound.h"
#include "GRenderObject.h"
#include "GMeshResource.h"

class GMeshBufferNode;
class GMeshBaseObj :
    public GRenderObject
{
	DeclareEditorType(GMeshBaseObj);
    public:
        GMeshBaseObj( void );
        ~GMeshBaseObj( void );

		virtual void RegisterAll();

		bool Render();

        ID3DXMesh *GetMeshByUsage( eMeshUsage mu );

        bool Pick( POINT pt );

        bool CheckIntersect(
            const D3DXVECTOR4& vPos,	//��������ϵ�еĵ�
            const D3DXVECTOR4& vDir,	//��������ϵ�е�����
            bool bInsectInfo	//�Ƿ���Ҫ��ײ��Ϣ
        );
		ID3DXMesh* ResetVectorMesh();
protected:
        //��Ⱦ�������
        GMeshBufferNode* mMeshBufferNode;
		ID3DXMesh*	mpMeshForVector;
public:

		bool Create();

		GMeshResource mMeshResource;
};
