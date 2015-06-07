#include "ModelData.h"
#include "Mesh.h"
#include "Material.h"
#include "Skeleton.h"
#include <glm/glm.hpp>

glm::mat4 FromFbxMatrix(FbxAMatrix Matrix)
{
	glm::mat4 NewMatrix;
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			NewMatrix[x][y] = (float)Matrix[x][y];
		}
	}
	return NewMatrix;
}

FModelData::FModelData() : Skeleton(nullptr)
{
	
}

FModelData::~FModelData()
{
	delete Skeleton;
	Skeleton = nullptr;

	for (size_t i = 0; i < Meshes.size(); i++)
	{
		delete Meshes[i];
	}
	Meshes.clear();

	for (size_t i = 0; i < Materials.size(); i++)
	{
		delete Materials[i];
	}
	Materials.clear();

	for (size_t i = 0; i < Animations.size(); i++)
	{
		delete Animations[i];
	}
	Animations.clear();
}

FFBXModelLoader::FFBXModelLoader()
{
	// Initialize the SDK manager. This object handles memory management.
	SdkManager = FbxManager::Create();

	// Create the IO settings object.
	IOSettings = FbxIOSettings::Create(SdkManager, IOSROOT);
	IOSettings->SetBoolProp(IMP_FBX_ANIMATION, true);
	IOSettings->SetBoolProp(IMP_FBX_LINK, true);
	SdkManager->SetIOSettings(IOSettings);
}

FFBXModelLoader::~FFBXModelLoader()
{
	SdkManager->Destroy();
}

bool FFBXModelLoader::LoadModel(const std::string& FilePath, FModelData& OutData)
{
	// Create an importer using the SDK manager.
	FbxImporter* Importer = FbxImporter::Create(SdkManager, "");

	// Use the first argument as the filename for the importer.
	if (!Importer->Initialize(FilePath.c_str(), -1, SdkManager->GetIOSettings()))
	{
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", Importer->GetStatus().GetErrorString());
		Importer->Destroy();
		return false;
	}

	// Create a new scene so that it can be populated by the imported file.
	FbxScene* Scene = FbxScene::Create(SdkManager, "ImportScene");

	// Import the contents of the file into the scene.
	Importer->Import(Scene);

	FbxAnimStack* AnimationStack = Scene->GetSrcObject<FbxAnimStack>(0);
	//SdkManager->GetAnimationEvaluator()->SetContext(AnimationStack);

	std::map<FbxSurfaceMaterial*, FMaterial*> MaterialMapping;

	FbxNode* RootNode = Scene->GetRootNode();
	if (RootNode)
	{
		OutData.Skeleton = new FSkeleton();
		LoadSkeleton(RootNode, OutData);

		for (int i = 0; i < RootNode->GetChildCount(); i++)
		{
			FbxNode* ChildNode = RootNode->GetChild(i);
			LoadMeshRecursive(ChildNode, MaterialMapping, OutData);
		}
	}

	LoadAnimation(Scene, OutData);

	Importer->Destroy();

	return true;
}

FSkeletalAnimation* FFBXModelLoader::LoadAnimation(const std::string& FilePath, FModelData& OutData)
{
	// Create an importer using the SDK manager.
	FbxImporter* Importer = FbxImporter::Create(SdkManager, "");

	// Use the first argument as the filename for the importer.
	if (!Importer->Initialize(FilePath.c_str(), -1, SdkManager->GetIOSettings()))
	{
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", Importer->GetStatus().GetErrorString());
		Importer->Destroy();
		return false;
	}

	// Create a new scene so that it can be populated by the imported file.
	FbxScene* Scene = FbxScene::Create(SdkManager, "ImportScene");

	// Import the contents of the file into the scene.
	Importer->Import(Scene);

	FbxAnimStack* AnimationStack = Scene->GetSrcObject<FbxAnimStack>(0);
	//SdkManager->GetAnimationEvaluator()->SetContext(AnimationStack);

	FSkeletalAnimation* Animation = LoadAnimation(Scene, OutData);

	Importer->Destroy();

	return Animation;
}

