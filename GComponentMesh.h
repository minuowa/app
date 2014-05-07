#pragma once
#include "gcomponentinterface.h"
class GComponentMesh :
	public GComponentBase<eComponentType_Mesh>
{
	DeclareComponentType(GComponentMesh);
public:
	GComponentMesh(void);
	~GComponentMesh(void);

	virtual void RegisterAll();

protected:
	CharString mMeshFile;
public:
	inline const CharString& MeshFile() const 
	{
		return mMeshFile; 
	}
	inline void MeshFile(const char* val) 
	{ 
		mMeshFile = val;
	}
};

