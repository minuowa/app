#pragma once
#include "GComponentInterface.h"
#include "GFactory.h"


//��ײ��Ϣ
struct IntersectInfo
{
    DWORD dwFaceIndex;
    float u, v;
    float fDist;
    D3DXVECTOR3 vNormal;		//�Ѿ�ת����������
    D3DXVECTOR3 vHitPos;		//�Ѿ�ת���������꣨����ռ�����꣩
    void *pObj;					//��ײ�����ָ��

    IntersectInfo();

};
//-------------------------------------------------------------------------
/**
	@brief GComponentTrans
	@author nnboy,17/9/2014  22:34
*/
class GComponentTrans: public GComponentBase<eComponentType_Trans>
{
    DeclareComponentType ( GComponentTrans )
public:
    GComponentTrans ( void );
    ~GComponentTrans ( void );
public:
    virtual void RegisterAll();
public:
    D3DXMATRIX GetWorldMatrix ( bool bForTrans = false );

    D3DXMATRIX GetRotationMatrix ( bool bForTrans = false );

    D3DXMATRIX GetScaleMatrix ( bool bForTrans = false );

    D3DXMATRIX GetTransLation ( bool bForTrans = false );

    D3DXVECTOR3 MoveStep ( float fPass );
    D3DXVECTOR3 TrunStepLeftRight ( float fPara );
    D3DXVECTOR3 TrunStepLeftRightWithUp ( float fpara );
    D3DXVECTOR3 TrunWithRight ( float fPara );

    int Jump();

    D3DXVECTOR3 MoveToPos ( D3DXVECTOR3 vTarget );

    D3DXVECTOR3 TrunToDir ( D3DXVECTOR3 vTargetDir );

    D3DXVECTOR3 SetDirWithUpon ( D3DXVECTOR3 vUpon );

    void Update();

private:

    D3DXVECTOR3 AutoMoveTo ( float fPass );

    virtual const char* GetComponentName();

public:

    D3DXVECTOR3 mvLastPos;
    //λ��
    D3DXVECTOR3 mTranslate;

    //�����ҷ����Ϸ���
    D3DXVECTOR3 mvDir;
    D3DXVECTOR3 mvRight;
    D3DXVECTOR3 mvUpon;

    //����ϵ��
    D3DXVECTOR3 mvZoom;

    //�������������ԭ�����ת�Ƕ�
    D3DXVECTOR3 mvBodyPass;
    D3DXVECTOR3 mBodyRote;

    //�ٶ�
    D3DXVECTOR3 mvSpeed;
    float mfSpeedMove;
    float mfSpeedTrun;

    D3DXVECTOR3 mvTargetPos;

    D3DXVECTOR3 mvTargetDir;

    bool mbAutoMoveWithTarget;

    bool mbAutoTrunWithTarget;

    bool mbAutoMove;

    bool mbJump;

    //IntersectInfo mInsectMapInfo;



    bool mbCanMoveStep;					//�Ƿ���Բ����ƶ�

    //bool mbIsEye;						//�Ƿ����۾����۾�������ǿ���߼�

    bool mbBack;						//�Ƿ��������
};
