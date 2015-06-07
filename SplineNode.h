#include "SceneNode.h"
#ifndef SplineNode_h__
#define SplineNode_h__

#include "SceneNode.h"
#include "Spline.h"

class FSplineNode : public FSceneNode
{
public:
	FSplineNode(const FSpline& SplineData, float Duration);
	virtual void OnUpdate(float DeltaTime) override;

private:
	FSpline SplineData;
	float Duration;
	float CurrentTime;
};

#endif // SplineNode_h__
