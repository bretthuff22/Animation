#ifndef INCLUDED_ENGINE_TRANSFORMCOMPONENT_H
#define INCLUDED_ENGINE_TRANSFORMCOMPONENT_H

//====================================================================================================
// Filename:	TransformComponent.h
// Created by:	Brett Huff
// Description:	Class for the transform component.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "EngineMath.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class TransformComponent
{
public:
	TransformComponent();
	~TransformComponent();

	Math::Matrix& GetTransform()				{ return mTransform; }
	const Math::Matrix& GetTransform() const	{ return mTransform; }

private:
	Math::Matrix mTransform;
};

#endif // #ifndef INCLUDED_ENGINE_TRANSFORMCOMPONENT_H