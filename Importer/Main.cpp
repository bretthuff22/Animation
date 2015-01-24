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
	fopen_s(&pFile, outfileName, "a+");

	fprintf_s(pFile, "MeshCount: %d/n", scene->mNumMeshes);
	fprintf_s(pFile, "TextureCount: %d/n", scene->mNumTextures);

	u32 numVertices = 0;
	u32 numIndices = 0;
	for(u32 meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		aiMesh* aiMesh = scene->mMeshes[meshIndex];
		numVertices += aiMesh->mNumVertices;
		numIndices += aiMesh->mNumFaces * 3;
	}
	fprintf_s(pFile, "VertexCount: %d/n", numVertices);
	fprintf_s(pFile, "IndexCount: %d/n", numIndices);
	
	

	if(scene->HasMeshes())
	{
		for(u32 meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			aiMesh* aiMesh = scene->mMeshes[meshIndex];
			
			//Mesh* mesh = new Mesh();

			//Mesh::Vertex* vertices = new Mesh::Vertex[aiMesh->mNumVertices];
			//Mesh::Vertex* vertexIter = vertices;
			for(u32 i = 0; i < aiMesh->mNumVertices; ++i)
			{
				fprintf_s(pFile, "%d %d %d ", aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z);// * 0.1f;
				if(aiMesh->HasNormals())
				{
					fprintf_s(pFile, "%d %d %d/n", aiMesh->mNormals[i].x, aiMesh->mNormals[i].y, aiMesh->mNormals[i].z);// * 0.1f;
				}
				else
				{
					fprintf_s(pFile, "/n");
				}

			}

			const u32 uvChannelCount = aiMesh->GetNumUVChannels();
			for (u32 i = 0; i < uvChannelCount; ++i)
			{
				//Mesh::Vertex* vertexIter = vertices;
				for (u32 j = 0; j < aiMesh->mNumVertices; ++j)
				{
					fprintf_s(pFile, "%d %d /n", aiMesh->mTextureCoords[i][j].x, aiMesh->mTextureCoords[i][j].y);
				}

				// TODO - Only get the first set of uv for now
				break;
			}

			for(u32 i = 0; i < aiMesh->mNumFaces; ++i)
			{
				fprintf_s(pFile, "%d %d %d /n", aiMesh->mFaces[i].mIndices[0], aiMesh->mFaces[i].mIndices[1], aiMesh->mFaces[i].mIndices[2]);
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
					fprintf_s(pFile, "/n");
				}
			}
		}
	}

	fclose(pFile);

	// Read animation data
	//if(scene->HasAnimations())
	//{
	//	model.mRoot = BuildSkeleton(*scene->mRootNode, model, nullptr);
	//}
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
	const char* outputFile = argv[argc -1];

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

