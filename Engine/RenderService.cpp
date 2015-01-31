//====================================================================================================
// Filename:	RenderService.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "RenderService.h"

#include "GameObject.h"
#include "Repository.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

RenderService::RenderService()
	: mGameObjectRepository(nullptr)
	, mTransformRepository(nullptr)
	, mModelRepository(nullptr)
{
}

//----------------------------------------------------------------------------------------------------

RenderService::~RenderService()
{
}

//----------------------------------------------------------------------------------------------------

void RenderService::Initialize(GraphicsSystem& gs,
	Camera& camera,
	GameObjectRepository& gor,
	TransformRepository& tr,
	ModelRepository& mr)
{
	mRenderer.Initialize(gs);
	mRenderer.SetCamera(camera);

	mGameObjectRepository = &gor;
	mTransformRepository = &tr;
	mModelRepository = &mr;
}

//----------------------------------------------------------------------------------------------------

void RenderService::Terminate()
{
	mModelRepository = nullptr;
	mTransformRepository = nullptr;
	mGameObjectRepository = nullptr;

	mRenderer.Terminate();
}

//----------------------------------------------------------------------------------------------------

void RenderService::Update()
{
	for (Subscribers::iterator iter = mSubscribers.begin();
		iter != mSubscribers.end();
		++iter)
	{
		RenderInfo& info = iter->second;
		TransformComponent& transformComponent = mTransformRepository->GetItem(info.transformId);
		ModelComponent& modelComponent = mModelRepository->GetItem(info.modelId);

		const Math::Matrix& transform = transformComponent.GetTransform();
		const Model* model = modelComponent.GetModel();
		model->Render(mRenderer); //model->Render(mRenderer, transform);
	}
}

//----------------------------------------------------------------------------------------------------

void RenderService::OnSubscribe(ID objId, RenderInfo& info)
{
	GameObject& object = mGameObjectRepository->GetItem(objId);
	info.transformId = object.GetComponentID(Meta::TransformComponentType);
	info.modelId = object.GetComponentID(Meta::ModelComponentType);
}