#ifndef GGameDemoHeader_h__
#define GGameDemoHeader_h__
//--------------------------------------------------------------------------------------------------
//Windows����ͷ�ļ������ļ�
#include <stdio.h>
#include <stdlib.h>

#include <list>
#include <Windows.h>
#include <WinBase.h>
#include <tchar.h>
#include <MMSystem.h>
#include <process.h>
#include <shlwapi.h>
#include <vector>
#include <D3D9Types.h>

#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"Imm32.lib")
#pragma comment(lib,"Kernel32.lib")


#include <assert.h>

//Direct9����ͷ�ļ������ļ�

#include <dinput.h>
#include <d3d9.h>
#include <d3dx9math.h>
#include <d3d9types.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#ifdef _DEBUG
#pragma comment(lib,"d3dx9d.lib")
#else
#pragma comment(lib,"d3dx9.lib")
#endif

#include "XBase.h"


//////////////////////////�����ļ���Ҫ�ĺ�////////////////////////////////////

//���峣��

#define NORMALMATRIX D3DXMATRIX(1.0f,0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f)//��λ����

#define ZEROVECTOR3 D3DXVECTOR3(0.0f,0.0f,0.0f)		//������

#define ZEROFLOAT 0.0f								//float����ֵ

#define ERR_INT -1						//����ֵΪ���εĺ���������ʱ���ص�ֵ
#define TRUE_INT 1						//����ֵΪ���εĺ�������ȷʱ�ķ���ֵ

#define ERR_DEBUG   0xffffffff			//�����õģ��൱��һ������ֵhr

//���ô��ĺ�����

#define RAND rand()

#define SAFERELEASE(p) if(p!=NULL) { p->Release(); p=NULL;}

#define SAFED_ELETE(p) if(p!=NULL) { delete p; p=NULL;}

#define SAFE_DELETE_ARRAY(p) { if(p!=NULL) { delete[] (p);   (p)=NULL; } }

inline void Toggle ( bool b )
{
	b = !b;
}
template<typename T1, typename T2>
void CXCast ( T1& dst, T2* src )
{
	dst = * ( ( T1* ) src );
}

//��Ϸ�ж������

#define Default_Force_Height 1.0f				//����Ĭ���ڵ�ͼ�ϵĸ߶�

#define STATIC_MESH_MAX_NUM_ON_MAP 10					//��ͼ�Ͼ�̬ģ���������

#define MAP_MAX_NUM	999								//��ͼ���������
//#define STATICMESHCOUNT							//��Դ��ģ������

#define MESH_PATH    "..\\Data\\res\\StaticMesh\\"				//ģ���ļ���·��

#define FILE_NAME_LENGTH 256						//�ļ�������󳤶�
//��Ծ�߶�




//Ϊ��ʹ������Ծʱ�����ڿ���ͣ����ʱ������Ծ�����в�ʹ���������ٶ�

#define JUMP_HEIGHT	60.0f							//����������ܴﵽ�ĸ߶�
#define JUMP_TIME	1.0f							//����������ڿ���ͣ��ʱ���1/2

#define GRAVITY (2*JUMP_HEIGHT/JUMP_TIME/JUMP_TIME)	//�������ٶ�

#define LOD_MAX_LEVEL 5							//���LOD�ȼ�

#define WINDOW_WIDTH 1024						//���ڿ��
#define WINDOW_HEIGHT 700						//���ڸ߶�

#define Max_Eye_Distance 30000						//�ܿ�������Զ����

#define LIGHT_NUM 1								//��Դ����

#define SEA_DEPTH_FACTOR 0.18f						//��ͼ��߶ȶ���,ռ�ܸ߶ȵı���

#define DIFFUSE_INTENSITY 4.0f//1.33f			//�������ǿ��
#define AMBIENT_INTENSIYT 0.075f					//������ǿ�� 0.3f
#define SPECULAR_INTENSITY 0.45f					//�����ǿ��

//�����ʼ�ٶ�
#define DEFAULT_SPEED_MOVE 100.0f
#define DEAULT_SPEED_TRUN 1.5f

#define QUAKE_SPEED 80.0f						//����ˮ�д������ٶ�

#define MAX_QUAKE_COUNT 8							//��ˮ��Դ��������

//�ṹ���ö������

enum TextPos
{
	tpCenter,
	tpLeft,
	tpRight,
};

//ģ�͵����ͣ�����ģ�͵Ĵ�����ʽ
enum eMeshType
{
	mtMap,	//��ͼ���͵ģ����ͼ������

	mtXFile,	//��X�ļ��м��ص�ģ��

};

struct MapObjInfo
{
	int nObjId;		//����ID
	float x;		//�����Xֵ
	float z;		//�����Zֵ

	float fBlockHeight;
	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;
};

enum eForceType
{
	ftUpAlways,			//�Ϸ������ǣ�0,1,0��
	ftUpWithMap,		//�Ϸ�������洹ֱ�����ͼ���ı�
};
//////////////////////////�����ļ����õĺ���////////////////////////////////////

D3DVECTOR Vector ( float x, float y, float z );

//�������ʱDebug��Ϣ
void DebugMsgBox ( HRESULT hr, const char *StrDebug );

//��Mesh���÷���
ID3DXMesh *SetNormal ( ID3DXMesh *pMesh, IDirect3DDevice9 *DVC );

//��ȡ���ַ�������
int GetSubCharCount ( char* Sourc, char* Dev );

