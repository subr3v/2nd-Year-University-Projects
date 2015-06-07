#ifndef ModelData_h__
#define ModelData_h__

#include <map>
#include <vector>
#include <fbxsdk.h>

struct FModelData
{
	class FSkeleton* Skeleton;
	std::vector<class FSkeletalAnimation*> Animations;
	std::vector<class FMesh*> Meshes;
	std::vector<class FMaterial*> Materials;

	FModelData();
	~FModelData();
};

class IModelLoader
{
public:
	IModelLoader() { }
	virtual ~IModelLoader() { }
	virtual bool LoadModel(const std::string& FilePath, FModelData& OutData) = 0;
	virtual FSkeletalAnimation* LoadAnimation(const std::string& FilePath, FModelData& OutData) { return nullptr; }
};

class FFBXModelLoader : public IModelLoader
{
public:
	FFBXModelLoader();
	~FFBXModelLoader();

	virtual bool LoadModel(const std::string& FilePath, FModelData& OutData) override;
	virtual FSkeletalAnimation* LoadAnimation(const std::string& FilePath, FModelData& OutData) override;
private:
	void LoadSkeleton(FbxNode* RootNode, FModelData& OutData);
	class FSkeletalAnimation* LoadAnimation(FbxScene* Scene, FModelData& OutData);

	// Mesh loading routines.
	void LoadMeshRecursive(FbxNode* Node, std::map<FbxSurfaceMaterial*, class FMaterial*>& MaterialsMapping, FModelData& OutData);

	// Skeleton Loading routines
	void TraverseHierarchySkeleton(FbxNode* CurrentNode, int CurrentDepth, int CurrentIndex, int ParentIndex, FModelData& OutData);
	void TraverseHierarchyLoadSkeleton(FbxNode* CurrentNode, FModelData& OutData);

	// Animation loading routines
	void TraverseNodeAnimationData(FbxNode* CurrentNode, FbxTime Start, FbxTime End, FModelData& OutData, FSkeletalAnimation* Animation);

	FbxManager* SdkManager;
	FbxIOSettings* IOSettings;
};

#endif // ModelData_h__
