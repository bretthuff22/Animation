#ifndef INCLUDED_ENGINE_BONEANIMATION_H
#define INCLUDED_ENGINE_BONEANIMATION_H

//====================================================================================================
// Filename:	BoneAnimation.h
// Created by:	Brett Huff
// Description:	Class for holding animation keyframes for a bone.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Keyframe.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

struct Bone;

//====================================================================================================
// Class Declarations
//====================================================================================================

class BoneAnimation
{
public:
	BoneAnimation();
	~BoneAnimation();

	Math::Matrix GetTransform(f32 time) const;
	u32 FindKeyframeIndex(f32 time) const;

//private:
	std::vector<Keyframe*> mKeyframes;
	u32 mBoneIndex;
};

#endif // #ifndef INCLUDED_ENGINE_BONEANIMATION_H