#ifndef InteractiveAnimatedModelSceneNode_h__
#define InteractiveAnimatedModelSceneNode_h__

#include "ClickableSceneNode.h"
#include "SkinnedModel.h"
#include "Skeleton.h"

class FAnimatedModelDisplayNode : public FClickableSceneNode
{
public:
	FAnimatedModelDisplayNode(FInput* Input, FCamera* Camera, FSkinnedModel* Model) : FClickableSceneNode(Input, Camera, Model)
	{
		CurrentAnimation = 0;
	}

	virtual void OnClicked() override
	{
		FSkinnedModel* Model = (FSkinnedModel*)GetRenderable();
		CurrentAnimation++;
		const std::vector<FSkeletalAnimation*>& Animations = Model->GetModelData()->Animations;
		if (CurrentAnimation >= Animations.size())
		{
			CurrentAnimation = 0;
		}
		Model->SetCurrentAnimation(Animations[CurrentAnimation]);
	}
private:
	size_t CurrentAnimation;
};


#endif // InteractiveAnimatedModelSceneNode_h__
