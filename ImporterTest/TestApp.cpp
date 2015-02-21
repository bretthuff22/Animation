//====================================================================================================
// Filename:	TestApp.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "TestApp.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//====================================================================================================
// Class Definitions
//====================================================================================================

TestApp::TestApp()
	: mMouseX(-1)
	, mMouseY(-1)
	, mDrawSkeleton(false)
{
	memset(mKeyStates, 0, sizeof(mKeyStates));
}

//----------------------------------------------------------------------------------------------------

TestApp::~TestApp()
{
}

void TestApp::DrawSkeleton()
{
	// Draw the skeleton
		// Get all the toRoot transforms
		// for each transform, extract 41, 42, 43 as position 
		// draw AABB
		const std::vector<Math::Matrix>& toRootTransforms = mAnimationController.ToRootTransforms();
		// get vector of bones
		const std::vector<Bone*>& bones = mModel.mBones;

		Color RandColor((float)(rand()%100)/100, (float)(rand()%100)/100, (float)(rand()%100)/100, 1.0f );
		for(u32 i = 0; i < toRootTransforms.size(); ++i)
		{
			const Math::Matrix& mat = toRootTransforms[i];
			const Math::Vector3 center( mat._41, mat._42, mat._43 );
			
			//SimpleDraw::AddAABB(center, 0.01f, Color((float)(rand()%100)/100, (float)(rand()%100)/100, (float)(rand()%100)/100, 1.0f ));
			SimpleDraw::AddAABB(center, 0.02f, RandColor);

			for(u32 j = 0; j < bones[i]->children.size(); ++j)
			{
				const Math::Matrix& child = toRootTransforms[bones[i]->children[j]->index];
				const Math::Vector3 childCenter( child._41, child._42, child._43 );
				SimpleDraw::AddLine(center, childCenter, RandColor);
			}
			// draw bones to children
		}
}

//----------------------------------------------------------------------------------------------------

void TestApp::OnInitialize(u32 width, u32 height)
{
	mWindow.Initialize(GetInstance(), GetAppName(), width, height);
	HookWindow(mWindow.GetWindowHandle());

	mTimer.Initialize();

	mGraphicsSystem.Initialize(mWindow.GetWindowHandle(), false);
	SimpleDraw::Initialize(mGraphicsSystem);
	
	const u32 windowWidth = mGraphicsSystem.GetWidth();
	const u32 windowHeight = mGraphicsSystem.GetHeight();

	mCamera.Setup(Math::kPiByTwo, (f32)windowWidth / (f32)windowHeight, 0.01f, 1000.0f);
	mCamera.SetPosition(Math::Vector3(0.0f, 2.0f, 1.0f));
	mCamera.SetLookAt(Math::Vector3(0.0f, 1.0f, 0.0f));

	mRenderer.Initialize(mGraphicsSystem);

	mModel.Load(mGraphicsSystem, "../Data/Models/soldier1.txt");
	
	mAnimationController.Initialize(mModel);
	//AnimationClip clip;
	//mAnimationController.StartClip(clip, true);
	mAnimationController.StartClip(*mModel.mAnimations[0], true);
}

//----------------------------------------------------------------------------------------------------

void TestApp::OnTerminate()
{
	mModel.Unload();

	mRenderer.Terminate();
	SimpleDraw::Terminate();
	mGraphicsSystem.Terminate();
	mWindow.Terminate();
}

//----------------------------------------------------------------------------------------------------

bool TestApp::OnInput(const InputEvent& evt)
{
	bool handled = false;

	switch (evt.type)
	{
	case InputEvent::KeyDown:
		{
			mKeyStates[evt.value] = true;
			handled = true;
			
			if (mKeyStates[VK_ESCAPE])
			{
				PostQuitMessage(0);
			}
			else if (mKeyStates[VK_F1])
			{
				mGraphicsSystem.ToggleFullscreen();
			}
		}
		break;
	case InputEvent::KeyUp:
		{
			mKeyStates[evt.value] = false;
			handled = true;
		}
		break;
	case InputEvent::MouseMove:
		{
			if (mMouseX != -1 && mMouseY != -1)
			{
				f32 deltaX = (f32)(evt.x - mMouseX);
				f32 deltaY = (f32)(evt.y - mMouseY);
				mCamera.Yaw(deltaX);
				mCamera.Pitch(deltaY);
			}
			mMouseX = evt.x;
			mMouseY = evt.y;
			handled = true;
		}
		break;
	}

	return handled;
}

//----------------------------------------------------------------------------------------------------

