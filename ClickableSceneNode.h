#ifndef ClickableSceneNode_h__
#define ClickableSceneNode_h__

#include "Input.h"
#include "SceneNode.h"
#include "Camera.h"

class FClickableSceneNode : public FSceneNode
{
public:
	FClickableSceneNode(FInput* Input, FCamera* Camera, IRenderable* Renderable) : FSceneNode(Renderable), InputManager(Input), Camera(Camera)
	{

	}

	virtual void OnUpdate(float DeltaTime) override
	{
		FRay MouseRay = InputManager->GetMouseRay(Camera->GetPosition());
		float Distance;

		FBoundingBox Box = GetBoundingBox();

		bool MouseOver = Box.CollidesWith(MouseRay, Distance);
		GetRenderingOptions().bShowBoundingBox = MouseOver;

		if (InputManager->IsMouseButtonPressed(MouseButton_Left) && MouseOver)
		{
			OnClicked();
		}
	}

	virtual void OnClicked() = 0;
private:
	FCamera* Camera;
	FInput* InputManager;
};

#endif // ClickableSceneNode_h__
