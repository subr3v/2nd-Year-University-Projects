#ifndef Renderable_h__
#define Renderable_h__

// No virtual destructor as this is not intended to be destructed.
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Geometry.h"

class IRenderable
{
public:

	virtual void Render() = 0;
	virtual void Update(float DeltaTime) { };
	virtual const std::vector<class FMesh*>& GetMeshes() const = 0;

	virtual const std::vector<glm::vec3>& GetMeshVertices(int MeshIndex) const
	{
		return GetMeshes()[MeshIndex]->GetVerticesData();
	}

	bool HasGeometryChanged() const { return bHasGeometryChanged; }
	void MarkGeometryChangeResolved() { bHasGeometryChanged = false; }
protected:
	~IRenderable() { }
	void MarkGeometryDirty() { bHasGeometryChanged = true; }

private:
	bool bHasGeometryChanged;
};

#endif // Renderable_h__
