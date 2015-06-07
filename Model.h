
#ifndef _MODEL_H_
#define _MODEL_H_

#include <vector>
#include <fbxsdk.h>
#include <glm/glm.hpp>
#include <map>
#include "ModelData.h"

class FModel
{
public:
	FModel(const std::string& Filename);
	~FModel();

	void Render();

	class FSkeleton* GetSkeleton() { return ModelData.Skeleton; }
	const std::vector<class FSkeletalAnimation*>& GetAnimations();
private:
	FModelData ModelData;
};

#endif