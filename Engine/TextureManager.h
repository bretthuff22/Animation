#ifndef INCLUDED_ENGINE_TEXTUREMANAGER_H
#define INCLUDED_ENGINE_TEXTUREMANAGER_H

//====================================================================================================
// Filename:	TextureManager.h
// Created by:	Brett Huff
// Description:	Class for managing textures.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "ResourceManager.h"
#include "Texture.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class TextureManager : public ResourceManager<Texture>
{
public:
	TextureManager();

	void Initialize(GraphicsSystem& gs);

private:
	virtual void InitResource(Texture* texture, const wchar_t* pFilename);
	virtual void CleanupResource(Texture* texture);

	GraphicsSystem* mpGraphicsSystem;
};

#endif // #ifndef INCLUDED_ENGINE_TEXTUREMANAGER_H