//====================================================================================================
// Filename:	AnimationClip.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "AnimationClip.h"

#include "BoneAnimation.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

AnimationClip::AnimationClip()
	: mDuration(0.0f)
	, mTicksPerSecond(0.0f)
	, mKeyframeCount(0)
{
}

//----------------------------------------------------------------------------------------------------

AnimationClip::~AnimationClip()
{
	for (u32 i = 0; i < (u32)mBoneAnimations.size(); ++i)
	{
		SafeDelete(mBoneAnimations[i]);
	}
	mBoneAnimations.clear();
}

Math::Matrix AnimationClip::GetTransform(f32 time, Bone* bone) const
{
	Math::Matrix transform;
	BoneAnimation* boneAnim = mBoneAnimations[bone->index];

	if(boneAnim != nullptr)
	{
		transform = boneAnim->GetTransform(time);
	}

	return transform;
}