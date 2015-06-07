#include "StaticModel.h"
#include "Mesh.h"
#include "ShadowVolume.h"

FStaticModel::FStaticModel(FModelData* ModelData) : ModelData(ModelData), DisplayList(nullptr)
{

}

FStaticModel::~FStaticModel()
{
	ModelData = nullptr;

	for (size_t i = 0; i < ShadowVolumes.size(); i++)
	{
		delete ShadowVolumes[i];
	}
	ShadowVolumes.clear();

	if (DisplayList != nullptr)
		delete DisplayList;
}

void FStaticModel::Render()
{
	if (DisplayList == nullptr)
	{
		DisplayList = new FOpenGLDisplayList();
		DisplayList->Begin();
		for (size_t i = 0; i < ModelData->Meshes.size(); i++)
		{
			ModelData->Meshes[i]->Render();
		}
		DisplayList->End();
	}

	DisplayList->Render();
}

const std::vector<class FMesh*>& FStaticModel::GetMeshes() const
{
	return ModelData->Meshes;
}

