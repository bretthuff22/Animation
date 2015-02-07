#include "Engine.h"
#include <conio.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


void PrintHelp()
{
	printf("Press any key to exit");
	while(!_kbhit())
	{

	}
}

Bone* BuildSkeleton(aiNode& aiNode, Model& model, Bone* parent)
{
	Bone* bone = nullptr;

	auto iter = model.mBoneIndexMap.find(aiNode.mName.C_Str());
	if(iter == model.mBoneIndexMap.end())
	{
		bone = new Bone();
		if(aiNode.mName.length > 0)
		{
			bone->name = aiNode.mName.C_Str();
		}
		else
		{
			static u32 counter = 0;
			char buffer[128];
			sprintf_s(buffer, 128, "Unknown%d", counter++);
			bone->name = buffer;
		}
		bone->index = model.mBones.size();

		model.mBones.push_back(bone);
		model.mBoneIndexMap.insert(std::make_pair(bone->name, bone->index));
	}
	else
	{
		bone = model.mBones[iter->second];
	}

	bone->transform = *(Math::Matrix*)&(aiNode.mTransformation);
	bone->parent = parent;
	if (parent != nullptr)
	{
		bone->parentIndex = parent->index;
	}

	for(u32 i = 0; i < aiNode.mNumChildren; ++i)
	{
		Bone* child = BuildSkeleton(*(aiNode.mChildren[i]), model, bone);
		bone->children.push_back(child);
	}

	return bone;
}