class FSkeletalAnimation* FFBXModelLoader::LoadAnimation(FbxScene* Scene, FModelData& OutData)
{
	int AnimStackCount = Scene->GetSrcObjectCount<FbxAnimStack>();
	FbxAnimStack* AnimationStack = Scene->GetSrcObject<FbxAnimStack>(0);
	FbxString AnimationStackName = AnimationStack->GetName();
	FbxTakeInfo* TakeInfo = Scene->GetTakeInfo(AnimationStackName);
	FbxTime AnimationStart = TakeInfo->mLocalTimeSpan.GetStart();
	FbxTime AnimationEnd = TakeInfo->mLocalTimeSpan.GetStop();
	FbxLongLong AnimationLength = AnimationEnd.GetFrameCount(FbxTime::eFrames24) - AnimationStart.GetFrameCount(FbxTime::eFrames24) + 1;

	if (AnimStackCount > 0)
	{
		FSkeletalAnimation* SkeletalAnimation = new FSkeletalAnimation(OutData.Skeleton);
		OutData.Animations.push_back(SkeletalAnimation);
		SkeletalAnimation->Name = AnimationStackName.Buffer();
		SkeletalAnimation->Keyframes.resize(AnimationLength + 1);
		SkeletalAnimation->Duration = (AnimationEnd - AnimationStart).GetSecondCount();

		for (size_t i = 0; i < SkeletalAnimation->Keyframes.size(); i++)
		{
			SkeletalAnimation->Keyframes[i].BonesTransform.resize(OutData.Skeleton->Bones.size());
		}

		TraverseNodeAnimationData(Scene->GetRootNode(), AnimationStart, AnimationEnd, OutData, SkeletalAnimation);
		return SkeletalAnimation;
	}

	return nullptr;
}

