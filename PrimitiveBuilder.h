#ifndef PrimitiveBuilder_h__
#define PrimitiveBuilder_h__

#include "glm/glm.hpp"
#include <vector>
#include "Renderable.h"

struct FTransform
{
	FTransform(const glm::vec3& Position = glm::vec3(0.0f), const glm::vec3& Rotation = glm::vec3(0.0f), const glm::vec3& Scale = glm::vec3(1.0f)) : Position(Position), Rotation(Rotation), Scale(Scale) { }
	operator glm::mat4() const;

	glm::vec3 Position, Rotation, Scale;
};

class FProceduralModel : public IRenderable
{
public:
	~FProceduralModel();

	void Render() override;
	const std::vector<class FMesh*>& GetMeshes() const override { return Meshes; }

private:
	FProceduralModel();
	virtual void Update(float DeltaTime) override { }

	friend class FPrimitiveBuilder;

	std::vector<class FShadowVolume*> ShadowVolumes;
	std::vector<class FMesh*> Meshes;
	std::vector<class FMaterial*> Materials;
};

class FPrimitiveBuilder
{
public:
	FPrimitiveBuilder();

	// Generates a plane with the specified material, transform and colour.
	class FMesh* GeneratePlane(class FTexture* Texture, const glm::mat4& Transform, int Width, int Height, float CellSize, const glm::vec4& Colour = glm::vec4(1.0f));
	
	// Generates a unit cube with the specified material, transform and colour.
	class FMesh* GenerateCube(class FTexture* Texture, const glm::mat4& Transform, const glm::vec4& Colour = glm::vec4(1.0f));
	
	// Generates a unit sphere with the specified material, transform and colour.
	class FMesh* GenerateSphere(int SubdivisionNumber, class FTexture* Texture, const glm::mat4& Transform, const glm::vec4& Colour = glm::vec4(1.0f));

	// Generates a unit disc with the specified material, transform and colour.
	class FMesh* GenerateDisc(int SubdivisionNumber, class FTexture* Texture, const glm::mat4& Transform, const glm::vec4& Colour = glm::vec4(1.0f));

	FProceduralModel* GetGeneratedModel() { return ProceduralModel;	}

private:
	class FMesh* CreateMesh(class FTexture* Texture, const glm::vec4& Colour, bool bUseIndexBuffer = false);
	FProceduralModel* ProceduralModel;
};

#endif // PrimitiveBuilder_h__
