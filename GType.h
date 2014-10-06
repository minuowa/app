#ifndef GType_h__
#define GType_h__
//-------------------------------------------------------------------------
/**
	@brief 
	@author nnboy,28/9/2014  20:29
*/
enum GDeviceType
{
	GDeviceType_DX9,
	GDeviceType_OpenGL,
	GDeviceType_Max,
};
enum eGMatrixType
{
	eGMatrixType_World,
	eGMatrixType_Proj,
	eGMatrixType_View,
	eGMatrixType_Max,
};

enum eGPool 
{
	eGPool_Default					= 0,
	eGPool_Managed					= 1,
	eGPool_System					= 2,
	eGPool_Scratch					= 3,

	eGPool_ForceDword				= 0x7fffffff
} ;
#endif // GType_h__