void FFBXModelLoader::LoadMeshRecursive(FbxNode* ChildNode, std::map<FbxSurfaceMaterial*, class FMaterial*>& MaterialsMapping, FModelData& OutData)
{
	struct FControlPoint
	{
		FSkinningData SkinningData;
		FbxVector4 Position;
	};

	if (ChildNode->GetNodeAttribute() != NULL)
	{
		FbxNodeAttribute::EType AttributeType = ChildNode->GetNodeAttribute()->GetAttributeType();

		if (AttributeType == FbxNodeAttribute::eMesh)
		{
			FbxMesh* Mesh = (FbxMesh*)ChildNode->GetNodeAttribute();

			FbxLayerElementArrayTemplate<FbxVector2>* UVVertices = 0;
			Mesh->GetTextureUV(&UVVertices, FbxLayerElement::eTextureDiffuse);

			FMesh* CurrentMesh = new FMesh(false);
			CurrentMesh->SetSkeleton(OutData.Skeleton);
			OutData.Meshes.push_back(CurrentMesh);

			assert(ChildNode->GetMaterialCount() >= 1);
			FbxSurfaceMaterial* Material = ChildNode->GetMaterial(0);

			if (MaterialsMapping[Material] == nullptr)
			{
				FbxProperty DiffuseTexture = Material->FindProperty(FbxSurfaceMaterial::sDiffuse);
				int TextureCount = DiffuseTexture.GetSrcObjectCount<FbxTexture>();
				if (TextureCount > 0)
				{
					FbxFileTexture* Texture = (FbxFileTexture*)DiffuseTexture.GetSrcObject<FbxTexture>(0);
					FMaterial* CurrentMaterial = new FMaterial(Texture->GetRelativeFileName());
					MaterialsMapping[Material] = CurrentMaterial;
					OutData.Materials.push_back(CurrentMaterial);
				}
				else
				{
					FMaterial* CurrentMaterial = new FMaterial();
					MaterialsMapping[Material] = CurrentMaterial;
					OutData.Materials.push_back(CurrentMaterial);
				}
			}

			CurrentMesh->SetMaterial(MaterialsMapping[Material]);
			FbxAMatrix GlobalTransformationMatrix = ChildNode->EvaluateGlobalTransform();
			glm::mat4 TransformationMatrix = FromFbxMatrix(GlobalTransformationMatrix);

			// Gather info about all the control points first.
			int ControlPointsCount = Mesh->GetControlPointsCount();
			std::vector<FControlPoint> ControlPoints(ControlPointsCount);

			for (int j = 0; j < ControlPointsCount; j++)
			{
				ControlPoints[j].Position = Mesh->GetControlPointAt(j);
			}

			// Load skinning data.
			for (int j = 0; j < Mesh->GetDeformerCount(FbxDeformer::eSkin); j++)
			{
				FbxSkin* SkinDeformer = (FbxSkin*)Mesh->GetDeformer(j, FbxDeformer::eSkin);
				for (int k = 0; k < SkinDeformer->GetClusterCount(); k++)
				{
					FbxCluster* Cluster = SkinDeformer->GetCluster(k);
					std::string BoneName = Cluster->GetLink()->GetName();
					int BoneIndex = OutData.Skeleton->FindBoneIndexByName(BoneName);

					FbxAMatrix transformMatrix;
					FbxAMatrix transformLinkMatrix;
					FbxAMatrix globalBindposeInverseMatrix;

					for (int pointIndex = 0; pointIndex < Cluster->GetControlPointIndicesCount(); pointIndex++)
					{
						int VertexIndex = Cluster->GetControlPointIndices()[pointIndex];
						float Weight = (float)Cluster->GetControlPointWeights()[pointIndex];
						ControlPoints[VertexIndex].SkinningData.BoneInfluences.push_back(FWeightIndexPair(Weight, BoneIndex));
					}
				}
			}

			for (int j = 0; j < Mesh->GetPolygonCount(); j++)
			{
				int iNumVertices = Mesh->GetPolygonSize(j);

				assert(iNumVertices == 3);

				for (int k = 0; k < iNumVertices; k++)
				{
					int vertexIndex = Mesh->GetPolygonVertex(j, k);
					int textureIndex = Mesh->GetTextureUVIndex(j, k);

					FbxVector4 FbxPosition = ControlPoints[vertexIndex].Position;
					glm::vec4 Position = glm::vec4(FbxPosition[0], FbxPosition[1], FbxPosition[2], 1.0f);
					if (OutData.Skeleton->Bones.size() == 0)
					{
						// Transformation matrix is only supported for static meshes.
						Position = TransformationMatrix * Position;
					}
					FbxVector4 Normal;
					Mesh->GetPolygonVertexNormal(j, k, Normal);
					FbxVector2 UV = UVVertices->GetAt(textureIndex);

					CurrentMesh->AddSkinnedVertexData(glm::vec3(Position), glm::vec3(Normal[0], Normal[1], Normal[2]), glm::vec2(UV[0], UV[1]), ControlPoints[vertexIndex].SkinningData);
				}
			}
		}
	}

	for (int i = 0; i < ChildNode->GetChildCount(); i++)
	{
		FbxNode* Child = ChildNode->GetChild(i);
		LoadMeshRecursive(Child, MaterialsMapping, OutData);
	}
}

void FFBXModelLoader::TraverseHierarchySkeleton(FbxNode* CurrentNode, int CurrentDepth, int CurrentIndex, int ParentIndex, FModelData& OutData)
{
	if (CurrentNode->GetNodeAttribute() && CurrentNode->GetNodeAttribute()->GetAttributeType())
	{
		if (CurrentNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			FBone CurrentBone(CurrentNode->GetName(), ParentIndex);
			OutData.Skeleton->Bones.push_back(CurrentBone);
		}
	}
	for (int i = 0; i < CurrentNode->GetChildCount(); i++)
	{
		FbxNode* ChildNode = CurrentNode->GetChild(i);
		TraverseHierarchySkeleton(ChildNode, CurrentDepth + 1, OutData.Skeleton->Bones.size(), OutData.Skeleton->Bones.size() == 0 ? -1 : CurrentIndex, OutData);
	}
}

