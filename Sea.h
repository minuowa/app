#pragma once

#include "GMeshBaseObj.h"


#define SEAPICNUM   64	//��ˮ����ͼƬ�ĸ���

#define FaceSwitchTime 40 //ÿ���л�5������

struct SeaVertex
{
    D3DXVECTOR3 vertex;	    //x,y,z
    float nx, ny, nz;	  //���ߣ�x��y��z
    float u, v;		   //��������
};

struct QuakePoint
{
        float mInitTime;	//��ʼʱ��
        float mAmplitude;	//���
        float mAngVelocity;	//���ٶȣ�=f*2*PI
        float mfX, mfZ;		//��Դ��λ��
    public:
        float GetPointEffect( float _x, float _z, float _t );
        void Init( float _x, float _z, float amplitude, float AngVelocity, float _t );
};

#define FVFSea D3DFVF_XYZ | D3DFVF_TEX1|D3DFVF_NORMAL
class GTexture;
class CSea : public GMeshBaseObj
{
    public:
        CSea( void );
        ~CSea( void );



		virtual bool Create( );

        void Update( float fpass );

        int AddQuakePoint( float _x, float _z, float fAmplitude, float fAngVelocity );
public:
	MeshPara* CreateParam() const;
	void CreateParam(const MeshPara& val);
    private:
        float mfCellWidth;	    //��ͼ�и��ӿ��
        float mfMaxHeight;	    //��ͼ�����߶�
        int	  mCellCount;	    //��ͼ��ÿ�и�������
        char* mstrFileName;    //�����ļ����֣�X�ļ�����
        char* mstrHeightMap;   //�߶�ͼ�ļ�����

		MeshPara*	mCreateParam;

		GTexture* mpFace;
        //LPDIRECT3DTEXTURE9 *mpFace;

        QuakePoint mQuakList[MAX_QUAKE_COUNT];

        int LnQuakeCount;

};
