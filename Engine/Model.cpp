#include "Precompiled.h"
#include "Model.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "MeshBuilder.h"
#include "MeshRenderer.h"
#include "GraphicsSystem.h"


Model::Model()
{

}

Model::~Model()
{
	for (u32 i = 0; i < (u32)mBones.size(); ++i)
	{
		SafeDelete(mBones[i]);
	}
	mBones.clear();
}

void Model::Load(GraphicsSystem& gs, const char* pFileName)
{
	// Model loading
	FILE* pFile = nullptr;
	fopen_s(&pFile, pFileName, "r");
	u32 numMesh = 0;
	u32 numTextures = 0;
	u32 numVertices = 0;
	u32 numIndices = 0;
	
	char buffer[256];
	fscanf_s(pFile, "%s", buffer, 256);
	fscanf_s(pFile, "%d\n", &numMesh, 256);
	fscanf_s(pFile, "%s", buffer, 256);
	fscanf_s(pFile, "%d\n", &numTextures, 256); 
	
	

	for (u32 t = 0; t < numMesh; ++t)
	{
		Mesh* mesh = new Mesh();

		fscanf_s(pFile, "%s", buffer, 256);
		fscanf_s(pFile, "%d\n", &numVertices, 256);
		fscanf_s(pFile, "%s", buffer, 256);
		fscanf_s(pFile, "%d\n", &numIndices, 256);

		Mesh::Vertex* vertices = new Mesh::Vertex[numVertices];
		Mesh::Vertex* vertexIter = vertices;

		for (u32 i = 0; i < numVertices; ++i)
		{
			fscanf_s(pFile, "%f", &vertexIter->position.x, 256);
			fscanf_s(pFile, "%f", &vertexIter->position.y, 256);
			fscanf_s(pFile, "%f", &vertexIter->position.z, 256);
			if (fgetc(pFile) != '\n')
			{
				fscanf_s(pFile, "%f", &vertexIter->normal.x, 256);
				fscanf_s(pFile, "%f", &vertexIter->normal.y, 256);
				fscanf_s(pFile, "%f\n", &vertexIter->normal.z, 256);
			}
			vertexIter++;
		}

		vertexIter = vertices;

		for (u32 i = 0; i < numVertices; ++i)
		{
			fscanf_s(pFile, "%f", &vertexIter->texcoord.x, 256);
			fscanf_s(pFile, "%f\n", &vertexIter->texcoord.y, 256);
			vertexIter++;
		}

		u16* indices = new u16[numIndices];
		u16* indexIter = indices;
		u16 numFaces = numIndices / 3;
		for (u32 i = 0; i < numFaces; ++i)
		{
			fscanf_s(pFile, "%d", &indexIter[0], 256);
			fscanf_s(pFile, "%d", &indexIter[1], 256);
			fscanf_s(pFile, "%d/n", &indexIter[2], 256);
		
			if (i == numFaces - 1)
			{
				bool boob = false;
			}
			indexIter += 3;
		}

		MeshBuilder::GenerateMesh(*mesh, vertices, numVertices, indices, numIndices);
		mMeshes.push_back(mesh);

		//SafeDeleteArray(vertices);
		//SafeDeleteArray(indices);

		MeshBuffer* meshBuffer = new MeshBuffer();
		meshBuffer->Initialize(gs, Mesh::GetVertexFormat(), *mesh);
		mMeshBuffers.push_back(meshBuffer);

	}


	// Texture loading 

	// "../Data/Models/myModel.txt"
	std::string path = pFileName;
	// "../Data/Models/"
	u32 slashIndex = path.find_last_of('/');
	path = path.erase(slashIndex + 1);

	for(u32 t = 0; t < numTextures; ++t)
	{
		Texture* texture = new Texture();
		char buffer[256];
		fscanf_s(pFile, "%s\n", buffer, 256);
		std::string fullpath = path;
		fullpath += buffer;

		wchar_t wbuffer[256];
		size_t numConverted = 0;
		mbstowcs_s(&numConverted, wbuffer, fullpath.c_str(), 256);
		texture->Initialize(gs, wbuffer);
		mTextures.push_back(texture);
	}

	//TODO: BONES
	//TODO: WEIGHTS

	fclose(pFile);
}

void Model::Unload()
{
	for(u32 i = 0; i < (u32)mMeshes.size(); ++i)
	{
		mMeshes[i]->Destroy();
		SafeDelete(mMeshes[i]);
	}
	mMeshes.clear();

	for(u32 i = 0; i < (u32)mMeshBuffers.size(); ++i)
	{
		mMeshBuffers[i]->Terminate();
		SafeDelete(mMeshBuffers[i]);
	}
	mMeshBuffers.clear();

	for(u32 i = 0; i < (u32)mTextures.size(); ++i)
	{
		mTextures[i]->Terminate();
		SafeDelete(mTextures[i]);
	}
	mTextures.clear();
}

void Model::Render(MeshRenderer& renderer) const 
{
	for (u32 i = 0; i < (u32)mMeshBuffers.size(); ++i)
	{
		renderer.SetTexture(*mTextures[i]);
		renderer.Render(*mMeshBuffers[i], Math::Matrix::Identity());
	}
}
