//====================================================================================================
// Filename:	Keyframe.cpp
// Created by:	Brett Huff
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "Keyframe.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

Keyframe::Keyframe()
	: mTranslation(0.0f, 0.0f, 0.0f)
	, mRotation(0.0f, 0.0f, 0.0f, 1.0f)
	, mScale(1.0f, 1.0f, 1.0f)
	, mTime(0.0f)
{}

//----------------------------------------------------------------------------------------------------

Math::Matrix Keyframe::GetTransform() const
{
	Math::Matrix matTrans = Math::Matrix::Translation(mTranslation);
	Math::Matrix matRot = Math::Matrix::RotationQuaternion(mRotation);
	Math::Matrix matScale = Math::Matrix::Scaling(mScale);
	return matScale * matRot * matTrans;
}