void FFBXModelLoader::TraverseHierarchyLoadSkeleton(FbxNode* CurrentNode, FModelData& OutData)
{
	if (CurrentNode->GetNodeAttribute() != NULL)
	{
		FbxNodeAttribute::EType AttributeType = CurrentNode->GetNodeAttribute()->GetAttributeType();

		if (AttributeType == FbxNodeAttribute::eMesh)
		{
			FbxMesh* Mesh = (FbxMesh*)CurrentNode->GetNodeAttribute();

			for (int j = 0; j < Mesh->GetDeformerCount(FbxDeformer::eSkin); j++)
			{
				FbxSkin* SkinDeformer = (FbxSkin*)Mesh->GetDeformer(j, FbxDeformer::eSkin);
				for (int k = 0; k < SkinDeformer->GetClusterCount(); k++)
				{
					FbxCluster* Cluster = SkinDeformer->GetCluster(k);
					std::string BoneName = Cluster->GetLink()->GetName();
					FBone* Bone = OutData.Skeleton->FindBoneByName(BoneName);
					FbxNode* BoneNode = Cluster->GetLink();

					FbxAMatrix BindPoseMatrix;
					FbxAMatrix LocalMatrix;

					Cluster->GetTransformMatrix(LocalMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space
					Cluster->GetTransformLinkMatrix(BindPoseMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space
					FbxAMatrix InverseBindPoseMatrix = BindPoseMatrix.Inverse() * LocalMatrix;
					FbxAMatrix GlobalTransform = CurrentNode->EvaluateGlobalTransform().Inverse() * BoneNode->EvaluateGlobalTransform();

					Bone->InverseBindPose = FromFbxMatrix(InverseBindPoseMatrix);
					Bone->BindPoseTransform = FromFbxMatrix(GlobalTransform);
				}
			}
		}
	}

	for (int i = 0; i < CurrentNode->GetChildCount(); i++)
	{
		FbxNode* ChildNode = CurrentNode->GetChild(i);
		TraverseHierarchyLoadSkeleton(ChildNode, OutData);
	}
}

void FFBXModelLoader::LoadSkeleton(FbxNode* RootNode, FModelData& OutData)
{
	for (int i = 0; i < RootNode->GetChildCount(); i++)
	{
		FbxNode* ChildNode = RootNode->GetChild(i);
		TraverseHierarchySkeleton(ChildNode, 0, 0, -1, OutData);
	}

	for (int i = 0; i < RootNode->GetChildCount(); i++)
	{
		FbxNode* ChildNode = RootNode->GetChild(i);
		TraverseHierarchyLoadSkeleton(ChildNode, OutData);
	}
}

void FFBXModelLoader::TraverseNodeAnimationData(FbxNode* CurrentNode, FbxTime Start, FbxTime End, FModelData& OutData, FSkeletalAnimation* Animation)
{
	if (CurrentNode->GetNodeAttribute() != NULL)
	{
		FbxNodeAttribute::EType AttributeType = CurrentNode->GetNodeAttribute()->GetAttributeType();
		if (AttributeType == FbxNodeAttribute::eMesh)
		{
			FbxMesh* Mesh = (FbxMesh*)CurrentNode->GetNodeAttribute();
			for (int j = 0; j < Mesh->GetDeformerCount(FbxDeformer::eSkin); j++)
			{
				FbxSkin* SkinDeformer = (FbxSkin*)Mesh->GetDeformer(j, FbxDeformer::eSkin);
				for (int k = 0; k < SkinDeformer->GetClusterCount(); k++)
				{
					FbxCluster* Cluster = SkinDeformer->GetCluster(k);
					std::string BoneName = Cluster->GetLink()->GetName();
					int BoneIndex = OutData.Skeleton->FindBoneIndexByName(BoneName);
					FbxNode* BoneNode = Cluster->GetLink();

					for (FbxLongLong t = Start.GetFrameCount(FbxTime::eFrames24); t < End.GetFrameCount(FbxTime::eFrames24); ++t)
					{
						FbxTime currTime;
						currTime.SetFrame(t, FbxTime::eFrames24);
						FbxAMatrix BoneMatrix = CurrentNode->EvaluateGlobalTransform(currTime).Inverse() * BoneNode->EvaluateGlobalTransform(currTime);
						Animation->Keyframes[t].BonesTransform[BoneIndex] = FromFbxMatrix(BoneMatrix);
					}
				}
			}
		}
	}

	for (int i = 0; i < CurrentNode->GetChildCount(); i++)
	{
		FbxNode* ChildNode = CurrentNode->GetChild(i);
		TraverseNodeAnimationData(ChildNode, Start, End, OutData, Animation);
	}
}
