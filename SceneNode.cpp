#include "SceneNode.h"
#include <windows.h>
#include <gl/GL.h>
#include "Mesh.h"
#include "ShadowVolume.h"
#include "tbb/tbb.h"
#include "SceneManager.h"

FRenderingOptions::FRenderingOptions() : bCastShadow(false), bShowBoundingBox(false)
{

}

FSceneNode::FSceneNode(IRenderable* Renderable) : Renderable(Renderable), Transform(1.0f), Parent(nullptr), bIsNodeDirty(false), SceneManager(nullptr)
{
	
}

FSceneNode::~FSceneNode()
{
	for (size_t i = 0; i < Children.size(); i++)
	{
		delete Children[i];
	}
	Children.clear();

	for (size_t i = 0; i < ShadowVolumes.size(); i++)
	{
		delete ShadowVolumes[i];
	}
	ShadowVolumes.clear();
}

FSceneNode& FSceneNode::AddChild(FSceneNode* Child)
{
	Children.push_back(Child);
	Child->Parent = this;
	return *Child;
}

void FSceneNode::Update_Parallel(float DeltaTime)
{
	tbb::task_group& UpdateGroup = SceneManager->GetUpdateTaskGroup();

	UpdateGroup.run([this, DeltaTime]
	{
		OnUpdate(DeltaTime);

		if (Renderable != nullptr)
		{
			Renderable->Update(DeltaTime);

			if (Renderable->HasGeometryChanged())
			{
				bIsNodeDirty = true;
				Renderable->MarkGeometryChangeResolved();
			}
		}

		RefreshGeometry();
	});

	for (size_t i = 0; i < Children.size(); i++)
	{
		Children[i]->Update_Parallel(DeltaTime);
	}
}

void FSceneNode::Update(float DeltaTime)
{
	OnUpdate(DeltaTime);
	
	if (Renderable != nullptr)
	{
		Renderable->Update(DeltaTime);

		if (Renderable->HasGeometryChanged())
		{
			bIsNodeDirty = true;
			Renderable->MarkGeometryChangeResolved();
		}
	}

	RefreshGeometry();

	for (size_t i = 0; i < Children.size(); i++)
	{
		Children[i]->Update(DeltaTime);
	}
}

void FSceneNode::RefreshGeometry()
{
	if (bIsNodeDirty)
	{
		UpdateBoundingBox();
		CalculateShadowVolumeGeometry();
		bIsNodeDirty = false;
	}
}

void FSceneNode::Draw()
{
	glPushMatrix();
	glMultMatrixf(&Transform[0][0]);
	OnPreDraw();

	if (Renderable != nullptr)
	{
		Renderable->Render();
	}

	for (size_t i = 0; i < Children.size(); i++)
	{
		Children[i]->Draw();
	}

	OnPostDraw();
	glPopMatrix();

}

void FSceneNode::DrawDebug()
{
	if (Renderable != nullptr && RenderingOptions.bShowBoundingBox)
	{
		BoundingBox.Draw(glm::vec3(1.0f));
	}

	for (size_t i = 0; i < Children.size(); i++)
	{
		Children[i]->DrawDebug();
	}
}

void FSceneNode::DrawShadowVolume()
{
	if (Renderable != nullptr && RenderingOptions.bCastShadow)
	{
		for (size_t i = 0; i < ShadowVolumes.size(); i++)
			ShadowVolumes[i]->Render();
	}

	for (size_t i = 0; i < Children.size(); i++)
	{
		Children[i]->DrawShadowVolume();
	}
}

void FSceneNode::UpdateShadowVolume_Parallel(bool bPointLight, const glm::vec3& LightVector)
{
	tbb::task_group& ShadowVolumeGroup = SceneManager->GetShadowVolumeTaskGroup();

	if (Renderable != nullptr && RenderingOptions.bCastShadow)
	{
		for (size_t i = 0; i < ShadowVolumes.size(); i++)
		{
			ShadowVolumeGroup.run([this, bPointLight, LightVector, i]
			{
				ShadowVolumes[i]->EvaluateLightSource(bPointLight, LightVector);
			});
		}
	}

	for (size_t i = 0; i < Children.size(); i++)
	{
		Children[i]->UpdateShadowVolume_Parallel(bPointLight, LightVector);
	}
}

void FSceneNode::UpdateShadowVolume(bool bPointLight, const glm::vec3& LightVector)
{
	if (Renderable != nullptr && RenderingOptions.bCastShadow)
	{
		for (size_t i = 0; i < ShadowVolumes.size(); i++)
			ShadowVolumes[i]->EvaluateLightSource(bPointLight, LightVector);
	}

	for (size_t i = 0; i < Children.size(); i++)
	{
		Children[i]->UpdateShadowVolume(bPointLight, LightVector);
	}
}

void FSceneNode::SetTransform(const glm::mat4& Transform)
{
	this->Transform = Transform; bIsNodeDirty = true;
	for (size_t i = 0; i < Children.size(); i++)
	{
		Children[i]->bIsNodeDirty = true;
	}
}

void FSceneNode::SetRenderable(IRenderable* Renderable)
{
	this->Renderable = Renderable; bIsNodeDirty = true;
}

void FSceneNode::UpdateBoundingBox()
{
	if (Renderable != nullptr)
	{
		BoundingBox = FBoundingBox();

		const std::vector<FMesh*>& Meshes = Renderable->GetMeshes();
		for (size_t i = 0; i < Meshes.size(); i++)
		{
			BoundingBox.InflateFromVertices(Renderable->GetMeshVertices(i));
		}

		glm::mat4 BoxTransform = CalculateGlobalTransform();

		BoundingBox.Min = glm::vec3(BoxTransform * glm::vec4(BoundingBox.Min, 1.0f));
		BoundingBox.Max = glm::vec3(BoxTransform * glm::vec4(BoundingBox.Max, 1.0f));
	}
}

glm::mat4 FSceneNode::CalculateGlobalTransform() const
{
	glm::mat4 GlobalTransform = glm::mat4(1.0f);

	std::vector<glm::mat4> Hierarchy;

	FSceneNode* ParentNode = Parent;
	while (ParentNode != nullptr)
	{
		Hierarchy.push_back(ParentNode->Transform);
		ParentNode = ParentNode->Parent;
	}

	if (Hierarchy.size() > 0)
	{
		for (int i = Hierarchy.size() - 1; i >= 0; i--)
		{
			GlobalTransform *= Hierarchy[i];
		}
	}

	return GlobalTransform * Transform;
}

void FSceneNode::CalculateShadowVolumeGeometry()
{
	// This function assumes that the number of meshes doesn't change.
	if (Renderable != nullptr)
	{
		const std::vector<FMesh*>& Meshes = Renderable->GetMeshes();
		glm::mat4 GlobalTransform = CalculateGlobalTransform();

		if (ShadowVolumes.size() == 0)
		{
			for (size_t i = 0; i < Meshes.size(); i++)
			{
				ShadowVolumes.push_back(new FShadowVolume(Meshes[i], GlobalTransform));
			}
		}
		else
		{
			for (size_t i = 0; i < Meshes.size(); i++)
			{
				ShadowVolumes[i]->UpdateGeometry(Renderable->GetMeshVertices(i), GlobalTransform);
			}
		}
	}
}

void FSceneNode::Initialise(class FSceneManager* SceneManager)
{
	this->SceneManager = SceneManager;
	for (size_t i = 0; i < Children.size(); i++)
	{
		Children[i]->Initialise(SceneManager);
	}
}