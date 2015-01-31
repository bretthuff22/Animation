#ifndef INCLUDED_ENGINE_ANIMATIONCLIP_H
#define INCLUDED_ENGINE_ANIMATIONCLIP_H

//====================================================================================================
// Filename:	AnimationClip.h
// Created by:	Peter Chan
// Description:	Class for an animation clip.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "EngineMath.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class BoneAnimation;

//====================================================================================================
// Class Declarations
//====================================================================================================

class AnimationClip
{
public:
	AnimationClip();
	~AnimationClip();

//private:
	std::string mName;

	std::vector<BoneAnimation*> mBoneAnimations;

	f32 mDuration;
	f32 mTicksPerSecond;
	u32 mKeyframeCount;
};

#endif // #ifndef INCLUDED_ENGINE_ANIMATIONCLIP_H