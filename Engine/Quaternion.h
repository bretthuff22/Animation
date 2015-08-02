#ifndef INCLUDED_ENGINE_QUATERNION_H
#define INCLUDED_ENGINE_QUATERNION_H

//====================================================================================================
// Filename:	Quaternion.h
// Created by:	Brett Huff
// Description:	Struct for a quaternion.
//====================================================================================================

//====================================================================================================
// Namespace
//====================================================================================================

namespace Math
{

//====================================================================================================
// Structs
//====================================================================================================

struct Quaternion
{
	f32 x, y, z, w;

	Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	Quaternion(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}

	Quaternion(const Vector3& axis, const f32 angleInRad)
	{
		f32 sinHalfTheta = sinf(angleInRad * 0.5f);
		w = cosf(angleInRad * 0.5f);
		x = axis.x * sinHalfTheta;
		y = axis.y * sinHalfTheta;
		z = axis.z * sinHalfTheta;
	}

	//Quaternion RotationAxis(Math::Vector3(f32 ax, f32 ay, f32 az), f32 angle);
};

} // namespace Math

//====================================================================================================
// Inline Definitions
//====================================================================================================

#include "Quaternion.inl"

#endif // #ifndef INCLUDED_ENGINE_QUATERNION_H