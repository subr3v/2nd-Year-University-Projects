#ifndef Scene_h__
#define Scene_h__

#include "Camera.h"
#include "ShadowVolume.h"
#include "CameraControllers.h"

class FScene
{
public:
	FScene(class FWindow* Window, class FGraphicsContext* GraphicsContext, class FInput* InputManager);
	~FScene();

	void Load();
	void Unload();
	void Update(float DeltaTime);
	void Render();

	void SetCameraController(ICameraController* Controller);
private:
	class FSceneRenderer* SceneRenderer;
	class FSceneManager* SceneManager;
	class FSceneBuilder* SceneBuilder;

	ICameraController* CurrentCameraController;

	FCamera ActiveCamera;

	FWindow* Window;
	FInput* InputManager;
	FGraphicsContext* GraphicsContext;
};

#endif // Scene_h__
