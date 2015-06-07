#include "SplineNode.h"
#include "PrimitiveBuilder.h"


FSplineNode::FSplineNode(const FSpline& SplineData, float Duration) : FSceneNode(nullptr), SplineData(SplineData), Duration(Duration), CurrentTime(0.0f)
{

}

void FSplineNode::OnUpdate(float DeltaTime)
{
	CurrentTime += DeltaTime;
	if (CurrentTime >= Duration)
	{
		CurrentTime = 0.0f;
	}

	this->SetTransform(FTransform(SplineData.Evaluate(CurrentTime / Duration)));
}
