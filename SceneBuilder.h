#ifndef SceneBuilder_h__
#define SceneBuilder_h__

class FSceneBuilder
{
public:
	FSceneBuilder(class FInput* InputManager, class FCamera* SceneCamera);

	void LoadScene(class FSceneManager* SceneManager);
	void UnloadScene();

private:
	class FInput* InputManager;
	class FCamera* SceneCamera;
};

#endif // SceneBuilder_h__
