#include "SkinnedModel.h"
#include "Skeleton.h"
#include "Mesh.h"
#include "ShadowVolume.h"
#include "tbb/tbb.h"

FSkinnedModel::FSkinnedModel(FModelData* ModelData) : ModelData(ModelData), CurrentAnimation(nullptr), CurrentAnimationFrame(1), CurrentFrameTime(0.0f)
{
	if (ModelData != nullptr && ModelData->Animations.size() > 0)
	{
		// Default animation is the first that was loaded with the model.
		CurrentAnimation = ModelData->Animations[0];

		// Preallocate space for animated vertices data and matrices.
		GlobalTransforms.resize(ModelData->Skeleton->Bones.size());

		for (size_t i = 0; i < ModelData->Meshes.size(); i++)
		{
			AnimatedMeshesData.push_back(FAnimatedMeshData());
			FAnimatedMeshData& MeshData = AnimatedMeshesData.back();
			int NumVertices = ModelData->Meshes[i]->GetVerticesData().size();
			MeshData.AnimatedVertexPositionData.resize(NumVertices);
			MeshData.AnimatedVertexNormalData.resize(NumVertices);
		}
	}
}

FSkinnedModel::~FSkinnedModel()
{
	CurrentAnimation = nullptr;

	for (size_t i = 0; i < ShadowVolumes.size(); i++)
	{
		delete ShadowVolumes[i];
	}
	ShadowVolumes.clear();
}

void FSkinnedModel::Update(float DeltaTime)
{
	if (CurrentAnimation != nullptr)
	{
		CurrentFrameTime += DeltaTime;
		float FrameDuration = 1.0f / (CurrentAnimation->Duration * 12.0f);
		if (CurrentFrameTime >= FrameDuration)
		{
			CurrentAnimationFrame++;
			if (CurrentAnimationFrame >= CurrentAnimation->Keyframes.size() - 1)
				CurrentAnimationFrame = 1;
			CurrentFrameTime -= FrameDuration;
			PerformSoftwareSkinning_Parallel();
		}
	}
}

void FSkinnedModel::Render()
{
	for (size_t i = 0; i < ModelData->Meshes.size(); i++)
	{
		ModelData->Meshes[i]->Render(AnimatedMeshesData[i].AnimatedVertexPositionData, AnimatedMeshesData[i].AnimatedVertexNormalData);
	}
}

void FSkinnedModel::SetCurrentAnimation(FSkeletalAnimation* Animation)
{
	this->CurrentAnimation = Animation;
}

void FSkinnedModel::PerformSoftwareSkinning_Parallel()
{
	FSkeleton* Skeleton = ModelData->Skeleton;
	const std::vector<glm::mat4> BoneTransforms = CurrentAnimation->Keyframes[CurrentAnimationFrame].BonesTransform;

	tbb::parallel_for<int>(0, ModelData->Meshes.size(), [this, Skeleton, BoneTransforms] (int i)
	{
		FAnimatedMeshData& AnimatedMeshData = AnimatedMeshesData[i];

		const std::vector<glm::vec3>& VertexPositionData = ModelData->Meshes[i]->GetVerticesData();
		const std::vector<glm::vec3>& NormalData = ModelData->Meshes[i]->GetNormalData();
		const std::vector<FSkinningData>& SkinningData = ModelData->Meshes[i]->GetSkinningData();

		for (size_t i = 0; i < GlobalTransforms.size(); i++)
		{
			FBone* Bone = &Skeleton->Bones[i];
			GlobalTransforms[i] = (BoneTransforms[i] * Bone->InverseBindPose); // TODO: Premultiply this.
		}

		int NumVertices = AnimatedMeshData.AnimatedVertexPositionData.size();

		for (size_t i = 0; i < NumVertices; i++)
		{
			// Animate skinned vertex.
			glm::mat4 BoneTransform = glm::mat4(1.0f);
			for (size_t k = 0; k < SkinningData[i].BoneInfluences.size(); k++)
			{
				FWeightIndexPair WeightInformation = SkinningData[i].BoneInfluences[k];
				BoneTransform *= GlobalTransforms[WeightInformation.BoneIndex] * WeightInformation.Weight;
			}

			glm::vec4 TransformedPosition = BoneTransform * glm::vec4(VertexPositionData[i], 1.0f);
			glm::vec4 TransformedNormal = BoneTransform * glm::vec4(NormalData[i], 0.0f);

			AnimatedMeshData.AnimatedVertexPositionData[i] = glm::vec3(TransformedPosition);
			AnimatedMeshData.AnimatedVertexNormalData[i] = glm::vec3(TransformedNormal);
		}
	});

	MarkGeometryDirty();
}

void FSkinnedModel::PerformSoftwareSkinning()
{
	FSkeleton* Skeleton = ModelData->Skeleton;
	const std::vector<glm::mat4> BoneTransforms = CurrentAnimation->Keyframes[CurrentAnimationFrame].BonesTransform;

	for (size_t i = 0; i < ModelData->Meshes.size(); i++)
	{
		FAnimatedMeshData& AnimatedMeshData = AnimatedMeshesData[i];

		const std::vector<glm::vec3>& VertexPositionData = ModelData->Meshes[i]->GetVerticesData();
		const std::vector<glm::vec3>& NormalData = ModelData->Meshes[i]->GetNormalData();
		const std::vector<FSkinningData>& SkinningData = ModelData->Meshes[i]->GetSkinningData();

		for (size_t i = 0; i < GlobalTransforms.size(); i++)
		{
			FBone* Bone = &Skeleton->Bones[i];
			GlobalTransforms[i] = (BoneTransforms[i] * Bone->InverseBindPose); // TODO: Premultiply this.
		}

		int NumVertices = AnimatedMeshData.AnimatedVertexPositionData.size();

		for (size_t i = 0; i < NumVertices; i++)
		{
			// Animate skinned vertex.
			glm::mat4 BoneTransform = glm::mat4(1.0f);
			for (size_t k = 0; k < SkinningData[i].BoneInfluences.size(); k++)
			{
				FWeightIndexPair WeightInformation = SkinningData[i].BoneInfluences[k];
				BoneTransform *= GlobalTransforms[WeightInformation.BoneIndex] * WeightInformation.Weight;
			}

			glm::vec4 TransformedPosition = BoneTransform * glm::vec4(VertexPositionData[i], 1.0f);
			glm::vec4 TransformedNormal = BoneTransform * glm::vec4(NormalData[i], 0.0f);

			AnimatedMeshData.AnimatedVertexPositionData[i] = glm::vec3(TransformedPosition);
			AnimatedMeshData.AnimatedVertexNormalData[i] = glm::vec3(TransformedNormal);
		}
	}

	MarkGeometryDirty();
}

const std::vector<class FMesh*>& FSkinnedModel::GetMeshes() const
{
	return ModelData->Meshes;
}

const std::vector<glm::vec3>& FSkinnedModel::GetMeshVertices(int MeshIndex) const
{
	return AnimatedMeshesData[MeshIndex].AnimatedVertexPositionData;
}