void ImportModel(const char* infileName, const char* outfileName)
{
	Assimp::Importer importer; 
	u32 flags = aiProcess_Triangulate |
				aiProcess_JoinIdenticalVertices |
				aiProcess_SortByPType |
				aiProcess_FlipUVs;

	const aiScene* scene = importer.ReadFile(infileName, flags);
	ASSERT(scene, "Failed to load model - %s", importer.GetErrorString());

	FILE* pFile = nullptr;
	fopen_s(&pFile, outfileName, "w");

	fprintf_s(pFile, "MeshCount: %d\n", scene->mNumMeshes);
	fprintf_s(pFile, "TextureCount: %d\n", scene->mNumMaterials);	
	
	Model model;

	if(scene->HasMeshes())
	{
		for(u32 meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			aiMesh* aiMesh = scene->mMeshes[meshIndex];
			fprintf_s(pFile, "VertexCount: %d\n", aiMesh->mNumVertices);
			fprintf_s(pFile, "IndexCount: %d\n", aiMesh->mNumFaces * 3);
			
			Mesh* mesh = new Mesh();

			Mesh::Vertex* vertices = new Mesh::Vertex[aiMesh->mNumVertices];
			Mesh::Vertex* vertexIter = vertices;
			for(u32 i = 0; i < aiMesh->mNumVertices; ++i)
			{
				fprintf_s(pFile, "%f %f %f ", aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z);// * 0.1f;
				if(aiMesh->HasNormals())
				{
					fprintf_s(pFile, "%f %f %f\n", aiMesh->mNormals[i].x, aiMesh->mNormals[i].y, aiMesh->mNormals[i].z);// * 0.1f;
				}
				else
				{
					fprintf_s(pFile, "\n");
				}

				
				vertexIter->position.x = aiMesh->mVertices[i].x;// * 0.1f;
				vertexIter->position.y = aiMesh->mVertices[i].y;// * 0.1f;
				vertexIter->position.z = aiMesh->mVertices[i].z;// * 0.1f;
				++vertexIter;

			}

			if(aiMesh->HasNormals())
			{
				vertexIter = vertices;
				for(u32 i = 0; i < aiMesh->mNumVertices; ++i)
				{
					vertexIter->normal.x = aiMesh->mNormals[i].x;
					vertexIter->normal.y = aiMesh->mNormals[i].y;
					vertexIter->normal.z = aiMesh->mNormals[i].z;
					++vertexIter;
				}
			}

			const u32 uvChannelCount = aiMesh->GetNumUVChannels();
			for (u32 i = 0; i < uvChannelCount; ++i)
			{
				Mesh::Vertex* vertexIter = vertices;
				for (u32 j = 0; j < aiMesh->mNumVertices; ++j)
				{
					fprintf_s(pFile, "%f %f\n", aiMesh->mTextureCoords[i][j].x, aiMesh->mTextureCoords[i][j].y);
					
					vertexIter->texcoord.x = aiMesh->mTextureCoords[i][j].x;
					vertexIter->texcoord.y = aiMesh->mTextureCoords[i][j].y;
					++vertexIter;
				}

				// TODO - Only get the first set of uv for now
				break;
			}

			u16* indices = new u16[aiMesh->mNumFaces * 3];
			u16* indexIter = indices;

			for(u32 i = 0; i < aiMesh->mNumFaces; ++i)
			{
				fprintf_s(pFile, "%d %d %d\n", aiMesh->mFaces[i].mIndices[0], aiMesh->mFaces[i].mIndices[1], aiMesh->mFaces[i].mIndices[2]);

				indexIter[0] = aiMesh->mFaces[i].mIndices[0];
				indexIter[1] = aiMesh->mFaces[i].mIndices[1];
				indexIter[2] = aiMesh->mFaces[i].mIndices[2];
				indexIter += 3;
			}

			MeshBuilder::GenerateMesh(*mesh, vertices, aiMesh->mNumVertices, indices, aiMesh->mNumFaces * 3);
			model.mMeshes.push_back(mesh);

			SafeDeleteArray(vertices);
			SafeDeleteArray(indices);

			if(aiMesh->HasBones())
			{
				VertexWeights& vertexWeights = mesh->GetVertexWeights();
				vertexWeights.resize(aiMesh->mNumVertices);

				for(u32 i = 0; i < aiMesh->mNumBones; ++i)
				{
					aiBone* aiBone = aiMesh->mBones[i];
					u32 boneIndex = 0;

					// see if we have already added this bone
					auto iter = model.mBoneIndexMap.find(aiBone->mName.C_Str());
					if(iter != model.mBoneIndexMap.end())
					{
						boneIndex = iter->second;
					}
					else
					{
						boneIndex = model.mBones.size();

						Bone* newBone = new Bone();
						ASSERT(aiBone->mName.length > 0, "Bone %d doesn't have a name!", boneIndex);
						newBone->name = aiBone->mName.C_Str();
						newBone->index = boneIndex;
						newBone->offsetTransform = *(Math::Matrix*)&aiBone->mOffsetMatrix;

						model.mBones.push_back(newBone);
						model.mBoneIndexMap.insert(std::make_pair(aiBone->mName.C_Str(), boneIndex));
					}

					for(u32 j = 0; j < aiBone->mNumWeights; ++j)
					{
						const aiVertexWeight& aiVertexWeight = aiBone->mWeights[j];
						BoneWeight weight;
						weight.boneIndex = boneIndex;
						weight.weight = aiVertexWeight.mWeight;
						vertexWeights[aiVertexWeight.mVertexId].push_back(weight);
					}
				}
			}
		}
	}

	// Read material data
	if(scene->HasMaterials())
	{
		for(u32 i = 0; i < scene->mNumMaterials; ++i)
		{
			aiMaterial* material = scene->mMaterials[i];

			const u32 textureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
			for(u32 j = 0; j < textureCount; ++j)
			{
				aiString texturePath;
				if(material->GetTexture(aiTextureType_DIFFUSE, j, &texturePath) == AI_SUCCESS)
				{
					fprintf_s(pFile, texturePath.C_Str());
					fprintf_s(pFile, "\n");
				}
			}
		}
	}

	// Read animation data
	if(scene->HasAnimations())
	{
		model.mRoot = BuildSkeleton(*scene->mRootNode, model, nullptr);
		fprintf_s(pFile, "NumAnimations: %d\n", scene->mNumAnimations);

		for(u32 animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex)
		{
			aiAnimation* aiAnimation = scene->mAnimations[animIndex];
			AnimationClip* animClip = new AnimationClip();

			animClip->mName = aiAnimation->mName.C_Str();
			animClip->mDuration = (f32)aiAnimation->mDuration;
			animClip->mTicksPerSecond = (f32)aiAnimation->mTicksPerSecond;
			if(animClip->mTicksPerSecond == 0.0f)
			{
				animClip->mTicksPerSecond = 1.0f;
			}

			fprintf_s(pFile, "%s\n", animClip->mName.c_str());
			fprintf_s(pFile, "%f\n", animClip->mDuration);
			fprintf_s(pFile, "%f\n", animClip->mTicksPerSecond);

			fprintf_s(pFile, "NumChannels: %d\n", aiAnimation->mNumChannels);
			for(u32 boneAnimIndex = 0; boneAnimIndex < aiAnimation->mNumChannels; ++boneAnimIndex)
			{
				aiNodeAnim* aiNodeAnim = aiAnimation->mChannels[boneAnimIndex];
				BoneAnimation* boneAnim = new BoneAnimation();

				boneAnim->mBoneIndex = model.mBoneIndexMap.at(aiNodeAnim->mNodeName.C_Str());
				fprintf_s(pFile, "%d\n", boneAnim->mBoneIndex);

				ASSERT(aiNodeAnim->mNumPositionKeys == aiNodeAnim->mNumRotationKeys, "Mismatched key count.");
				ASSERT(aiNodeAnim->mNumPositionKeys == aiNodeAnim->mNumScalingKeys, "Mismatched key count.");

				fprintf_s(pFile, "NumPositionKeys: %d\n", aiNodeAnim->mNumPositionKeys);
				for(u32 keyIndex = 0; keyIndex < aiNodeAnim->mNumPositionKeys; ++keyIndex)
				{
					const aiVectorKey& posKey = aiNodeAnim->mPositionKeys[keyIndex];
					const aiQuatKey& rotKey = aiNodeAnim->mRotationKeys[keyIndex];
					const aiVectorKey& scaleKey = aiNodeAnim->mScalingKeys[keyIndex];

					ASSERT(posKey.mTime == rotKey.mTime, "Mismatched key time");
					ASSERT(posKey.mTime == scaleKey.mTime, "Mismatched key time");

					Keyframe* keyframe = new Keyframe();
					keyframe->mTranslation = Math::Vector3(posKey.mValue.x, posKey.mValue.y, posKey.mValue.z);
					keyframe->mRotation = Math::Quaternion(rotKey.mValue.x, rotKey.mValue.y, rotKey.mValue.z, rotKey.mValue.w);
					keyframe->mScale = Math::Vector3(scaleKey.mValue.x, scaleKey.mValue.y, scaleKey.mValue.z);
					keyframe->mTime = (f32)posKey.mTime;

					fprintf_s(pFile, "%f %f %f\n", posKey.mValue.x, posKey.mValue.y, posKey.mValue.z);
					fprintf_s(pFile, "%f %f %f %f\n", rotKey.mValue.x, rotKey.mValue.y, rotKey.mValue.z, rotKey.mValue.w);
					fprintf_s(pFile, "%f %f %f\n", scaleKey.mValue.x, scaleKey.mValue.y, scaleKey.mValue.z);
					fprintf_s(pFile, "%f\n", (f32)posKey.mTime);

					boneAnim->mKeyframes.push_back(keyframe);
				}

				animClip->mBoneAnimations.push_back(boneAnim);
			}

			model.mAnimations.push_back(animClip);
		}
	}

	fprintf_s(pFile, "NumBones: %d\n", model.mBones.size());

	for(u32 boneIndex = 0; boneIndex < model.mBones.size(); ++boneIndex)
	{
		Bone* bone = model.mBones[boneIndex];

		//		-> name
		fprintf_s(pFile, "BoneName: %s", bone->name.c_str());
		fprintf_s(pFile, "\n");

		//		-> parent index
		fprintf_s(pFile, "Parent: %d\n", bone->parentIndex);

		//		-> children indices
		u16 size = bone->children.size();
		fprintf_s(pFile, "NumberOfChildren: %d\n", size);
		for(u16 i = 0; i < size; ++i)
		{
			fprintf_s(pFile, "%d ", bone->children[i]->index);
			if(i == size - 1)
			{
				fprintf_s(pFile, "\n");
			}
		}

		//		-> transform
		fprintf_s(pFile, "Transform: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f \n", 
				  bone->transform._11, bone->transform._12, bone->transform._13, bone->transform._14, 
				  bone->transform._21, bone->transform._22, bone->transform._23, bone->transform._24, 
				  bone->transform._31, bone->transform._32, bone->transform._33, bone->transform._34, 
				  bone->transform._41, bone->transform._42, bone->transform._43, bone->transform._44); 
		//		-> offset transform
		fprintf_s(pFile, "OffsetTransform: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f \n", 
				  bone->offsetTransform._11, bone->offsetTransform._12, bone->offsetTransform._13, bone->offsetTransform._14, 
				  bone->offsetTransform._21, bone->offsetTransform._22, bone->offsetTransform._23, bone->offsetTransform._24, 
				  bone->offsetTransform._31, bone->offsetTransform._32, bone->offsetTransform._33, bone->offsetTransform._34, 
				  bone->offsetTransform._41, bone->offsetTransform._42, bone->offsetTransform._43, bone->offsetTransform._44); 

		fprintf_s(pFile, "\n");
	}

	for(u32 meshIndex = 0; meshIndex < model.mMeshes.size(); ++meshIndex)
	{
		Mesh* mesh = model.mMeshes[meshIndex];
		const VertexWeights& vertexWeights = mesh->GetVertexWeights();
		u32 numVertexWeights = vertexWeights.size();
		fprintf_s(pFile, "NumVertexWeights: %d\n", numVertexWeights);

		for(u32 weightIndex = 0; weightIndex < numVertexWeights; ++weightIndex)
		{
			fprintf_s(pFile, "%d ", vertexWeights[weightIndex].size());
			for(u32 bIndex = 0; bIndex < vertexWeights[weightIndex].size(); ++bIndex)
			{
				fprintf_s(pFile, "%d %f ", vertexWeights[weightIndex][bIndex].boneIndex, vertexWeights[weightIndex][bIndex].weight);
			}
			fprintf_s(pFile, "\n");
		}
	}


	fclose(pFile);

	
}

int main(int argc, const char* argv[])
{
	if(argc < 3)
	{
		printf("[Importer] - too few arguments.\n");
		PrintHelp();
		return 0;
	}

	printf("[Importer] starting...");

	std::string filename(argv[1]);
	const char* inputFile = argv[argc - 2];
	const char* outputFile = argv[argc - 1];

	//Model model;
	//std::vector<std::string> textureNames;

	printf("Importing model...");

	ImportModel(inputFile, outputFile);

	printf("[Importer] finished.");

	printf("Press any key to exit.");
	while(!_kbhit())
	{

	}

	return 0;
}

