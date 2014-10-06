#pragma once

#include "GComponentTrans.h"
#include "GObject.h"
#include "GFactory.h"

//���������Ե�����
class GNode: public GObject
{
    DeclareEditorType ( GNode );
public:

    GNode();

    virtual ~GNode();
    void SetNodeName ( CChar* name );

    void LinkTo ( CXRapidxmlNode* parent );
    GNode* GetNodeByName ( const char* name );
    virtual void Clear();
    virtual bool ReCreate();
public:
    virtual void RegisterAll();
    DWORD GetObjCount();					//��ȡ���д��������������������Ѿ����ٵĶ���
    bool Create( );
    void SetParentBone ( GNode *Parent, char *sName );
    int GetObjID() const;
    GNode* AddChild ( GNode* c );
    bool	RemoveChild ( GNode* child );

    virtual bool Draw();
    virtual void GetInput ( DWORD frameTimeMs );
    virtual void Update();


    //������������һ�£��õ������ڵ�ͼ�ϵ�λ��
    IntersectInfo *UpdateForForceOnMap();

    IntersectInfo *UpdateForForceOnObj ( void *pObj );
    D3DXMATRIX GetWorldMatrix ( bool bForTrans );
    virtual eObjAnimState SetState ( eObjAnimState oas, bool bBack ) ;
    virtual D3DXMATRIX GetWorldMatrixByBone ( char *sBoneName, bool bForTrans = false );

    void ForceOnMap ( void *pMap, float fForceHeight, eForceType ft );

    IntersectInfo *GetBlockPoint();

    void SetDir ( D3DXVECTOR3 vNormal );

    GComponentTrans& GetTrans() const;
    void UpdateTrans();
    void OnComponentChange ( GComponentInterface* component, bool canDetach, bool notifyEditor );
    virtual void OnPropertyChange ( void* pre, void* changed );
protected:
    virtual void BeginRender();
    virtual void EndRender();
    virtual bool Render();
    void	MakeXMLNode ( CXRapidxmlNode& node );

public:

    CXDynaArray<GNode*> mChildren;

    GComponentOwner	mComponentOwner;

    //eHitType m_HitType;					//�Ƿ�ײ������

    void *m_pOnObj;						//����������ķǵ�ͼ����

    bool m_bEyeCliper;					//�Ƿ���Ҫ�۾��ü�

    bool m_bForceOnMap;					//�Ƿ������ڵ�ͼ

    float m_fForceHeight;				//�����ڵ�ͼ�ϵĸ߶�

    void *mForceMap;					//�����ĵ�ͼ��ָ��

    eForceType m_ForceType;				//�����ڵ�ͼ�ϵ�����

    IntersectInfo m_InsectInfo;			//��ײ��Ϣ��ע�⣺�ڳ����е���һһ���ط���������ײ�����ܸı���ֵ



    float m_fBlockArea;					//���������������뾶��С

    float m_fBlockHeight;				//������������߶�


    DWORD mObjState;					//������Ⱦ״̬

    bool m_bBehaviour;					//�Ƿ������������û�в���������LastPos��Pos�������ͼ����ײ������׼ȷ

    bool m_bCanSelect;					//�Ƿ��ѡ�񲢿���

    bool m_bBeSelected;					//��ѡ����

    bool m_bUseMatrialColor;

    D3DXVECTOR3 m_vBlockPoint;

    bool m_bHit;

    bool m_bBlock;

    bool mCanGetInput;
    bool mDynamic;

    float m_fBoundRadius;

protected:

    eObjAnimState m_ObjAnimState;		//������������ǰ״̬

    D3DXMATRIX _matWorld;				//������������

public:

    eObjParentType mOpt;

    char msParentName[32];

    GNode *mParent;							//�����Ķ���

protected:

    DWORD _nID;								//���������еĴ����������еľ���ID

private:

    static DWORD __OBJCOUNTER;				//������Ŀ����

public:

    inline GComponentInterface* AttachComponent ( eComponentType type, bool canDetach = true, bool notifyEditor = true )
    {
        GComponentInterface* component = mComponentOwner.AttachComponent ( type );
        OnComponentChange ( component, canDetach , notifyEditor );
        return component;
    }
    inline GComponentInterface* AttachComponent ( const char* name, bool canDetach = true, bool notifyEditor = true )
    {
        GComponentInterface* component = mComponentOwner.AttachComponent ( name );
        OnComponentChange ( component, canDetach, notifyEditor );
        return component;
    }
    inline void DetachComponent ( const char* name )
    {
        mComponentOwner.DetachComponent ( name );
        OnComponentChange ( 0, false, true );
    }
    inline GComponentOwner& GetComponentOwner()
    {
        return mComponentOwner;
    }
	inline GComponentInterface* GetComponent(eComponentType type) const
	{
		return mComponentOwner.GetComponent(type);
	}
};

typedef CXDynaArray<GNode*> GNodeArr;
