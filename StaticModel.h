#ifndef StaticModel_h__
#define StaticModel_h__

#include <fbxsdk.h>
#include <string>
#include <vector>
#include <map>
#include "Renderable.h"
#include "ModelData.h"
#include "DisplayList.h"

class FStaticModel : public IRenderable
{
public:
	FStaticModel(FModelData* ModelData);
	~FStaticModel();

	virtual void Render() override;
	virtual void Update(float DeltaTime) override { }

	virtual const std::vector<class FMesh*>& GetMeshes() const override;

private:
	std::vector<class FShadowVolume*> ShadowVolumes;
	FModelData* ModelData;
	FOpenGLDisplayList* DisplayList;
};


#endif // StaticModel_h__
