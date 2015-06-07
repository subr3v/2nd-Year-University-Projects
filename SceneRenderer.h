#include "Lighting.h"
#ifndef SceneRenderer_h__
#define SceneRenderer_h__

struct FSceneRendererOptions
{
	bool bRenderShadowVolumes;
	bool bRenderDebugLightPosition;

	FSceneRendererOptions() : bRenderShadowVolumes(false), bRenderDebugLightPosition(false) { }
};

class FSceneRenderer
{
public:
	FSceneRenderer(class FGraphicsContext* GraphicsContext);
	void DrawScene(class FSceneManager* SceneManager);

	FSceneRendererOptions& GetRenderingOptions() { return RenderingOptions; }
private:
	class FGraphicsContext* GraphicsContext;
	FSceneRendererOptions RenderingOptions;
};

#endif // SceneRenderer_h__
