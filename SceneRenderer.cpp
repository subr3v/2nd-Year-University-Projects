#include "SceneRenderer.h"
#include "RenderingHelpers.h"
#include "SceneManager.h"
#include "Geometry.h"

FSceneRenderer::FSceneRenderer(FGraphicsContext* GraphicsContext) : GraphicsContext(GraphicsContext)
{

}

void FSceneRenderer::DrawScene(FSceneManager* SceneManager)
{
	{
		FStencilShadowHelper::FDepthPassState DepthPass;
		SceneManager->Draw();
	}

	const std::vector<ILight*> Lights = SceneManager->GetSceneLights();

	glClearStencil(0);
	
	for (size_t i = 0; i < Lights.size(); i++)
	{
		ILight* Light = Lights[i];
		if (!Light->bIsActive || !Light->bCastShadow)
			continue;

		glm::vec3 LightVector(Light->GetLightVector());

		SceneManager->UpdateShadowVolume(Light->IsPointSource(), LightVector);

		{
			FStencilShadowHelper::FShadowVolumePass IncreasePass(GL_INCR, GL_BACK);
			SceneManager->DrawShadowVolume();
		}

		{
			FStencilShadowHelper::FShadowVolumePass DecreasePass(GL_DECR, GL_FRONT);
			SceneManager->DrawShadowVolume();
		}

		// This only works correctly for point light sources.
		if (RenderingOptions.bRenderDebugLightPosition)
		{
			FBoundingBox LightBox;
			LightBox.Min = glm::vec3(-1.0f);
			LightBox.Max = glm::vec3(1.0f);
			LightBox.Position = LightVector;
			LightBox.Draw(glm::vec3(1.0f, 0.0f, 0.0f));
		}
	}

	{
		FStencilShadowHelper::FShadowedScenePassState ShadowedPass;
		GraphicsContext->ResetLighting();
		SceneManager->Draw();
	}

	GraphicsContext->ResetLighting();

	int LightIndex = 0;
	for (size_t i = 0; i < Lights.size(); i++)
	{
		ILight* Light = Lights[i];
		if (Light->bIsActive)
		{
			Light->Apply(LightIndex++);
		}
	}

	{
		FStencilShadowHelper::FLitScenePassState LitPass;
		SceneManager->Draw();
	}

	SceneManager->DrawDebug();

	if (RenderingOptions.bRenderShadowVolumes)
	{
		FScopedGLState Culling(GL_CULL_FACE, GL_TRUE);
		glCullFace(GL_FRONT);
		SceneManager->DrawShadowVolume();
		glCullFace(GL_BACK);
		SceneManager->DrawShadowVolume();
	}
}