void TestApp::OnUpdate()
{
	if (mWindow.HandleMessage())
	{
		mRunning = false;
	}
	else
	{
		// Update our time
		mTimer.Update();

		// Update animation
		mAnimationController.Update(mTimer.GetElapsedTime() * 1000.0f);

		// Camera movement
		const float kMoveSpeed = 10.0f;
		const float kTurnSpeed = 5.0f;
		if (mKeyStates[VK_UP] || mKeyStates['W'])
		{
			mCamera.Walk(kMoveSpeed * mTimer.GetElapsedTime());
		}
		else if (mKeyStates[VK_DOWN] || mKeyStates['S'])
		{
			mCamera.Walk(-kMoveSpeed * mTimer.GetElapsedTime());
		}
		else if (mKeyStates[VK_RIGHT] || mKeyStates['D'])
		{
			mCamera.Strafe(kMoveSpeed * mTimer.GetElapsedTime());
		}
		else if (mKeyStates[VK_LEFT] || mKeyStates['A'])
		{
			mCamera.Strafe(-kMoveSpeed * mTimer.GetElapsedTime());
		}
		else if (mKeyStates['K'])
		{
			mDrawSkeleton = !mDrawSkeleton;
		}
		
		// Render scene
		mGraphicsSystem.BeginRender(Color::Black());

		mRenderer.SetCamera(mCamera);

		if(mDrawSkeleton)
		{
			DrawSkeleton();
		}

		// for each mesh
		for(u32 i = 0; i < mModel.mMeshes.size(); ++i)
		{
			const Mesh* mesh = mModel.mMeshes[i];
			// copy vertexweights
			const VertexWeights& vertexWeights = mesh->GetVertexWeights();

			// if vw not empty
			if(!vertexWeights.empty())
			{
				const std::vector<Math::Matrix>& boneTransforms = mAnimationController.BoneTransforms();
				// copy vertices
				const Mesh::Vertex* vertices = mesh->GetVertices();
				// get vertex count
				const u32 count = mesh->GetVertexCount();
				// create new vertexarray of size vertex count
				Mesh::Vertex* newVertices = new Mesh::Vertex[count];
				// for each vertex
				for(u32 j = 0; j < count; ++j)
				{
					// create zero transform
					Math::Matrix transform;
					transform = transform.Zero();

					// copy boneweights from this vertexweight
					const BoneWeights& boneWeights = vertexWeights[j];

					if(i == 1)
					{
						transform = boneTransforms[22];
					}
					else
					{
						// for each boneweight
						for(u32 k = 0; k < boneWeights.size(); ++k)
						{
							const BoneWeight& boneWeight = boneWeights[k];
						
							transform = transform + boneTransforms[i == 1 ? 22 : boneWeight.boneIndex] * boneWeight.weight;
						}
					}
					// insert position into newVertices 
					newVertices[j].position = Math::TransformCoord(vertices[j].position, transform);
					// insert normal into newVertices 
					newVertices[j].normal = Math::Normalize(Math::TransformNormal(vertices[j].normal, transform));
					// insert texcoord into newVertices
					newVertices[j].texcoord = vertices[j].texcoord;
				}
				// update mesh buffer at index with newVertices
				mModel.mMeshBuffers[i]->UpdateBuffer(mGraphicsSystem, newVertices, count);
				// safedelete newVertices
				SafeDeleteArray(newVertices);
			}
		}

		mModel.Render(mRenderer);

		SimpleDraw::Render(mCamera);

		mGraphicsSystem.EndRender();
	}

	
}

void TestApp::LoadModel(const char* filename, Model& model)
{
	Assimp::Importer importer; 
	u32 flags = aiProcess_Triangulate |
				aiProcess_JoinIdenticalVertices |
				aiProcess_SortByPType |
				aiProcess_FlipUVs;

	const aiScene* scene = importer.ReadFile(filename, flags);
	ASSERT(scene, "Failed to load model - %s", importer.GetErrorString());

	if(scene->HasMeshes())
	{
		for(u32 meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			aiMesh* aiMesh = scene->mMeshes[meshIndex];
			Mesh* mesh = new Mesh();

			Mesh::Vertex* vertices = new Mesh::Vertex[aiMesh->mNumVertices];
			Mesh::Vertex* vertexIter = vertices;
			for(u32 i = 0; i < aiMesh->mNumVertices; ++i)
			{
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
				indexIter[0] = aiMesh->mFaces[i].mIndices[0];
				indexIter[1] = aiMesh->mFaces[i].mIndices[1];
				indexIter[2] = aiMesh->mFaces[i].mIndices[2];
				indexIter += 3;
			}

			MeshBuilder::GenerateMesh(*mesh, vertices, aiMesh->mNumVertices, indices, aiMesh->mNumFaces * 3);
			mModel.mMeshes.push_back(mesh);

			SafeDeleteArray(vertices);
			SafeDeleteArray(indices);

			MeshBuffer* meshBuffer = new MeshBuffer();
			meshBuffer->Initialize(mGraphicsSystem, Mesh::GetVertexFormat(), *mesh, true);
			mModel.mMeshBuffers.push_back(meshBuffer);
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
					textureNames.push_back(texturePath.C_Str());
				}
			}
		}
	}

	// Read animation data
	//if(scene->HasAnimations())
	//{
	//	model.mRoot = BuildSkeleton(*scene->mRootNode, model, nullptr);
	//}
}