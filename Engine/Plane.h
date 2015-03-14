#ifndef INCLUDED_ENGINE_PLANE_H
#define INCLUDED_ENGINE_PLANE_H

//#include "Precompiled.h"
namespace Math
{

struct Plane
{
	Vector3 n;
	f32 d;

	Plane() : n(0.0f, 1.0f, 0.0f) {}
	Plane(f32 a, f32 b, f32 c, f32 d) : n(a, b, c), d(d){}

};

}
#endif