#include "GGameDemoHeader.h"
#include "EyeCliper.h"

CEyeCliper::CEyeCliper(void)
{

}

CEyeCliper::CEyeCliper( float fn,float angle, float wh )
{
	float y=fn*tanf(angle);

	float x=y*wh;
	//����ʾ��ͼ
	//1.........2
	//4.........3
	mVertex[0]=ZEROVECTOR3;
	mVertex[1]=D3DXVECTOR3(-x,y,fn);
	mVertex[2]=D3DXVECTOR3(x,y,fn);
	mVertex[3]=D3DXVECTOR3(x,-y,fn);
	mVertex[4]=D3DXVECTOR3(-x,-y,fn);

}

CEyeCliper::~CEyeCliper(void)
{

}

void CEyeCliper::Update( D3DXMATRIX *pMat )
{
	D3DXVECTOR3 Vec[5];

	for (int i=0;i<5;i++)
	{
		D3DXVec3TransformCoord(&Vec[i],&mVertex[i],pMat);
	}

	//����ʾ��ͼ
	//1.........2
	//4.........3
	//����
	D3DXPlaneFromPoints(&mPlane[0],&Vec[2],&Vec[1],&Vec[0]);

	//����
	D3DXPlaneFromPoints(&mPlane[1],&Vec[0],&Vec[4],&Vec[3]);

	//����
	D3DXPlaneFromPoints(&mPlane[2],&Vec[0],&Vec[1],&Vec[4]);

	//����
	D3DXPlaneFromPoints(&mPlane[3],&Vec[0],&Vec[3],&Vec[2]);

	//��Զ��
	D3DXPlaneFromPoints(&mPlane[4],&Vec[1],&Vec[2],&Vec[3]);
}

bool CEyeCliper::IsInEye( D3DXVECTOR3 vPos )
{

	for (int i=0;i<5;i++)
	{
		if(D3DXPlaneDotCoord(&mPlane[i],&vPos)<-50.0f)
		{
			return false;
		}
	}

	return true;
}
