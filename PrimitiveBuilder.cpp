#include "PrimitiveBuilder.h"
#include "Mesh.h"
#include "Material.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "ShadowVolume.h"

FProceduralModel::FProceduralModel() { }

FProceduralModel::~FProceduralModel()
{
	for (size_t i = 0; i < Meshes.size(); i++)
	{
		delete Meshes[i];
	}
	Meshes.clear();

	for (size_t i = 0; i < Materials.size(); i++)
	{
		delete Materials[i];
	}
	Materials.clear();

	for (size_t i = 0; i < ShadowVolumes.size(); i++)
	{
		delete ShadowVolumes[i];
	}
	ShadowVolumes.clear();
}

void FProceduralModel::Render()
{
	for (size_t i = 0; i < Meshes.size(); i++)
	{
		Meshes[i]->Render();
	}
}

FPrimitiveBuilder::FPrimitiveBuilder()
{
	ProceduralModel = new FProceduralModel();
}

class FMesh* FPrimitiveBuilder::GenerateCube(class FTexture* Texture, const glm::mat4& Transform, const glm::vec4& Colour)
{
	FMesh* Mesh = CreateMesh(Texture, Colour);

	const float kHalfCubeSize = 0.5f;

	// Bottom face.
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f));

	// Top face
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, kHalfCubeSize, kHalfCubeSize), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, kHalfCubeSize, kHalfCubeSize), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, kHalfCubeSize, kHalfCubeSize), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f));

	// Front face
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, kHalfCubeSize, kHalfCubeSize), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, kHalfCubeSize, kHalfCubeSize), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f));
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, kHalfCubeSize, kHalfCubeSize), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f));

	// Back face
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f));
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f));

	// Right face
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, kHalfCubeSize, kHalfCubeSize), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(kHalfCubeSize, kHalfCubeSize, kHalfCubeSize), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f));

	// Left face
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f));
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, kHalfCubeSize, kHalfCubeSize), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f));
	Mesh->AddVertexData(glm::vec3(-kHalfCubeSize, kHalfCubeSize, kHalfCubeSize), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f));

	Mesh->TransformVertices(Transform);
	return Mesh;
}

class FMesh* FPrimitiveBuilder::CreateMesh(class FTexture* Texture, const glm::vec4& Colour, bool bUseIndexBuffer)
{
	FMesh* Mesh = new FMesh(bUseIndexBuffer);
	FMaterial* Material = new FMaterial(Texture);
	Material->SetDiffuse(Colour);
	// TODO: Might need to fix this or add more stuff. check it later
	ProceduralModel->Meshes.push_back(Mesh);
	ProceduralModel->Materials.push_back(Material);
	Mesh->SetMaterial(Material);
	return Mesh;
}

class FMesh* FPrimitiveBuilder::GenerateDisc(int SubdivisionNumber, class FTexture* Texture, const glm::mat4& Transform, const glm::vec4& Colour /*= glm::vec4(1.0f)*/)
{
	FMesh* Mesh = CreateMesh(Texture, Colour);

	const float kHalfCubeSize = 0.5f;
	float Theta = 0.0f;
	float DeltaTheta = (float) ((2 * M_PI) / SubdivisionNumber);
	for (int i = 0; i < SubdivisionNumber; i++)
	{
		float PreviousTheta = Theta;
		Theta += DeltaTheta;

		glm::vec3 Positions[3] = 
		{
			glm::vec3(cos(PreviousTheta), sin(PreviousTheta), 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(cos(Theta), sin(Theta), 0.0f)
		};

		glm::vec3 Normal(0.0f, 0.0f, 1.0f);

		for (int k = 0; k < 3; k++)
		{
			Mesh->AddVertexData(Positions[k], Normal, glm::vec2(Positions[k].x * 0.5f + 0.5f, Positions[k].y * 0.5f + 0.5f));
		}
	}

	Mesh->TransformVertices(Transform);
	return Mesh;
}

class FMesh* FPrimitiveBuilder::GeneratePlane(class FTexture* Texture, const glm::mat4& Transform, int Width, int Height, float CellSize, const glm::vec4& Colour /*= glm::vec4(1.0f)*/)
{
	FMesh* Mesh = CreateMesh(Texture, Colour, true);

	float PlaneWidth = Width * CellSize;
	float PlaneHeight = Height * CellSize;