//��ȡָ��λ�õ����ַ���
int GetSubString ( char* Sourc, char* Dev, int Index, char*sOut );

//��ȡָ��λ�õ����ַ�
int GetSubCharIndexByCount ( char* Sourc, char* Dev, int Count );

//���ļ������л�ȡ·��
void  GetFilePath ( char* sFileName, char* path );

//����·�����ļ�������ȡ�ļ�ȫ��
void GetFileName ( char *sPathName, char* sFileName );

//�ж��ַ����Ƿ���ڣ��Ƿ��ǿ�ֵ
bool IsStrEmpty ( char * str );

//����ָ����Χ�����������
int RandIntEx ( int nMin, int nMax );

//����ָ����Χ�ĸ��������
float RandFloatEx ( float fMin, float fMax );

//�������������������
void PreRandom();

//�ж��ļ��Ƿ����
bool IsFileExist ( char * sFileName );

//��һ���ַ���ת����D3DXVECTOR3
D3DXVECTOR3 StrToVector3 ( char *str );	//��һ������ά�������ַ�����ֵȡ����

//�жϵ��Ƿ���һ������������
bool IsPointInRect ( POINT pt, RECT *rc );

//��ȡ��Χ��뾶
bool GetBoundRadius ( ID3DXMesh *pMesh, float *pfRadiusOut );

#define FiDeclareSingleton( ClassName ) \
	private:\
	static ClassName* mInstance;\
	public:\
	static ClassName& GetSingleton()  { if( mInstance == 0 ) { static ClassName scn; mInstance = &scn; } return *mInstance; }\
	static ClassName* GetSingletonPtr(){ if( mInstance == 0 ) { static ClassName scn; mInstance = &scn;} return mInstance;}

#define FiImplateSingleton( ClassName ) ClassName* ClassName::mInstance = 0 ;
#define FiMarkSingleton() \
	static bool bInit = false;\
	CXASSERT(!bInit && "not a singleton");\
	bInit = true;\

enum eHitType
{
	htNull,							//ʲô��û��ײ��
	htAutoMoveHitNoMap,				//û��ײ��
	htAutoMoveHitNoObj,				//û��ײ������
	htAutoMoveHitMap,				//ײ����ͼ��
	htAutoMoveHitObj,				//ײ��������
	//htAutoMoveHitObjAndMap,		//��ײ����

	htNotAutoMoveHitNoMap,			//û��ײ��
	htNotAutoMoveHitMap,			//ײ����ͼ
	htNotAutoMoveHitObj,			//ײ������
	htNotAutoMoveHitNoObj,			//û��ײ������

	htNotAutoMoveHitBelowMap,		//ײ�������������
	htNotAutoMoveHitNoBelowMap,		//û��ײ�����������

};


//�������ͣ�����ʱ������
enum eObjType
{
	Obj,
	WorldObj,
	VisObj,
	MeshObj,
	AnimMeshObj,
};

#define UpdateWithMap 1

#define OS_LIGHTON				0x0001			//�Ƿ񿪵ƹ�
#define OS_RENDER				0x0002			//�Ƿ���Ⱦ
#define OS_GETINPUT				0x0004			//�Ƿ���Բٿ�
#define OS_UPDATE				0x0008			//�Ƿ���Ը���
#define OS_BEHIT				0x0010			//�Ƿ񱻵����
#define OS_CHANGEWHENHIT		0x0020			//���ʱ�Ƿ����ı�
#define OS_CANBESELECT			0x0040			//�Ƿ���Ա�ѡ��
#define OS_BESELECTED			0x0080			//�Ƿ�ѡ����
#define OS_PLAYDONE				0x0100			//�ö����Ƿ񲥷����

#define OS_DEFAULT				( OS_RENDER || OS_UPDATE )		//Ĭ��״̬


enum eObjAnimState			//�����״̬
{
	oasNULL,
	oasStandBy,
	oasMoving,
	oasTurning,
	oasDead,
	oasAttack,
	oasBeAttack,
	oasRunAttack,
};



enum eObjParentType
{
	optByPosition,
	optByName,
};

struct ForceMapPara
{
	void *pMap;

	float fForceHeight;

	eForceType ft;

	ForceMapPara ( void *Map, float fHeight, eForceType ForceType )
	{
		pMap = Map;

		fForceHeight = fHeight;

		ft = ForceType;
	}

};

enum eMeshUsage
{
	muRender,
	muInsect,
};

struct MeshPara
{
public:

	int	  LnID;				//ģ�͵�ID
	float mfCellWidth;	    //��ͼ�и��ӿ��
	float mfMaxHeight;	    //��ͼ�����߶�
	int	  LnCellCount;	    //��ͼ�и�������
	char* mstrFileName;     //�����ļ����֣�X�ļ�����
	char* mstrHeightMap;    //�߶�ͼ�ļ�����

public:
	MeshPara() {};

	MeshPara ( int nID, float CellWidth, float MaxHight, int CellCount, char *strFileName, char *strHeightMap )
	{
		LnID = nID;
		mfCellWidth = CellWidth;
		mfMaxHeight = MaxHight;
		LnCellCount = CellCount;
		mstrFileName = strFileName;
		mstrHeightMap = strHeightMap;
	}
};
typedef void* GHandle;
extern const char* GetMediaPath();
//--------------------------------------------------------------------------------------------------

#endif // GGameDemoHeader_h__