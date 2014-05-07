#include "GGameDemoHeader.h"
#include "GComponentMesh.h"


GComponentMesh::GComponentMesh(void)
{
}


GComponentMesh::~GComponentMesh(void)
{
}

void GComponentMesh::RegisterAll()
{
	__RegisterProperty(mMeshFile);
}
