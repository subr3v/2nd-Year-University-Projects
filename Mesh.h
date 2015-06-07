#ifndef Mesh_h__
#define Mesh_h__

#include <vector>
#include <glm/glm.hpp>
#include "Geometry.h"

struct FWeightIndexPair
{
	float Weight;
	int BoneIndex;

	FWeightIndexPair() { }

	FWeightIndexPair(float Weight, int BoneIndex) : Weight(Weight), BoneIndex(BoneIndex)
	{

	}
};

struct FSkinningData
{
	std::vector<FWeightIndexPair> BoneInfluences;
};

class FMesh
{
public:
	FMesh(bool bUseIndexBuffer);
	~FMesh();

	void AddIndexData(unsigned short Index);
	void AddVertexData(const glm::vec3& Position, const glm::vec3& Normal, const glm::vec2& Texcoord);
	void AddSkinnedVertexData(const glm::vec3& Position, const glm::vec3& Normal, const glm::vec2& Texcoord, const FSkinningData& SkinningData);
	void SetMaterial(class FMaterial* Material) { this->Material = Material; }
	void SetSkeleton(class FSkeleton* Skeleton) { this->Skeleton = Skeleton; }

	void TransformVertices(const glm::mat4& Matrix, const glm::mat4& TextureMatrix = glm::mat4(1.0f));

	void Render();

	// This is used for skinned animation, where displaced vertices and normals are provided from an external source.
	void Render(const std::vector<glm::vec3>& VertexPositionData, std::vector<glm::vec3>& NormalData);

	std::vector<glm::vec3>& GetVerticesData() { return VertexPositionData; }
	std::vector<glm::vec3>& GetNormalData() { return NormalData; }

	const std::vector<unsigned short>& GetIndicesData() const { return IndicesData; }
	const std::vector<glm::vec3>& GetVerticesData() const { return VertexPositionData; }
	const std::vector<glm::vec3>& GetNormalData() const { return NormalData; }
	const std::vector<FSkinningData>& GetSkinningData() const { return SkinningData; }

	bool UsesIndexBuffer() const { return bUseIndexBuffer; }

	class FMaterial* GetMaterial() { return Material; }
private:
	bool bUseIndexBuffer;

	class FMaterial* Material;
	class FSkeleton* Skeleton;

	std::vector<unsigned short> IndicesData;

	std::vector<glm::vec3> AnimatedVertexPositionData;
	std::vector<glm::vec3> AnimatedVertexNormalData;

	std::vector<glm::vec3> VertexPositionData;
	std::vector<glm::vec3> NormalData;
	std::vector<glm::vec2> UVData;
	std::vector<FSkinningData> SkinningData;
};

#endif // Mesh_h__
