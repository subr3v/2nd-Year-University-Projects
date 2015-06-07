#ifndef ShadowVolume_h__
#define ShadowVolume_h__

#include "Renderable.h"
#include <glm/glm.hpp>
#include <vector>

struct FPlane
{
	float a, b, c, d;
	glm::vec3 v1, v2, v3;

	FPlane(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) : v1(v1), v2(v2), v3(v3)
	{
		a = v1.y * (v2.z - v3.z) + v2.y * (v3.z - v1.z) + v3.y * (v1.z - v2.z);
		b = v1.z * (v2.x - v3.x) + v2.z * (v3.x - v1.x) + v3.z * (v1.x - v2.x);
		c = v1.x * (v2.y - v3.y) + v2.x * (v3.y - v1.y) + v3.x * (v1.y - v2.y);
		d = -(v1.x * (v2.y * v3.z - v3.y * v2.z) +  v2.x * (v3.y * v1.z - v1.y * v3.z) + v3.x * (v1.y * v2.z - v2.y * v1.z));
	}

	inline bool IsOnSameSide(const glm::vec3& LightPosition) const
	{
		return (LightPosition.x * a + LightPosition.y * b + LightPosition.z * c + d) > 0;
	}
};

struct FEdge
{
	glm::vec3 v0, v1;
	bool bIsValid;

	FEdge(const glm::vec3& v0, const glm::vec3& v1) : v0(v0), v1(v1), bIsValid(true) { }

	inline bool operator==(const FEdge& Other) const
	{
		return (v0 == Other.v0 && v1 == Other.v1) || (v0 == Other.v1 && v1 == Other.v0);
	}
};

class FShadowVolume
{
public:
	FShadowVolume(class FMesh* Mesh, const glm::mat4& Transform);
	~FShadowVolume();

	void Render();

	void UpdateGeometry(const std::vector<glm::vec3>& Vertices, const glm::mat4& Transform);
	void EvaluateLightSource(bool bIsPointLight, const glm::vec3& LightPosition);
	void SetColour(const glm::vec3& Colour) { this->ShadowColour = Colour; }

private:
	glm::vec3 ShadowColour;

	void ExtrudeEdgePair(int TargetIndex, const glm::vec3& LightPosition, float ExtensionMagnitude, const glm::vec3& v0, const glm::vec3& v1);
	inline glm::vec3 ExtendVertex(const glm::vec3& LightPosition, glm::vec3 Vertex, float ExtensionMagnitude);

	std::vector<glm::vec3> VertexPositionData;

	std::vector<FEdge> SilhouetteEdges;
	std::vector<FPlane> MeshPlanes;
	class FMesh* Mesh;
};

#endif // ShadowVolume_h__
