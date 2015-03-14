#ifndef INCLUDED_ENGINE_AABB_H
#define INCLUDED_ENGINE_AABB_H

//#include "Precompiled.h"
namespace Math
{

struct AABB
{
	Vector3 center;
	Vector3 extend;

	AABB() : center(0.0f, 1.0f, 0.0f), extend(1.0f, 1.0f, 1.0f) {}
	AABB(f32 x, f32 y, f32 z, f32 ex, f32 ey, f32 ez) : center(x, y, z), extend(ex, ey, ez) {}
	AABB(const Vector3& center, const Vector3& extend) : center(center), extend(extend) {}

};

}
#endif