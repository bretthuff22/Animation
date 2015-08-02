
#ifndef INCLUDED_ENGINE_H
#define INCLUDED_ENGINE_H

//====================================================================================================
// Filename:	Engine.h
// Created by:	Brett Huff
// Description:	3D Engine.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Common.h"

// Core headers
#include "Application.h"
#include "Delegate.h"
#include "Random.h"

// Component headers
#include "ModelComponent.h"
#include "TransformComponent.h"


// GameObject headers
#include "GameObject.h"
#include "ID.h"
#include "MetaTypes.h"
#include "RepositoryTypes.h"

// Graphics headers
#include "AnimationClip.h"
#include "AnimationController.h"
#include "Bone.h"
#include "BoneAnimation.h"
#include "Camera.h"
#include "Color.h"
#include "ConstantBuffer.h"
#include "Font.h"
#include "GraphicsSystem.h"
#include "Heightmap.h"
#include "Mesh.h"
#include "MeshBuffer.h"
#include "MeshBuilder.h"
#include "MeshRenderer.h"
#include "MeshUtil.h"
#include "Model.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "SimpleDraw.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "VertexShader.h"

// Input heades
#include "InputEvent.h"
#include "InputSystem.h"
#include "InputTypes.h"

// Math headers
#include "EngineMath.h"


// Platform headers
#include "Timer.h"
#include "Window.h"

// Physics headers
#include "Constraints.h"
#include "Particle.h"
#include "PhysicsWorld.h"

// Resource headers
#include "Resource.h"
#include "ResourceManager.h"
#include "TextureManager.h"

// Service headers
#include "RenderService.h"

#endif // #ifndef INCLUDED_ENGINE_H