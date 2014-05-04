#pragma once
#include "GBound.h"
#include "GRenderObject.h"

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
            D3DXVECTOR4 vPos,	//��������ϵ�еĵ�
            D3DXVECTOR4 vDir,	//��������ϵ�е�����
            bool bInsectInfo	//�Ƿ���Ҫ��ײ��Ϣ
        );
		ID3DXMesh* ResetVectorMesh();
protected:
        //��Ⱦ�������
        GMeshBufferNode* mMeshBufferNode;
		ID3DXMesh*	mpMeshForVector;
		CharString mMeshFile;
public:
		CharString MeshFile() const { return mMeshFile; }
		void MeshFile(CharString val) { mMeshFile = val; }
		bool Create();
};