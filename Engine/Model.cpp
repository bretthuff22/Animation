#include "Precompiled.h"
#include "Model.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "MeshBuilder.h"
#include "MeshRenderer.h"
#include "GraphicsSystem.h"
#include "Bone.h"
#include "AnimationClip.h"
#include "BoneAnimation.h"


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
		
			indexIter += 3;
		}

		MeshBuilder::GenerateMesh(*mesh, vertices, numVertices, indices, numIndices);
		mMeshes.push_back(mesh);

		MeshBuffer* meshBuffer = new MeshBuffer();
		meshBuffer->Initialize(gs, Mesh::GetVertexFormat(), *mesh, true);
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

	// ANIMATIONS
	u32 numAnimations = 0;
	fscanf_s(pFile, "%s", buffer, 256);
	fscanf_s(pFile, "%d\n", &numAnimations);
	

	for(u32 animIndex = 0; animIndex < numAnimations; ++animIndex)
	{
		AnimationClip* animClip = new AnimationClip();

		fscanf_s(pFile, "%s\n", buffer, 256);
		animClip->mName = buffer;

		fscanf_s(pFile, "%f\n", &animClip->mDuration);
		fscanf_s(pFile, "%f\n", &animClip->mTicksPerSecond);

		fscanf_s(pFile, "%s", buffer, 256);
		u32 numChannels = 0;
		fscanf_s(pFile, "%d\n", &numChannels);
		
		for(u32 boneAnimIndex = 0; boneAnimIndex < numChannels; ++boneAnimIndex)
		{
			BoneAnimation* boneAnim = new BoneAnimation();

			fscanf_s(pFile, "%d\n", &boneAnim->mBoneIndex);

			u32 numPositionKeys = 0;
			fscanf_s(pFile, "%s", buffer, 256);
			fscanf_s(pFile, "%d\n", &numPositionKeys);

			for(u32 keyIndex = 0; keyIndex < numPositionKeys; ++keyIndex)
			{
				Keyframe* keyframe = new Keyframe();

				fscanf_s(pFile, "%f %f %f\n", &keyframe->mTranslation.x, &keyframe->mTranslation.y, &keyframe->mTranslation.z);
				fscanf_s(pFile, "%f %f %f %f\n", &keyframe->mRotation.x, &keyframe->mRotation.y, &keyframe->mRotation.z, &keyframe->mRotation.w);
				fscanf_s(pFile, "%f %f %f\n", &keyframe->mScale.x, &keyframe->mScale.y, &keyframe->mScale.z);
				fscanf_s(pFile, "%f\n", &keyframe->mTime);

				boneAnim->mKeyframes.push_back(keyframe);
			}

			animClip->mBoneAnimations.push_back(boneAnim);
		}

		mAnimations.push_back(animClip);
	}

	// BONES
	u32 numBones = 0;
	fscanf_s(pFile, "%s", buffer, 256);
	fscanf_s(pFile, "%d\n", &numBones, 256);
	//mBones.resize(numBones);

	for (u32 i = 0; i < numBones; ++i)
	{
		Bone* bone = new Bone();

		//		-> name
		fscanf_s(pFile, "%s", buffer, 256);
		fscanf_s(pFile, "%s\n", buffer, 256);
		bone->name = buffer;
		//		-> parent index
		fscanf_s(pFile, "%s", buffer, 256);
		fscanf_s(pFile, "%d\n", &bone->parentIndex, 256);
		
		
		//		-> children indices
		u32 size = 0;
		fscanf_s(pFile, "%s", buffer, 256);
		fscanf_s(pFile, "%d\n", &size, 256);
		bone->childrenIndex.resize(size);

		for (u32 j = 0; j < size; ++j)
		{
			u32 childIndex = 0;
			fscanf_s(pFile, "%d ", &childIndex, 256);
			bone->childrenIndex[j] = childIndex;
			if (j == size - 1)
			{
				fscanf_s(pFile, "\n");
			}
		}

		//		-> transform
		fscanf_s(pFile, "%s", buffer, 256);
		fscanf_s(pFile, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f \n",
			&bone->transform._11, &bone->transform._12,&bone->transform._13, &bone->transform._14,
			&bone->transform._21, &bone->transform._22,&bone->transform._23, &bone->transform._24,
			&bone->transform._31, &bone->transform._32,&bone->transform._33, &bone->transform._34,
			&bone->transform._41, &bone->transform._42,&bone->transform._43, &bone->transform._44, 256);
		//		-> offset transform
		fscanf_s(pFile, "%s", buffer, 256);
		fscanf_s(pFile, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f \n",
			&bone->offsetTransform._11, &bone->offsetTransform._12, &bone->offsetTransform._13, &bone->offsetTransform._14,
			&bone->offsetTransform._21, &bone->offsetTransform._22, &bone->offsetTransform._23, &bone->offsetTransform._24,
			&bone->offsetTransform._31, &bone->offsetTransform._32, &bone->offsetTransform._33, &bone->offsetTransform._34,
			&bone->offsetTransform._41, &bone->offsetTransform._42, &bone->offsetTransform._43, &bone->offsetTransform._44, 256);

		bone->transform = Math::Transpose(bone->transform);
		bone->offsetTransform = Math::Transpose(bone->offsetTransform);

		fscanf_s(pFile, "\n");

		bone->index = i;
		 
		mBones.push_back(bone); // 67 bones
	}

	Bone* boneIter = mBones.front();
	for (u32 i = 0; i < numBones; ++i)
	{
		if (boneIter->parentIndex != -1)
		{
			boneIter->parent = mBones[boneIter->parentIndex];
		}
		else
		{
			mRoot = boneIter;
		}

		for (u32 j = 0; j < boneIter->childrenIndex.size(); ++j)
		{
			boneIter->children.push_back(mBones[boneIter->childrenIndex[j]]);
		}

		if (i + 1 < numBones)
		{
			boneIter = mBones[i + 1];
		}
	}

	// match bones to animations using indices
	for (u32 animIndex = 0; animIndex < numAnimations; ++animIndex)
	{
		for (u32 boneAnimIndex = 0; boneAnimIndex < mAnimations[animIndex]->mBoneAnimations.size(); ++boneAnimIndex)
		{
			u16 boneIndex = mAnimations[animIndex]->mBoneAnimations[boneAnimIndex]->mBoneIndex;
			mAnimations[animIndex]->mBoneAnimations[boneAnimIndex]->mBone = mBones[boneAnimIndex];
		}
	}
	
	//WEIGHTS
	for (u32 meshIndex = 0; meshIndex < mMeshes.size(); ++meshIndex)
	{
		u32 numVertexWeights = 0; 
		fscanf_s(pFile, "%s", buffer, 256);
		fscanf_s(pFile, "%d\n", &numVertexWeights, 256);
		mMeshes[meshIndex]->GetVertexWeights().resize(numVertexWeights);
		

		for (u32 weightIndex = 0; weightIndex < numVertexWeights; ++weightIndex)
		{
			u32 numWeights = 0;
			fscanf_s(pFile, "%d ", &numWeights);
			mMeshes[meshIndex]->GetVertexWeights()[weightIndex].resize(numWeights);

			for (u32 bIndex = 0; bIndex < numWeights; ++bIndex)
			{
				u32 boneIndex = 0;
				f32 weight = 0.0f;
				fscanf_s(pFile, "%d %f ", &boneIndex, &weight, 256);
				mMeshes[meshIndex]->GetVertexWeights()[weightIndex][bIndex].boneIndex = boneIndex;
				mMeshes[meshIndex]->GetVertexWeights()[weightIndex][bIndex].weight = weight;

			}
			fscanf_s(pFile, "\n");
		}
	}
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
