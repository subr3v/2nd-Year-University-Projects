#ifndef SkinnedModel_h__
#define SkinnedModel_h__

#include "ModelData.h"
#include "Renderable.h"
#include <glm/glm.hpp>

struct FAnimatedMeshData
{
	std::vector<glm::vec3> AnimatedVertexPositionData;
	std::vector<glm::vec3> AnimatedVertexNormalData;
};

class FSkinnedModel : public IRenderable
{
public:
	FSkinnedModel(FModelData* ModelData);
	~FSkinnedModel();

	virtual void Render() override;
	virtual void Update(float DeltaTime) override;
	virtual const std::vector<class FMesh*>& GetMeshes() const override;
	virtual const std::vector<glm::vec3>& GetMeshVertices(int MeshIndex) const override;

	void SetCurrentAnimation(class FSkeletalAnimation* Animation);

	FModelData* GetModelData() const { return ModelData; }

private:
	void PerformSoftwareSkinning();
	void PerformSoftwareSkinning_Parallel();



	class FSkeletalAnimation* CurrentAnimation;
	FModelData* ModelData;
	std::vector<FAnimatedMeshData> AnimatedMeshesData;
	std::vector<glm::mat4> GlobalTransforms;
	int CurrentAnimationFrame;
	float CurrentFrameTime;
	std::vector<class FShadowVolume*> ShadowVolumes;
};

#endif // SkinnedModel_h__
