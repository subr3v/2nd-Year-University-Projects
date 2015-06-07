#include "Scene.h"
#include "Input.h"
#include "GraphicsContext.h"
#include "SceneManager.h"
#include "SceneRenderer.h"
#include "SceneBuilder.h"

FScene::FScene(class FWindow* Window, FGraphicsContext* GraphicsContext, FInput* InputManager) 
	: Window(Window), InputManager(InputManager),
	GraphicsContext(GraphicsContext), CurrentCameraController(nullptr)
{
	SceneRenderer = new FSceneRenderer(GraphicsContext);
	SceneBuilder = new FSceneBuilder(InputManager, &ActiveCamera);
	SceneManager = new FSceneManager();
}

FScene::~FScene()
{
	delete CurrentCameraController;
	delete SceneManager;
	delete SceneRenderer;
	delete SceneBuilder;
}

void FScene::Load()
{
	SetCameraController(new FFreeFlyCameraController(Window, InputManager));
	ActiveCamera.SetPosition(glm::vec3(0.0f, 10.0f, 75.0f));
	SceneBuilder->LoadScene(SceneManager);
	SceneManager->Initialise();
}

void FScene::Unload()
{
	SceneBuilder->UnloadScene();
}

void FScene::Update(float DeltaTime)
{
	if (CurrentCameraController != nullptr)
		CurrentCameraController->Update(DeltaTime, &ActiveCamera);

	if (InputManager->IsKeyPressed('H'))
	{
		GraphicsContext->SetWireframeMode(!GraphicsContext->GetWireframeMode());
	}

	if (InputManager->IsKeyPressed('J'))
	{
		SceneRenderer->GetRenderingOptions().bRenderShadowVolumes = !SceneRenderer->GetRenderingOptions().bRenderShadowVolumes;
	}

	if (InputManager->IsKeyPressed('R'))
	{
		if (CurrentCameraController != nullptr)
			CurrentCameraController->Reset(&ActiveCamera);
	}

	if (InputManager->IsKeyPressed('1'))
	{
		SetCameraController(new FFreeFlyCameraController(Window, InputManager));
	}

	if (InputManager->IsKeyPressed('2'))
	{
		SetCameraController(new FLookCameraController(Window, InputManager));
	}

	if (InputManager->IsKeyPressed('3'))
	{
		SetCameraController(new FXYPlaneCameraController(Window, InputManager));
	}

	if (InputManager->IsKeyPressed('4'))
	{
		FSpline CameraPath;
		CameraPath.AddPoint(glm::vec3(0.0f, 75.0f, 1.0f));
		CameraPath.AddPoint(glm::vec3(0.0f, 150.0f, 0.0f));
		CameraPath.AddPoint(glm::vec3(0.0f, 150.0f, 150.0f));
		CameraPath.AddPoint(glm::vec3(-30.0f, 50.0f, 250.0f));
		SetCameraController(new FSplineCameraController(CameraPath, 20.0f, Window, InputManager));
	}

	if (InputManager->IsKeyPressed('5'))
	{
		SetCameraController(nullptr);
	}

	if (InputManager->IsKeyPressed('P'))
	{
		SceneManager->SetUseParallel(!SceneManager->IsUsingParallel());
	}

	SceneManager->Update(DeltaTime);
}

void FScene::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glLoadIdentity();

	ActiveCamera.Apply();

	SceneRenderer->DrawScene(SceneManager);

	GraphicsContext->SwapBuffers();
}

void FScene::SetCameraController(ICameraController* Controller)
{
	if (CurrentCameraController != nullptr)
	{
		delete CurrentCameraController;
	}
	else if(Controller != nullptr) 
		Controller->Reset(&ActiveCamera); // We have to initialise the camera with the first valid controller here.

	CurrentCameraController = Controller;
}
