#ifndef INCLUDED_ENGINE_RAY_H
#define INCLUDED_ENGINE_RAY_H

//#include "Precompiled.h"
namespace Math
{

struct Ray
{
	Vector3 org;
	Vector3 dir;
	Ray() : org(0.0f, 0.0f, 0.0f), dir(0.0f, 0.0f, 1.0f) {}
	Ray(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz) : org(x, y, z), dir(dx, dy, dz) {}
	Ray(const Vector3& org, const Vector3& dir) : org(org), dir(dir) {}
};

}

#endif