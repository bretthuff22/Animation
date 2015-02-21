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
	: mBoneIndex(0)
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

Math::Matrix BoneAnimation::GetTransform(f32 time) const
{
	Math::Matrix transform;
	if(time < mKeyframes[0]->mTime)
	{
		Keyframe* keyframe = mKeyframes[0];
		Math::Matrix rotation = Math::Matrix::RotationQuaternion(keyframe->mRotation);
		Math::Matrix scale = Math::Matrix::Scaling(keyframe->mScale);
		Math::Matrix translation = Math::Matrix::Translation(keyframe->mTranslation);

		transform = rotation * scale * translation;
	}
	else if(time > mKeyframes[mKeyframes.size() - 1]->mTime)
	{
		Keyframe* keyframe = mKeyframes[mKeyframes.size() - 1];
		Math::Matrix rotation = Math::Matrix::RotationQuaternion(keyframe->mRotation);
		Math::Matrix scale = Math::Matrix::Scaling(keyframe->mScale);
		Math::Matrix translation = Math::Matrix::Translation(keyframe->mTranslation);

		transform = rotation * scale * translation;
	}
	else
	{
		u32 index = FindKeyframeIndex(time);

		Keyframe* keyframeFirst = mKeyframes[index];
		Math::Matrix rotation = Math::Matrix::RotationQuaternion(keyframeFirst->mRotation);
		Math::Matrix scale = Math::Matrix::Scaling(keyframeFirst->mScale);
		Math::Matrix translation = Math::Matrix::Translation(keyframeFirst->mTranslation);

		Math::Matrix matrixFirst = rotation * scale * translation;

		Keyframe* keyframeSecond = mKeyframes[index + 1];
		rotation = Math::Matrix::RotationQuaternion(keyframeSecond->mRotation);
		scale = Math::Matrix::Scaling(keyframeSecond->mScale);
		translation = Math::Matrix::Translation(keyframeSecond->mTranslation);

		Math::Matrix matrixSecond = rotation * scale * translation;

		f32 prct = (time - mKeyframes[index]->mTime)/(mKeyframes[index + 1]->mTime - mKeyframes[index]->mTime);

		transform = Math::Lerp(matrixFirst, matrixSecond, prct);
	}

	return transform;
	
}

u32 BoneAnimation::FindKeyframeIndex(f32 time) const
{
	ASSERT(!mKeyframes.empty(), "FindKeyframeIndex: keyframes empty")
	const u32 size = mKeyframes.size();

	ASSERT(time >= mKeyframes[0]->mTime, "FindKeyframeIndex: time out of range");
	ASSERT(time <= mKeyframes[size - 1]->mTime, "FindKeyframeIndex: time out of range");

	u32 keyframeIndex = 1;

	for(; keyframeIndex < size; ++keyframeIndex)
	{
		if(mKeyframes[keyframeIndex]->mTime > time)
		{
			break;
		}
	}

	return keyframeIndex - 1;
}