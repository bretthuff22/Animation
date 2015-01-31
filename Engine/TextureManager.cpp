//====================================================================================================
// Filename:	TextureManager.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "TextureManager.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

TextureManager::TextureManager()
	: mpGraphicsSystem(nullptr)
{}


//----------------------------------------------------------------------------------------------------

void TextureManager::Initialize(GraphicsSystem& gs)
{
	mpGraphicsSystem = &gs;
}

//----------------------------------------------------------------------------------------------------

void TextureManager::InitResource(Texture* texture, const wchar_t* pFilename)
{
	ASSERT(mpGraphicsSystem != nullptr, "[TextureManager] Not initialized.");
	texture->Initialize(*mpGraphicsSystem, pFilename);
}

//----------------------------------------------------------------------------------------------------

void TextureManager::CleanupResource(Texture* texture)
{
	texture->Terminate();
}