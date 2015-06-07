#include "Skeleton.h"
#include "Model.h"

FSkeleton::FSkeleton() { }

FBone* FSkeleton::FindBoneByName(const std::string& name)
{
	for (size_t i = 0; i < Bones.size(); i++)
	{
		if (Bones[i].Name == name)
			return &Bones[i];
	}
	return nullptr;
}

int FSkeleton::FindBoneIndexByName(const std::string& name) const
{
	for (size_t i = 0; i < Bones.size(); i++)
	{
		if (Bones[i].Name == name)
			return i;
	}
	return -1;
}

FSkeletalAnimation::FSkeletalAnimation(class FSkeleton* Skeleton) : Skeleton(Skeleton) { }

void FSkeletonDebugger::DrawSkeleton(FSkeleton* Skeleton)
{
	for (size_t i = 0; i < Skeleton->Bones.size(); i++)
	{
		DrawBone(Skeleton->Bones[i].BindPoseTransform);
	}
}

void FSkeletonDebugger::DrawSkeletonAnimation(FSkeletalAnimation* Animation, int Frame)
{
	FSkeleton* Skeleton = Animation->Skeleton;
	for (size_t i = 0; i < Skeleton->Bones.size(); i++)
	{
		DrawBone(Animation->Keyframes[Frame].BonesTransform[i]);
	}
}

void FSkeletonDebugger::DrawBone(const glm::mat4& Transform)
{
	// Draw cube
}
