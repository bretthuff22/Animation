#ifndef INCLUDED_ENGINE_ANIMATIONCLIP_H
#define INCLUDED_ENGINE_ANIMATIONCLIP_H

//====================================================================================================
// Filename:	AnimationClip.h
// Created by:	Brett Huff
// Description:	Class for an animation clip.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "EngineMath.h"
#include "Bone.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

//struct Bone;
class BoneAnimation;

//====================================================================================================
// Class Declarations
//====================================================================================================

class AnimationClip
{
public:
	AnimationClip();
	~AnimationClip();

	Math::Matrix GetTransform(f32 time, Bone* bone) const;

//private:
	std::string mName;

	std::vector<BoneAnimation*> mBoneAnimations;

	f32 mDuration;
	f32 mTicksPerSecond;
	u32 mKeyframeCount;
};

#endif // #ifndef INCLUDED_ENGINE_ANIMATIONCLIP_H