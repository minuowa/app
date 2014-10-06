#pragma once

//�����࣬Ϊ����ϵͳ��һ����Ա

class CXArea
{

    public:

        CXArea( void );

        ~CXArea( void );

        void Init();

    public:
        virtual bool IsDotInArea( D3DXVECTOR3 vPos );					//���ڸ�������Ϊ�棬����Ϊ��

        virtual D3DXVECTOR3 GeneratePos();
};

class CAreaRect: public CXArea
{
    public:

        CAreaRect();

        ~CAreaRect();

    public:

        void Init( float Height, float Width );

        bool IsDotInArea( D3DXVECTOR3 vPos );

        D3DXVECTOR3 GeneratePos();

    public:

        float m_fHeight;
        float m_fWidth;



};

class CAreaCube: public CXArea
{

    public:

        CAreaCube();

        ~CAreaCube();

        void Init( float xmin, float xmax, float ymin, float ymax, float zmin, float zmax );

    public:

        virtual bool IsDotInArea( D3DXVECTOR3 vPos );

        virtual D3DXVECTOR3 GeneratePos();

    public:

        D3DXPLANE mPlanes[6];

        float mfxMin, mfxMax;

        float mfyMin, mfyMax;

        float mfzMin, mfzMax;



};

class CAreaSphere : public CXArea
{

    public:

        CAreaSphere();

        ~CAreaSphere();

    public:

        void Init( int nRadius );

        virtual bool IsDotInArea( D3DXVECTOR3 vPos );

        virtual D3DXVECTOR3 GeneratePos();

    public:

        int LnRadius;

};

class CAreaSphereHollow: public CXArea
{

    public:

        CAreaSphereHollow();

        ~CAreaSphereHollow();

    public:

        void Init( int nRadius );

        virtual bool IsDotInArea( D3DXVECTOR3 vPos );

        virtual D3DXVECTOR3 GeneratePos();

    public:

        int LnRadius;

};