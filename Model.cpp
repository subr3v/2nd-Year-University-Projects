#include "model.h"
#include "Mesh.h"
#include "Material.h"
#include "Skeleton.h"

FModel::FModel(const std::string& Filename)
{
	FFBXModelLoader ModelLoader;
	ModelLoader.LoadModel(Filename, ModelData);
}

FModel::~FModel()
{

}

void FModel::Render()
{
	for (size_t i = 0; i < ModelData.Meshes.size(); i++)
	{
		ModelData.Meshes[i]->Render();
	}
}

const std::vector<class FSkeletalAnimation*>& FModel::GetAnimations()
{
	return ModelData.Animations;
}
