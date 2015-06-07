#include "SceneManager.h"
#include "SceneNode.h"

FSceneManager::FSceneManager() : bUseParallel(true)
{
	RootNode = new FSceneNode();
}

FSceneManager::~FSceneManager()
{
	delete RootNode;

	for (size_t i = 0; i < SceneLights.size(); i++)
	{
		delete SceneLights[i];
	}
	SceneLights.clear();
}

void FSceneManager::Update(float DeltaTime)
{
	if (bUseParallel)
	{
		RootNode->Update_Parallel(DeltaTime);
		UpdateTaskGroup.wait();
	}
	else
		RootNode->Update(DeltaTime);
}

void FSceneManager::Draw()
{
	RootNode->Draw();
}

void FSceneManager::DrawDebug()
{
	RootNode->DrawDebug();
}

void FSceneManager::UpdateShadowVolume(bool bPointLight, const glm::vec3& LightVector)
{
	if (bUseParallel)
	{
		RootNode->UpdateShadowVolume_Parallel(bPointLight, LightVector);
		ShadowVolumeTaskGroup.wait();
	}
	else
	{
		RootNode->UpdateShadowVolume(bPointLight, LightVector);
	}
}

void FSceneManager::DrawShadowVolume()
{
	RootNode->DrawShadowVolume();
}

void FSceneManager::Initialise()
{
	RootNode->Initialise(this);
}

tbb::task_group& FSceneManager::GetUpdateTaskGroup()
{
	return UpdateTaskGroup;
}

tbb::task_group& FSceneManager::GetShadowVolumeTaskGroup()
{
	return ShadowVolumeTaskGroup;
}

void FSceneManager::SetUseParallel(bool bUseParallel)
{
	this->bUseParallel = bUseParallel;
}
