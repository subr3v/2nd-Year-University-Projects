#ifndef WaterSurfaceNode_h__
#define WaterSurfaceNode_h__

#include "SceneNode.h"

class FWaterSurfaceNode : public FSceneNode
{
public:
	FWaterSurfaceNode();
	~FWaterSurfaceNode();

private:
	virtual void OnUpdate(float DeltaTime) override;

	virtual void OnPreDraw() override;
	virtual void OnPostDraw() override;

	class FTexture* WaterTexture;
	class FProceduralModel* Model;
	class FMesh* PlaneMesh;
	int Width, Height;
	float CellSize;

	float Timer;
};


#endif // WaterSurfaceNode_h__
