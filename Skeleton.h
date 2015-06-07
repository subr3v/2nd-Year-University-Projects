#ifndef Skeleton_h__
#define Skeleton_h__

#include <string>
#include <vector>
#include <glm/glm.hpp>

class FBone
{
public:
	std::string Name;
	int ParentIndex;
	glm::mat4 InverseBindPose;
	glm::mat4 BindPoseTransform;

	FBone(const std::string& Name, int ParentIndex) : Name(Name), ParentIndex(ParentIndex) { }
	FBone() { }
};

struct FAnimationKeyframe
{
	std::vector<glm::mat4> BonesTransform;
};

class FSkeletalAnimation
{
public:
	std::string Name;
	int Duration;
	std::vector<FAnimationKeyframe> Keyframes;
	class FSkeleton* Skeleton;

	FSkeletalAnimation(class FSkeleton* Skeleton);
};

class FSkeleton
{
public:
	FSkeleton();
	std::vector<FBone> Bones;

	FBone* FindBoneByName(const std::string& name);
	int FindBoneIndexByName(const std::string& name) const;
};

class FSkeletonDebugger
{
public:
	void DrawSkeleton(FSkeleton* Skeleton);
	void DrawSkeletonAnimation(FSkeletalAnimation* Animation, int Frame);

private:
	void DrawBone(const glm::mat4& Transform);
};

#endif // Skeleton_h__
