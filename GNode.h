#pragma once

#include "GComponentTrans.h"
#include "GD9Device.h"
#include "GD8Input.h"
#include "GObject.h"
#include "GFactory.h"

//有坐标属性的物体
class GNode: public GObject
{
    DeclareEditorType ( GNode );
public:

    GNode();

    virtual ~GNode();
    void SetNodeName ( CChar* name );

    void LinkTo ( CXRapidxmlNode* parent );
    GNode* GetNodeByName ( const char* name );
public:
    virtual void RegisterAll();
    DWORD GetObjCount();					//获取所有创建的物体总数，包括已经销毁的对象
    bool Create( );
    void SetParentBone ( GNode *Parent, char *sName );
    int GetObjID() const;
    GNode* AddChild ( GNode* c );
    bool	RemoveChild ( GNode* child );

    virtual bool Draw();
    virtual void GetInput ( DWORD frameTimeMs );
    virtual void Update();


    //创建物体后更新一下，得到物体在地图上的位置
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
	void OnComponentChange();
protected:
    virtual void BeginRender();
    virtual void EndRender();
    virtual bool Render();
    void	MakeXMLNode ( CXRapidxmlNode& node );

public:

    CXDynaArray<GNode*> mChildren;

    GComponentOwner	mComponentOwner;

    //eHitType m_HitType;					//是否撞到物体

    void *m_pOnObj;						//本对象下面的非地图对象

    bool m_bEyeCliper;					//是否需要眼睛裁剪

    bool m_bForceOnMap;					//是否依附于地图

    float m_fForceHeight;				//依附于地图上的高度

    void *mForceMap;					//依附的地图的指针

    eForceType m_ForceType;				//依附于地图上的类型

    IntersectInfo m_InsectInfo;			//碰撞信息，注意：在程序中的任一一个地方做射线碰撞都可能改变其值



    float m_fBlockArea;					//该物体的阻塞区域半径大小

    float m_fBlockHeight;				//该物体的阻塞高度


    DWORD mObjState;					//物体渲染状态

    bool m_bBehaviour;					//是否产生过动作，没有产生过动作LastPos和Pos不能与地图做碰撞，否则不准确

    bool m_bCanSelect;					//是否可选择并控制

    bool m_bBeSelected;					//被选中了

    bool m_bUseMatrialColor;

    D3DXVECTOR3 m_vBlockPoint;

    bool m_bHit;

    bool m_bBlock;

    bool mCanGetInput;
    bool mDynamic;

    float m_fBoundRadius;

protected:

    eObjAnimState m_ObjAnimState;		//骨骼动画对象当前状态

    D3DXMATRIX _matWorld;				//物体的世界矩阵

public:

    eObjParentType mOpt;

    char msParentName[32];

    GNode *mParent;							//依附的对象

protected:

    DWORD _nID;								//物体在所有的创建的物体中的绝对ID

private:

    static DWORD __OBJCOUNTER;				//物体数目计数

public:
    inline const CChar* GetEditName() const
    {
        return mNodeName.c_str();
    }
	inline GComponentInterface* AttachComponent ( eComponentType type )
	{
		GComponentInterface* component=mComponentOwner.AttachComponent ( type );
		OnComponentChange();
		return component;
	}
	inline GComponentInterface* AttachComponent ( const char* name )
    {
		GComponentInterface* component=mComponentOwner.AttachComponent ( name );
		OnComponentChange();
		return component;
    }
    inline void DetachComponent ( const char* name )
    {
        mComponentOwner.DetachComponent ( name );
		OnComponentChange();
    }
    inline GComponentOwner& GetComponentOwner()
    {
        return mComponentOwner;
    }
};

typedef CXDynaArray<GNode*> GNodeArr;
