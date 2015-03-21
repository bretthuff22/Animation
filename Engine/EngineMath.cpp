//====================================================================================================
// Filename:	EngineMath.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "EngineMath.h"

namespace Math
{

//====================================================================================================
// Constants
//====================================================================================================

const f32 kPi			= 3.14159265358979f;
const f32 kTwoPi		= 6.28318530717958f;
const f32 kPiByTwo		= 1.57079632679489f;
const f32 kDegToRad		= kPi / 180.0f;
const f32 kRadToDeg		= 180.0f / kPi;

//====================================================================================================
// Function Definitions
//====================================================================================================

Matrix Matrix::RotationAxis(const Vector3& axis, f32 rad)
{
	const Vector3 u = Normalize(axis);
	const f32 x = u.x;
	const f32 y = u.y;
	const f32 z = u.z;
	const f32 s = sin(rad);
	const f32 c = cos(rad);

	return Matrix
	(
		c + (x * x * (1.0f - c)),
		x * y * (1.0f - c) + z * s,
		x * z * (1.0f - c) - y * s,
		0.0f,

		x * y * (1.0f - c) - z * s,
		c + (y * y * (1.0f - c)),
		y * z * (1.0f - c) + x * s,
		0.0f,

		x * z * (1.0f - c) + y * s,
		y * z * (1.0f - c) - x * s,
		c + (z * z * (1.0f - c)),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	);
}

//----------------------------------------------------------------------------------------------------

Matrix Matrix::RotationQuaternion(const Quaternion& q)
{
	return Matrix
	(
		1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
		(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
		(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
		0.0f,
		
		(2.0f * q.x * q.y) - (2.0f * q.z * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
		(2.0f * q.y * q.z) + (2.0f * q.x * q.w),
		0.0f,
		
		(2.0f * q.x * q.z) + (2.0f * q.y * q.w),
		(2.0f * q.y * q.z) - (2.0f * q.x * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	);
}


bool Intersect(const Ray& ray, const Plane& plane, f32& distance)
{
	const Vector3 n = Normalize(plane.n);
	f32 DirDotPlane = Dot(ray.dir, n);
	f32 OrgDotPlane = Dot(ray.org, n);
	if(DirDotPlane == 0.0f)
	{
		// ray and plane parallel
		if(OrgDotPlane == plane.d)
		{
			distance = 0.0f;
			return true;
		}
		else
		{
			return false;
		}
	}
	
	distance = (plane.d - OrgDotPlane)/DirDotPlane;

	return true;
}

bool Intersect(const Ray& ray, const AABB& aabb, f32& distEntry, f32& distExit)
{
	Vector3 boxMin = aabb.center - aabb.extend;
	Vector3 boxMax = aabb.center + aabb.extend;
	f32 tmin, tmax, tymin, tymax, tzmin, tzmax;

	f32 divx = 1.0f / ray.dir.x;
	f32 divy = 1.0f / ray.dir.y;
	f32 divz = 1.0f / ray.dir.z;

	if(divx >= 0.0f)
	{
		tmin = (boxMin.x - ray.org.x) * divx;
		tmax = (boxMax.x - ray.org.x) * divx;
	}
	else
	{
		tmin = (boxMax.x - ray.org.x) * divx;
		tmax = (boxMin.x - ray.org.x) * divx;
	}
	if(divy >= 0.0f)
	{
		tymin = (boxMin.y - ray.org.y) * divy;
		tymax = (boxMax.y - ray.org.y) * divy;
	}
	else
	{
		tymin = (boxMax.y - ray.org.y) * divy;
		tymax = (boxMin.y - ray.org.y) * divy;
	}

	if((tmin > tymax) || (tymin > tmax))
	{
		return false;
	}

	if(tymin > tmin)
	{
		tmin = tymin;
	}

	if(tymax < tmax)
	{
		tmax = tymax;
	}

	if(divz >= 0.0f)
	{
		tzmin = (boxMin.z - ray.org.z) * divz;
		tzmax = (boxMax.z - ray.org.z) * divz;
	}
	else
	{
		tzmin = (boxMax.z - ray.org.z) * divz;
		tzmax = (boxMin.z - ray.org.z) * divz;
	}

	if((tmin > tzmax) || (tzmin > tmax))
	{
		return false;
	}

	if(tzmin > tmin)
	{
		tmin = tzmin;
	}

	if(tzmax < tmax)
	{
		tmax = tzmax;
	}

	distEntry = tmin;
	distExit = tmax;
	return true;
}

bool Intersect(const Ray& ray, const OBB& obb, f32& distEntry, f32& distExit)
{
	Matrix matTrans = Math::Matrix::Translation(obb.center);
	Matrix matRot = Math::Matrix::RotationQuaternion(obb.rot);
	Matrix matWorld = matRot * matTrans;
	Matrix matWorldInv = Inverse(matWorld);
	
	Vector3 localOrg = TransformCoord(ray.org, matWorldInv);
	Vector3 localDir = TransformNormal(ray.dir, matWorldInv);
	
	AABB localAABB(Vector3::Zero(), obb.extend);

	return Intersect(Ray(localOrg, localDir), localAABB, distEntry, distExit);
}

bool Intersect(const Vector3& point, const AABB& aabb)
{
	Vector3 boxMin = aabb.center - aabb.extend;
	Vector3 boxMax = aabb.center + aabb.extend;

//	Vector3 p(point - aabb.center);
//	Vector3 e(aabb.extend);

	return (point.x >= boxMin.x && point.x <= boxMax.x &&
			point.y >= boxMin.y && point.y <= boxMax.y &&
			point.z >= boxMin.z && point.z <= boxMax.z );
}

bool Intersect(const Vector3& point, const OBB& obb)
{
	// translate to obb space
	Matrix matTrans = Math::Matrix::Translation(obb.center);
	Matrix matRot = Math::Matrix::RotationQuaternion(obb.rot);
	Matrix matWorld = matRot * matTrans;
	Matrix matWorldInv = Inverse(matWorld);

	Vector3 localPoint = TransformCoord(point, matWorldInv);
	AABB localAABB(Vector3::Zero(), obb.extend);

	return Intersect(localPoint, localAABB);

	//Matrix matOBBInv = Inverse(Convert(obb.rot)); 
	//Vector3 p = point - obb.center;
	//Vector3 newPos = TransformCoord(p, matOBBInv);

	//Vector3 e(obb.extend);

	//return !(newPos.x < -e.x || newPos.x > e.x ||
	//   newPos.y < -e.y || newPos.y > e.y ||
	//   newPos.z < -e.z || newPos.z > e.z);
}

bool GetContactPoint(const Ray& ray, const OBB& obb, Vector3& point, Vector3& normal)
{
	Matrix matTrans = Math::Matrix::Translation(obb.center);
	Matrix matRot = Math::Matrix::RotationQuaternion(obb.rot);
	Matrix matWorld = matRot * matTrans;
	Matrix matWorldInv = Inverse(matWorld);

	Vector3 org = TransformCoord(ray.org, matWorldInv);
	Vector3 dir = TransformNormal(ray.dir, matWorldInv);
	Ray localRay(org, dir);

	Plane planes[] =
	{
		Plane( 0.0f,  0.0f, -1.0f, obb.extend.z),
		Plane( 0.0f,  0.0f,  1.0f, obb.extend.z),
		Plane( 0.0f, -1.0f,  0.0f, obb.extend.y),
		Plane( 0.0f,  1.0f,  0.0f, obb.extend.y),
		Plane(-1.0f,  0.0f,  0.0f, obb.extend.x),
		Plane( 1.0f,  0.0f,  0.0f, obb.extend.x)
	};

	u32 numIntersections = 0;
	for(u32 i = 0; i < 6; ++i)
	{
		const f32 d = Dot(org, planes[i].n);
		if(d > planes[i].d)
		{
			f32 distance = 0.0f;
			if(Intersect(localRay, planes[i], distance) && distance > 0.0f)
			{
				Vector3 pt = org + (dir * distance);
				if(abs(pt.x) < obb.extend.x + 0.01f &&
				   abs(pt.y) < obb.extend.y + 0.01f &&
				   abs(pt.z) < obb.extend.z + 0.01f)
				{
					point = pt;
					normal = planes[i].n;
					++numIntersections;
				}
			}
		}
	}

	if(numIntersections == 0)
	{
		return false;
	}

	point = TransformCoord(point, matWorld);
	normal = TransformNormal(normal, matWorld);

	return true;
}

} // namespace Math