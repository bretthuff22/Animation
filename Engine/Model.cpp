#include "Precompiled.h"
#include "Model.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "MeshRenderer.h"
#include "GraphicsSystem.h"


Model::Model()
{

}

Model::~Model()
{

}

void Model::Load(GraphicsSystem& gs, const char* pFileName)
{
	// Model loading
	FILE* pFile = nullptr;
	fopen_s(&pFile, pFileName, "r");

	
	
	
	
	// Texture loading 

	// "../Data/Models/myModel.txt"
	std::string path = pFileName;
	// "../Data/Models/"
	u32 slashIndex = path.find_last_of('/');
	path = path.erase(slashIndex + 1);

	//for(u32 t = 0; t < numTextures; ++t)
	//{
	//	Texture* texture = new Texture();
	//	char buffer[256];
	//	fscanf_s(pFile, "%s\n", buffer, 256);
	//	std::string fullpath = path;
	//	fullpath += buffer;

	//	wchar_t wbuffer[256];
	//	size_t numConverted = 0;
	//	mbstowcs_s(&numConverted, wbuffer, fullpath.c_str(), 256);
	//	texture->Initialize(gs, wbuffer);
	//	mTextures.push_back(texture);
	//}

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

void Model::Render(MeshRenderer& renderer)
{
	for (u32 i = 0; i < (u32)mMeshBuffers.size(); ++i)
	{
		renderer.SetTexture(*mTextures[i]);
		renderer.Render(*mMeshBuffers[i], Math::Matrix::Identity());
	}
}