	float UIncrement = 1 / (float)Width;
	float VIncrement = 1 / (float)Height;

	float HalfSize = CellSize * 0.5f;

	glm::vec3 Normal(0.0f, 1.0f, 0.0f);

	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			glm::vec3 Point(-HalfSize, 0.0f, -HalfSize);
			glm::vec2 UV(x / (float)(Width - 1), y / (float)(Height - 1));
			glm::mat4 PositionTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-PlaneWidth * 0.5f + x * CellSize, 0.0f, -PlaneHeight * 0.5f + y * CellSize));
			Point = glm::vec3(PositionTransform * glm::vec4(Point, 1.0f));
			Mesh->AddVertexData(Point, Normal, UV);
		}
	}

	for (int x = 0; x < Width - 1; x++)
	{
		for (int y = 0; y < Height - 1; y++)
		{
			Mesh->AddIndexData(Width * (y + 1) + x);
			Mesh->AddIndexData(x + 1 + y * Width);
			Mesh->AddIndexData(x + y * Width);

			Mesh->AddIndexData(x + 1 + Width * y);
			Mesh->AddIndexData(Width * (y + 1) + x);
			Mesh->AddIndexData(Width * (y + 1) + x + 1);
		}
	}

	Mesh->TransformVertices(Transform);
	return Mesh;
}

class FMesh* FPrimitiveBuilder::GenerateSphere(int NumberOfSubdivisions, class FTexture* Texture, const glm::mat4& Transform, const glm::vec4& Colour /*= glm::vec4(1.0f)*/)
{
	FMesh* Mesh = CreateMesh(Texture, Colour);

	struct FSphereHelper
	{
		static glm::vec3 CalculateSpherePoint(float Theta, float Delta)
		{
			return glm::vec3(cos(Theta) * sin(Delta), cos(Delta), sin(Theta) * sin(Delta));
		}
	};

	float Theta = 0.0f; // Latitude
	float Delta = 0.0f; // Longitude

	float ThetaDifference = (float)(M_PI) / NumberOfSubdivisions;
	float DeltaDifference = (float)(2 * M_PI) / NumberOfSubdivisions;

	float DeltaTexcoord = 1.0f / NumberOfSubdivisions;

	float CurrentU = 0.0f;
	float CurrentV = 0.0f;

	for (int Longitude = 0; Longitude < NumberOfSubdivisions; Longitude++)
	{
		Theta = 0.0f;
		CurrentU = 0.0f;

		for (int Latitude = 0; Latitude < NumberOfSubdivisions; Latitude++)
		{

			glm::vec3 SpherePoints[6] =
			{
				FSphereHelper::CalculateSpherePoint(Theta, Delta),
				FSphereHelper::CalculateSpherePoint(Theta, Delta + DeltaDifference),
				FSphereHelper::CalculateSpherePoint(Theta + ThetaDifference, Delta + DeltaDifference),

				FSphereHelper::CalculateSpherePoint(Theta, Delta),
				FSphereHelper::CalculateSpherePoint(Theta + ThetaDifference, Delta + DeltaDifference),
				FSphereHelper::CalculateSpherePoint(Theta + ThetaDifference, Delta),
			};

			glm::vec2 SphereUvs[6] =
			{
				glm::vec2(CurrentU, CurrentV),
				glm::vec2(CurrentU, CurrentV + DeltaTexcoord),
				glm::vec2(CurrentU + DeltaTexcoord, CurrentV + DeltaTexcoord),

				glm::vec2(CurrentU, CurrentV),
				glm::vec2(CurrentU + DeltaTexcoord, CurrentV + DeltaTexcoord),
				glm::vec2(CurrentU + DeltaTexcoord, CurrentV),
			};

			for (int i = 0; i < 6; i++)
			{
				Mesh->AddVertexData(SpherePoints[i], SpherePoints[i], SphereUvs[i]);
			}

			Theta += ThetaDifference;
			CurrentU += DeltaTexcoord;
		}

		CurrentV += DeltaTexcoord;
		Delta += DeltaDifference;
	}

	Mesh->TransformVertices(Transform);
	return Mesh;
}

FTransform::operator glm::mat4() const
{
	return glm::scale(glm::translate(glm::mat4(1.0f), Position) * glm::yawPitchRoll(glm::radians(Rotation.y), glm::radians(Rotation.x), glm::radians(Rotation.z)), Scale);
}
