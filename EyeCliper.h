#pragma once



class CEyeCliper
{
	/********************************************************************

	1,在眼睛和最近面之间的对象可以不做剔除，所以可以简化为一个视锥

	2，只计算最远面的四个顶点，减少了算法的复杂度

	********************************************************************/

public:

	CEyeCliper(void);

	~CEyeCliper(void);

	CEyeCliper(float fn,float angle, float wh);

	D3DXVECTOR3 mVertex[5];

	D3DXPLANE mPlane[5];

public:

	bool IsInEye(D3DXVECTOR3 vPos);

	void Update( D3DXMATRIX *pMat );

};
