#include "WaterSurfaceNode.h"
#include "PrimitiveBuilder.h"
#include "Mesh.h"
#include "Texture.h"

FWaterSurfaceNode::FWaterSurfaceNode() : Timer(0.0f)
{
	this->Width = 128;
	this->Height = 128;
	this->CellSize = 2;

	WaterTexture = new FTexture("water.png");

	FPrimitiveBuilder PlaneBuilder;
	PlaneMesh = PlaneBuilder.GeneratePlane(WaterTexture, glm::mat4(1.0f), Width, Height, CellSize);
	this->Model = PlaneBuilder.GetGeneratedModel();

	PlaneMesh->GetMaterial();

	SetRenderable(Model);
}

FWaterSurfaceNode::~FWaterSurfaceNode()
{
	delete Model;
	Model = nullptr;
}

void FWaterSurfaceNode::OnUpdate(float DeltaTime)
{
	Timer += DeltaTime;

	std::vector<glm::vec3>& WaterVertices = PlaneMesh->GetVerticesData();
	std::vector<glm::vec3>& WaterNormals = PlaneMesh->GetNormalData();

	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			glm::vec3& Vertex = WaterVertices[y * Width + x];
			float ChunkHeight = 0.0f;
			ChunkHeight += sinf((((float)x / Width) * M_PI) + Timer);
			ChunkHeight += sinf((((float)x / Width) * M_PI * 8) + Timer);
			ChunkHeight += sinf((((float)y / Height) * M_PI * 4) + Timer);
			ChunkHeight += sinf((((float)y / Height) * M_PI * 12) + Timer);
			Vertex.y = ChunkHeight;
		}
	}

	for (int y = 0; y < Height - 1; y++)
	{
		for (int x = 0; x < Width - 1; x++)
		{
			glm::vec3& Normal = WaterNormals[y * Width + x];

			glm::vec3 P0 = WaterVertices[x + y * Width];
			glm::vec3 P1 = WaterVertices[x + 1 + y * Width];
			glm::vec3 P2 = WaterVertices[Width * (y + 1) + x];

			glm::vec3 Cross = glm::cross((P0 - P1), (P0 - P2));

			Normal = -glm::normalize(Cross);
		}
	}
}

void FWaterSurfaceNode::OnPreDraw()
{
	glMatrixMode(GL_TEXTURE);
	
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(Timer * 0.01f, 0.0f, 0.0f);

	glMatrixMode(GL_MODELVIEW);
}

void FWaterSurfaceNode::OnPostDraw()
{
	glMatrixMode(GL_TEXTURE);
	
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}

