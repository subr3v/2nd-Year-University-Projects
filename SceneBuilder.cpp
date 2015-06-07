#include "SceneBuilder.h"
#include "PrimitiveBuilder.h"
#include "Mesh.h"
#include "WaterSurfaceNode.h"
#include "Lighting.h"
#include "Spline.h"
#include "SceneManager.h"
#include "ModelData.h"
#include "SkinnedModel.h"
#include "InteractiveAnimatedModelSceneNode.h"
#include "SplineNode.h"
#include "StaticModel.h"

FSceneBuilder::FSceneBuilder(FInput* InputManager, FCamera* SceneCamera) : InputManager(InputManager), SceneCamera(SceneCamera)
{

}

void FSceneBuilder::LoadScene(class FSceneManager* SceneManager)
{
	FSceneNode* SceneRootNode = SceneManager->GetRoot();
	//FTexture* CrateTexture = new FTexture("crate.png");
	//CrateTexture->SetFiltering(Trilinear);

	FPrimitiveBuilder PrimitiveBuilder;
	//PrimitiveBuilder.GenerateCube(CrateTexture, FTransform(glm::vec3(10.0f, 12.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(5.0f)));
	FMesh* PlaneMesh = PrimitiveBuilder.GeneratePlane(nullptr, FTransform(glm::vec3(60.0f, 35.0f, 25.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)), 2, 2, 50.0f);
	//PrimitiveBuilder.GenerateDisc(20, CrateTexture, FTransform(glm::vec3(-35.0f, 15.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(15.0f)));
	//PrimitiveBuilder.GenerateSphere(32, nullptr, FTransform(glm::vec3(35.0f, 15.0f, 5.0f), glm::vec3(0.0), glm::vec3(15.0f)));
	PrimitiveBuilder.GeneratePlane(nullptr, FTransform(glm::vec3(0.0f, 1.0f, 0.0f)), 16, 16, 64.0f, glm::vec4(1.0f, 1.0f, 1.0f, 0.2f));

	FProceduralModel* ProceduralTestModel = PrimitiveBuilder.GetGeneratedModel();

	FPrimitiveBuilder Builder;
	Builder.GeneratePlane(nullptr, FTransform(), 16, 16, 4.0f, glm::vec4(0.8f, 0.5f, 0.5f, 0.7f));

	FModelData* ModelData = new FModelData();
	FModelData* EnvData = new FModelData();

	FFBXModelLoader ModelLoader;
	ModelLoader.LoadModel("Models/golem_earth_run.fbx", *ModelData);
	ModelLoader.LoadAnimation("Models/golem_earth_idle.fbx", *ModelData);
	//ModelLoader.LoadAnimation("Models/golem_earth_cast.fbx", *ModelData);
	//ModelLoader.LoadModel("Models/environment.fbx", *EnvData);

	FSkinnedModel* SkinnedModel = new FSkinnedModel(ModelData);
	FSkinnedModel* SkinnedModel2 = new FSkinnedModel(ModelData);

	FSceneNode* SkinnedModelNode = new FSceneNode(SkinnedModel);
	//SkinnedModelNode->SetTransform(FTransform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(15.0f)));
	SkinnedModelNode->GetRenderingOptions().bCastShadow = true;

	FSceneNode* SkinnedModelNode2 = new FAnimatedModelDisplayNode(InputManager, SceneCamera, SkinnedModel2);
	SkinnedModelNode2->SetTransform(FTransform(glm::vec3(-50.0f, 0.0f, 50.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	SkinnedModelNode2->GetRenderingOptions().bCastShadow = true;

	for (int i = 0; i < 6; i++)
	{
		for (int x = 0; x < 8; x++)
		{
			FSkinnedModel* AnimatedModel = new FSkinnedModel(ModelData);
			FSceneNode* AnimatedModelNode = new FSceneNode(AnimatedModel);
			AnimatedModelNode->GetRenderingOptions().bCastShadow = true;
			AnimatedModelNode->SetTransform(FTransform(glm::vec3(40.0f * i, 0.0f, 30.0f * x)));
			SceneRootNode->AddChild(AnimatedModelNode);
		}
	}

	FSpline SplineData;
	SplineData.AddPoint(glm::vec3(0.0f));
	SplineData.AddPoint(glm::vec3(0.0f, 0.0f, 50.0f));
	SplineData.AddPoint(glm::vec3(35.0f, 0.0f, 0.0f));

	FSplineNode* SplineNode = new FSplineNode(SplineData, 10.0f);
	SplineNode->AddChild(SkinnedModelNode);

	SceneRootNode->AddChild(SplineNode);
	SceneRootNode->AddChild(SkinnedModelNode2);

	FSceneNode* StaticModelNode = new FSceneNode(new FStaticModel(EnvData));
	//StaticModelNode->SetTransform(FTransform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-90.0f, 0.0f, 0.0f), glm::vec3(1 / 12.0f)));
	//StaticModelNode->GetRenderingOptions().bCastShadow = true;
	//StaticModelNode->GetRenderingOptions().bRenderBoundingBox = true;

	//SceneRootNode->AddChild(StaticModelNode);

	FSceneNode* ProceduralNode = new FSceneNode(ProceduralTestModel);
	SceneRootNode->AddChild(ProceduralNode);
	ProceduralNode->GetRenderingOptions().bCastShadow = true;

	FWaterSurfaceNode* WaterNode = new FWaterSurfaceNode();
	WaterNode->SetTransform(FTransform(glm::vec3(150.0f, 8.0f, 50.0f)));

	SceneRootNode->AddChild(WaterNode);

	FPointLight* PointLight = new FPointLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(20.0f, 55.5f, 0.0f));
	FSpline LightSpline;
	LightSpline.AddPoint(glm::vec3(20.0f, 55.5f, 0.0f));
	LightSpline.AddPoint(glm::vec3(60.0f, 55.5f, 0.0f));
	LightSpline.AddPoint(glm::vec3(-130.0f, 55.5f, 15.0f));
	LightSpline.AddPoint(glm::vec3(20.0f, 55.5f, 0.0f));

	FSplineNode* LightSplineNode = new FSplineNode(LightSpline, 15.0f);
	SceneRootNode->AddChild(LightSplineNode);
	LightSplineNode->AddChild(new FLightSceneNode(PointLight));

	FPointLight* PointLight2 = new FPointLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 55.5f, 50.0f));

	SceneManager->AddLight(PointLight);
	SceneManager->AddLight(PointLight2);
}

void FSceneBuilder::UnloadScene()
{

}
