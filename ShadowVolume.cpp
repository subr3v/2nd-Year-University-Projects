#include "ShadowVolume.h"

#include <Windows.h>
#include <gl/GL.h>
#include "Mesh.h"

#include "tbb/tbb.h"

FShadowVolume::FShadowVolume(class FMesh* Mesh, const glm::mat4& Transform) : ShadowColour(glm::vec3(1.0f, 1.0f, 1.0f)), Mesh(Mesh)
{
	if (Mesh->UsesIndexBuffer())
	{
		const std::vector<unsigned short>& MeshIndices = Mesh->GetIndicesData();
		const std::vector<glm::vec3>& MeshVertices = Mesh->GetVerticesData();

		for (int i = 0; i < MeshIndices.size(); i += 3)
		{
			// Define the edge we're currently working on extruding...
			int e0 = i;
			int e1 = i + 1;
			int e2 = i + 2;

			const glm::vec3& v0 = glm::vec3(Transform * glm::vec4(MeshVertices[MeshIndices[e0]], 1.0f));
			const glm::vec3& v1 = glm::vec3(Transform * glm::vec4(MeshVertices[MeshIndices[e1]], 1.0f));
			const glm::vec3& v2 = glm::vec3(Transform * glm::vec4(MeshVertices[MeshIndices[e2]], 1.0f));

			FPlane TrianglePlane(v0, v1, v2);
			MeshPlanes.emplace_back(TrianglePlane);
		}
	}
	else
	{
		const std::vector<glm::vec3>& MeshVertices = Mesh->GetVerticesData();
		for (int i = 0; i < MeshVertices.size(); i += 3)
		{
			// Define the edge we're currently working on extruding...
			int e0 = i;
			int e1 = i + 1;
			int e2 = i + 2;

			const glm::vec3& v0 = glm::vec3(Transform * glm::vec4(MeshVertices[e0], 1.0f));
			const glm::vec3& v1 = glm::vec3(Transform * glm::vec4(MeshVertices[e1], 1.0f));
			const glm::vec3& v2 = glm::vec3(Transform * glm::vec4(MeshVertices[e2], 1.0f));

			FPlane TrianglePlane(v0, v1, v2);
			MeshPlanes.emplace_back(TrianglePlane);
		}
	}
}

void FShadowVolume::UpdateGeometry(const std::vector<glm::vec3>& Vertices, const glm::mat4& Transform)
{
	const std::vector<glm::vec3>& MeshVertices = Mesh->GetVerticesData();
	for (int i = 0; i < MeshVertices.size(); i += 3)
	{
		// Define the edge we're currently working on extruding...
		int e0 = i;
		int e1 = i + 1;
		int e2 = i + 2;

		const glm::vec3& v0 = glm::vec3(Transform * glm::vec4(Vertices[e0], 1.0f));
		const glm::vec3& v1 = glm::vec3(Transform * glm::vec4(Vertices[e1], 1.0f));
		const glm::vec3& v2 = glm::vec3(Transform * glm::vec4(Vertices[e2], 1.0f));

		FPlane TrianglePlane(v0, v1, v2);
		MeshPlanes[i / 3] = TrianglePlane;
	}
}

FShadowVolume::~FShadowVolume()
{

}

void FShadowVolume::EvaluateLightSource(bool bIsPointLight, const glm::vec3& LightPosition)
{
	VertexPositionData.clear();
	SilhouetteEdges.clear();

	const float kShadowInfinity = 1000.0f;

	for (size_t i = 0; i < MeshPlanes.size(); i++)
	{
		const FPlane& TrianglePlane = MeshPlanes[i];

		if (TrianglePlane.IsOnSameSide(LightPosition))
		{
			SilhouetteEdges.push_back(FEdge(TrianglePlane.v1, TrianglePlane.v2));
			SilhouetteEdges.push_back(FEdge(TrianglePlane.v2, TrianglePlane.v3));
			SilhouetteEdges.push_back(FEdge(TrianglePlane.v3, TrianglePlane.v1));
		}
	}

	size_t EdgesNumber = SilhouetteEdges.size();

	size_t ValidEdges = EdgesNumber;

	for (size_t i = 0; i < EdgesNumber; i++)
	{
		if (SilhouetteEdges[i].bIsValid == false)
			continue;;

		for (size_t k = i + 1; k < EdgesNumber; k++)
		{
			if (SilhouetteEdges[i] == SilhouetteEdges[k])
			{
				SilhouetteEdges[i].bIsValid = SilhouetteEdges[k].bIsValid = false;
				ValidEdges -= 2;
				break;
			}
		}
	}

	VertexPositionData.resize(ValidEdges * 4);
	
	tbb::atomic<int>* VertexIndex = new tbb::atomic<int>();
	*VertexIndex = 0;

	tbb::parallel_for<int>(0, EdgesNumber, 
		[this, LightPosition, kShadowInfinity, VertexIndex] (int i)
	{
		if (SilhouetteEdges[i].bIsValid)
		{
			int Value = (*VertexIndex)++;
			ExtrudeEdgePair(Value * 4, LightPosition, kShadowInfinity, SilhouetteEdges[i].v0, SilhouetteEdges[i].v1);
		}
	});
	
	delete VertexIndex;

	/*
	int VertexIndex = 0;
	for (size_t i = 0; i < EdgesNumber; i++)
	{
		if (SilhouetteEdges[i].bIsValid)
		{
			int TargetIndex = VertexIndex++;
			ExtrudeEdgePair(TargetIndex * 4, LightPosition, kShadowInfinity, SilhouetteEdges[i].v0, SilhouetteEdges[i].v1);
		}
	}
	*/
}

void FShadowVolume::ExtrudeEdgePair(int Index, const glm::vec3& LightPosition, float ExtensionMagnitude, const glm::vec3& v0, const glm::vec3& v1)
{
	// v0 of our extruded quad will simply use the edge's first 
	// vertex or e0.
	VertexPositionData[Index] = v0;

	// v1 of our quad is created by taking the edge's first 
	// vertex and extending it out by some amount.
	VertexPositionData[Index + 1] = ExtendVertex(LightPosition, v0, ExtensionMagnitude);

	// v2 of our quad is created by taking the edge's second 
	// vertex and extending it out by some amount.
	VertexPositionData[Index + 2] = ExtendVertex(LightPosition, v1, ExtensionMagnitude);

	// v3 of our extruded quad will simply use the edge's second 
	// vertex or e1.
	VertexPositionData[Index + 3] = v1;
}

void FShadowVolume::Render()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glColor4f(ShadowColour.r, ShadowColour.g, ShadowColour.b, 0.2f);

	glVertexPointer(3, GL_FLOAT, 0, VertexPositionData.data());
	glDrawArrays(GL_QUADS, 0, VertexPositionData.size());

	glDisableClientState(GL_VERTEX_ARRAY);
}

glm::vec3 FShadowVolume::ExtendVertex(const glm::vec3& LightPosition, glm::vec3 Vertex, float ExtensionMagnitude)
{
	glm::vec3 LightDirection = Vertex - LightPosition;
	return LightPosition + LightDirection * ExtensionMagnitude;
}
