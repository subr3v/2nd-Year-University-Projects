#ifndef SceneManager_h__
#define SceneManager_h__

#include <glm/glm.hpp>
#include <vector>
#include "Lighting.h"
#include "tbb/tbb.h"

class FSceneManager
{
public:
	FSceneManager();
	~FSceneManager();

	void Initialise();

	void Update(float DeltaTime);
	void Draw();
	void DrawDebug();

	void UpdateShadowVolume(bool bPointLight, const glm::vec3& LightVector);
	void DrawShadowVolume();

	void AddLight(ILight* Light) { SceneLights.push_back(Light); }
	const std::vector<ILight*>& GetSceneLights() { return SceneLights; }
	class FSceneNode* GetRoot() { return RootNode; }

	void SetUseParallel(bool bUseParallel);
	bool IsUsingParallel() { return bUseParallel; }

	tbb::task_group& GetUpdateTaskGroup();
	tbb::task_group& GetShadowVolumeTaskGroup();

private:
	class FSceneNode* RootNode;
	std::vector<ILight*> SceneLights;

	bool bUseParallel;

	tbb::task_group UpdateTaskGroup;
	tbb::task_group ShadowVolumeTaskGroup;
};

#endif // SceneManager_h__
