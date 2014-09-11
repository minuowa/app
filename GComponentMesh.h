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
	virtual void OnPropertyChange(void* pre,void* changed);
protected:
	String mMeshFile;
public:
	inline const String& MeshFile() const 
	{
		return mMeshFile; 
	}
	inline void MeshFile(const char* val) 
	{ 
		mMeshFile = val;
	}
};

