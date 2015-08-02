#ifndef INCLUDED_ENGINE_REPOSITORYTYPES_H
#define INCLUDED_ENGINE_REPOSITORYTYPES_H

//====================================================================================================
// Filename:	RepositoryTypes.h
// Created by:	Brett Huff
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Repository.h"

#include "GameObject.h"

#include "ModelComponent.h"
#include "TransformComponent.h"

//====================================================================================================
// Typedefs
//====================================================================================================

typedef Repository<GameObject>			GameObjectRepository;

typedef Repository<ModelComponent>		ModelRepository;
typedef Repository<TransformComponent>	TransformRepository;

#endif // #ifndef INCLUDED_ENGINE_REPOSITORYTYPES_H