#ifndef INCLUDED_ENGINE_H
#define INCLUDED_ENGINE_H

//====================================================================================================
// Filename:	Engine.h
// Created by:	Peter Chan
// Description:	3D Engine.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Common.h"

// Core headers
#include "Application.h"
#include "Delegate.h"

// Graphics headers
#include "Camera.h"
#include "Color.h"
#include "ConstantBuffer.h"
#include "Font.h"
#include "GraphicsSystem.h"
#include "Heightmap.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "MeshRenderer.h"
#include "MeshUtil.h"
#include "Model.h"
#include "PixelShader.h"
#include "MeshBuffer.h"
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

#endif // #ifndef INCLUDED_ENGINE_H