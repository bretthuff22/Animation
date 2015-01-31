//====================================================================================================
// Filename:	BoneAnimation.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "BoneAnimation.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

BoneAnimation::BoneAnimation()
	: mBone(nullptr)
	, mBoneIndex(0)
{
}

//----------------------------------------------------------------------------------------------------

BoneAnimation::~BoneAnimation()
{
	for (u32 i = 0; i < (u32)mKeyframes.size(); ++i)
	{
		SafeDelete(mKeyframes[i]);
	}
	mKeyframes.clear();
}