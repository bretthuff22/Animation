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
		bone->name = aiNode.mName.C_Str();
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
	}

	for(u32 boneIndex = 0; boneIndex < model.mBones.size(); ++boneIndex)
	{
		Bone* bone = model.mBones[boneIndex];

		//		-> name
		fprintf_s(pFile, "Bone name: %s", bone->name.c_str());
		fprintf_s(pFile, "\n");

		//		-> parent index
		fprintf_s(pFile, "Parent: %d\n", bone->parentIndex);

		//		-> children indices
		u16 size = bone->children.size();
		fprintf_s(pFile, "Number of children: %d\n", size);
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
		fprintf_s(pFile, "Offset Transform: %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f \n", 
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


	
// TODO

// Save vertex bone weights
// num, index, weight
	// ex: 2 0 .2500 1 .7500

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

