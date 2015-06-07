#include "Mesh.h"
#include "Material.h"

#include <windows.h>
#include <gl/GL.h>
#include "Model.h"
#include "Skeleton.h"

FMesh::FMesh(bool bUseIndexBuffer) : bUseIndexBuffer(bUseIndexBuffer), Material(nullptr), Skeleton(nullptr)
{

}

FMesh::~FMesh()
{
	if (Material != nullptr)
		Material = nullptr;
	if (Skeleton != nullptr)
		Skeleton = nullptr;
}

void FMesh::Render()
{
	Render(VertexPositionData, NormalData);
}

void FMesh::Render(const std::vector<glm::vec3>& VertexPositionData, std::vector<glm::vec3>& NormalData)
{
	Material->Apply();

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, VertexPositionData.data());
	glNormalPointer(GL_FLOAT, 0, NormalData.data());
	glTexCoordPointer(2, GL_FLOAT, 0, UVData.data());

	if (bUseIndexBuffer)
		glDrawElements(GL_TRIANGLES, IndicesData.size(), GL_UNSIGNED_SHORT, IndicesData.data());
	else
		glDrawArrays(GL_TRIANGLES, 0, VertexPositionData.size());

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void FMesh::AddIndexData(unsigned short Index)
{
	IndicesData.push_back(Index);
}

void FMesh::AddVertexData(const glm::vec3& Position, const glm::vec3& Normal, const glm::vec2& Texcoord)
{
	VertexPositionData.push_back(Position);
	NormalData.push_back(Normal);
	UVData.push_back(Texcoord);
}

void FMesh::AddSkinnedVertexData(const glm::vec3& Position, const glm::vec3& Normal, const glm::vec2& Texcoord, const FSkinningData& SkinningData)
{
	VertexPositionData.push_back(Position);
	NormalData.push_back(Normal);
	UVData.push_back(Texcoord);
	this->SkinningData.push_back(SkinningData);
}

void FMesh::TransformVertices(const glm::mat4& Matrix, const glm::mat4& TextureMatrix /*= glm::mat4(1.0f)*/)
{
	glm::mat3 NormalMatrix = glm::mat3(Matrix);

	if (TextureMatrix == glm::mat4(1.0f)) // Avoid multiplying texture coordinates if texture matrix is an identity matrix.
	{
		for (size_t i = 0; i < VertexPositionData.size(); i++)
		{
			glm::vec4 Position = Matrix * glm::vec4(VertexPositionData[i], 1.0f);
			VertexPositionData[i] = glm::vec3(Position);
			NormalData[i] = NormalMatrix * NormalData[i];
		}
	}
	else
	{
		for (size_t i = 0; i < VertexPositionData.size(); i++)
		{
			glm::vec4 Position = Matrix * glm::vec4(VertexPositionData[i], 1.0f);
			VertexPositionData[i] = glm::vec3(Position);
			NormalData[i] = NormalMatrix * NormalData[i];
			UVData[i] = glm::vec2(TextureMatrix * glm::vec4(UVData[i], 0.0f, 1.0f));
		}
	}
}
