#ifndef SceneNode_h__
#define SceneNode_h__

#include <glm/glm.hpp>
#include <vector>
#include "Renderable.h"

struct FRenderingOptions
{
	bool bCastShadow;
	bool bShowBoundingBox;

	FRenderingOptions();
};

class FSceneNode
{
public:
	FSceneNode(IRenderable* Renderable = nullptr);
	virtual ~FSceneNode();

	FSceneNode& AddChild(FSceneNode* Child);

	void Update_Parallel(float DeltaTime);
	void UpdateShadowVolume_Parallel(bool bPointLight, const glm::vec3& LightVector);

	void Update(float DeltaTime);
	void Draw();
	void DrawDebug();

	void RefreshGeometry();

	void UpdateShadowVolume(bool bPointLight, const glm::vec3& LightVector);
	void DrawShadowVolume();

	void SetTransform(const glm::mat4& Transform);
	void SetRenderable(IRenderable* Renderable);

	const IRenderable* GetRenderable() const { return Renderable; }
	glm::mat4 GetTransform() const { return Transform; }
	glm::mat4 GetGlobalTransform() const { return CalculateGlobalTransform(); }
	FRenderingOptions& GetRenderingOptions() { return RenderingOptions; }
	FBoundingBox GetBoundingBox() const { return BoundingBox; }
	
private:
	void Initialise(class FSceneManager* SceneManager);

	virtual void OnPreDraw() { }
	virtual void OnPostDraw() { }
	virtual void OnUpdate(float DeltaTime) { }

	glm::mat4 CalculateGlobalTransform() const;

	void UpdateBoundingBox();
	void CalculateShadowVolumeGeometry();

	FSceneNode* Parent;

	IRenderable* Renderable;
	FRenderingOptions RenderingOptions;

	std::vector<FSceneNode*> Children;
	glm::mat4 Transform;
	FBoundingBox BoundingBox;
	bool bIsNodeDirty;

	std::vector<class FShadowVolume*> ShadowVolumes;

	friend class FSceneManager;
	class FSceneManager* SceneManager;
};


#endif // SceneNode_h__
