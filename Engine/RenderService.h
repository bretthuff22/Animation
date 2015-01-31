#ifndef INCLUDED_ENGINE_RENDERSERVICE_H
#define INCLUDED_ENGINE_RENDERSERVICE_H

//====================================================================================================
// Filename:	RenderService.h
// Created by:	Peter Chan
// Description:	Class for the render service.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "MeshRenderer.h"
#include "RepositoryTypes.h"
#include "Service.h"

//====================================================================================================
// Structs
//====================================================================================================

struct RenderInfo
{
	ID transformId;
	ID modelId;
};

//====================================================================================================
// Class Declarations
//====================================================================================================

class RenderService : public Service<RenderInfo>
{
public:
	RenderService();
	~RenderService();

	void Initialize(GraphicsSystem& gs,
		Camera& camera,
		GameObjectRepository& gor,
		TransformRepository& tr,
		ModelRepository& mr);
	void Terminate();

	void Update();

private:
	virtual void OnSubscribe(ID objId, RenderInfo& info);

	GameObjectRepository* mGameObjectRepository;
	TransformRepository* mTransformRepository;
	ModelRepository* mModelRepository;

	MeshRenderer mRenderer;
};

#endif // #ifndef INCLUDED_ENGINE_RENDERSERVICE_H