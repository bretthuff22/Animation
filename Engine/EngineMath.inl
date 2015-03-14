//====================================================================================================
// Filename:	EngineMath.inl
// Created by:	Peter Chan
//====================================================================================================

namespace Math
{

//====================================================================================================
// Function Definitions
//====================================================================================================

template <typename T>
inline T Min(T a, T b)
{
	return (a > b) ? b : a;
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline T Max(T a, T b)
{
	return (a < b) ? b : a;
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline T Clamp(T value, T min, T max)
{
	return Max(min, Min(max, value));
}

//----------------------------------------------------------------------------------------------------

inline f32 Abs(f32 value)
{
	return (value >= 0.0f) ? value : -value;
}

//----------------------------------------------------------------------------------------------------

inline f32 Sign(f32 value)
{
	return (value >= 0.0f) ? 1.0f : -1.0f;
}

//----------------------------------------------------------------------------------------------------

inline f32 Sqr(f32 value)
{
	return value * value;
}

//----------------------------------------------------------------------------------------------------

inline f32 Sqrt(f32 value)
{
	ASSERT(value >= 0.0f, "[Math] Cannot square root a negative number!");
	return sqrt(value);
}

//----------------------------------------------------------------------------------------------------

inline bool Compare(f32 a, f32 b, f32 epsilon)
{
	return Abs(a - b) < epsilon;
}

//----------------------------------------------------------------------------------------------------

inline bool IsZero(f32 value)
{
	return Compare(value, 0.0f);
}

//----------------------------------------------------------------------------------------------------

inline bool IsZero(const Vector3& v)
{
	return IsZero(v.x) && IsZero(v.y) && IsZero(v.z);
}

//----------------------------------------------------------------------------------------------------

inline f32 MagnitudeSqr(const Vector3& v)
{
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

//----------------------------------------------------------------------------------------------------

inline f32 Magnitude(const Vector3& v)
{
	return Sqrt(MagnitudeSqr(v));
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Normalize(const Vector3& v)
{
	ASSERT(!IsZero(v), "[Math] Cannot normalize zero length vector!");
	const f32 inv = 1.0f / Magnitude(v);
	return v * inv;
}

//----------------------------------------------------------------------------------------------------

inline f32 DistanceSqr(const Vector3& a, const Vector3& b)
{
	return MagnitudeSqr(a - b);
}

//----------------------------------------------------------------------------------------------------

inline f32 Distance(const Vector3& a, const Vector3& b)
{
	return Sqrt(DistanceSqr(a, b));
}

//----------------------------------------------------------------------------------------------------

inline f32 Dot(const Vector3& a, const Vector3& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Cross(const Vector3& a, const Vector3& b)
{
	return Vector3
	(
		(a.y * b.z) - (a.z * b.y),
		(a.z * b.x) - (a.x * b.z),
		(a.x * b.y) - (a.y * b.x)
	);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Project(const Vector3& v, const Vector3& n)
{
	return n * (Dot(v, n) / Dot(n, n));
}

//----------------------------------------------------------------------------------------------------

inline f32 Determinant(const Matrix& m)
{
	f32 det = 0.0f;
	det  = (m._11 * (m._22 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._32 * m._44 - (m._42 * m._34)) + m._24 * (m._32 * m._43 - (m._42 * m._33))));
	det -= (m._12 * (m._21 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._43 - (m._41 * m._33))));
	det += (m._13 * (m._21 * (m._32 * m._44 - (m._42 * m._34)) - m._22 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._42 - (m._41 * m._32))));
	det -= (m._14 * (m._21 * (m._32 * m._43 - (m._42 * m._33)) - m._22 * (m._31 * m._43 - (m._41 * m._33)) + m._23 * (m._31 * m._42 - (m._41 * m._32))));
	return det;
}

//----------------------------------------------------------------------------------------------------

inline Matrix Adjoint(const Matrix& m)
{
	return Matrix
	(
		 (m._22 * ((m._33 * m._44) - (m._43 * m._34)) - m._23 * ((m._32 * m._44) - (m._42 * m._34)) + m._24 * ((m._32 * m._43) - (m._42 * m._33))),
		-(m._12 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._32 * m._44) - (m._42 * m._34)) + m._14 * ((m._32 * m._43) - (m._42 * m._33))),
		 (m._12 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._22 * m._44) - (m._42 * m._24)) + m._14 * ((m._22 * m._43) - (m._42 * m._23))),
		-(m._12 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._22 * m._34) - (m._32 * m._24)) + m._14 * ((m._22 * m._33) - (m._32 * m._23))),

		-(m._21 * ((m._33 * m._44) - (m._43 * m._34)) - m._31 * ((m._23 * m._44) - (m._24 * m._43)) + m._41 * ((m._23 * m._34) - (m._24 * m._33))),
		 (m._11 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._31 * m._44) - (m._41 * m._34)) + m._14 * ((m._31 * m._43) - (m._41 * m._33))),
		-(m._11 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._43) - (m._41 * m._23))),
		 (m._11 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._21 * m._34) - (m._31 * m._24)) + m._14 * ((m._21 * m._33) - (m._31 * m._23))),

		 (m._21 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._22 * m._44) - (m._42 * m._24)) + m._41 * ((m._22 * m._34) - (m._32 * m._24))),
		-(m._11 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._12 * m._44) - (m._42 * m._14)) + m._41 * ((m._12 * m._34) - (m._32 * m._14))),
		 (m._11 * ((m._22 * m._44) - (m._42 * m._24)) - m._12 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._42) - (m._41 * m._22))),
		-(m._11 * ((m._22 * m._34) - (m._32 * m._24)) - m._21 * ((m._12 * m._34) - (m._32 * m._14)) + m._31 * ((m._12 * m._24) - (m._22 * m._14))),

		-(m._21 * ((m._32 * m._43) - (m._42 * m._33)) - m._31 * ((m._22 * m._43) - (m._42 * m._23)) + m._41 * ((m._22 * m._33) - (m._32 * m._23))),
		 (m._11 * ((m._32 * m._43) - (m._42 * m._33)) - m._12 * ((m._31 * m._43) - (m._41 * m._33)) + m._13 * ((m._31 * m._42) - (m._41 * m._32))),
		-(m._11 * ((m._22 * m._43) - (m._42 * m._23)) - m._12 * ((m._21 * m._43) - (m._41 * m._23)) + m._13 * ((m._21 * m._42) - (m._41 * m._22))),
		 (m._11 * ((m._22 * m._33) - (m._32 * m._23)) - m._12 * ((m._21 * m._33) - (m._31 * m._23)) + m._13 * ((m._21 * m._32) - (m._31 * m._22)))
	);
}

//----------------------------------------------------------------------------------------------------

inline Matrix Inverse(const Matrix& m)
{
	const f32 determinant = Determinant(m);
	ASSERT(!IsZero(determinant), "[Math] Cannot find the inverse of matrix. Determinant equals 0.0!");
	const f32 invDet = 1.0f / determinant;
	return Adjoint(m) * invDet;
}

//----------------------------------------------------------------------------------------------------

inline Matrix Transpose(const Matrix& m)
{
	return Matrix
	(
		m._11, m._21, m._31, m._41,
		m._12, m._22, m._32, m._42,
		m._13, m._23, m._33, m._43,
		m._14, m._24, m._34, m._44
	);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 TransformCoord(const Vector3& v, const Matrix& m)
{
	return Vector3
	(
		v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41,
		v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42,
		v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43
	);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 TransformNormal(const Vector3& v, const Matrix& m)
{
	return Vector3
	(
		v.x * m._11 + v.y * m._21 + v.z * m._31,
		v.x * m._12 + v.y * m._22 + v.z * m._32,
		v.x * m._13 + v.y * m._23 + v.z * m._33
	);
}

//----------------------------------------------------------------------------------------------------

inline Vector3 Lerp(const Vector3& v0, const Vector3& v1, f32 t)
{
	return v0 + ((v1 - v0) * t);
}

inline Matrix Lerp(const Matrix& m0, const Matrix& m1, f32 t)
{
	return m0 + ((m1 - m0) * t);
}

inline Quaternion Slerp(Quaternion q0, Quaternion q1, f32 t)
{
	Quaternion slerp;

	return slerp;
}

//inline bool Intersect(const OBB& obb, const Vector3& pos)
//{
//	// translate to obb space
//
//	Math::Matrix matOBBInv = Inverse(Convert(obb.rot)); 
//
//	Math::Vector3 newPos = TransformCoord(pos, matOBBInv);
//
//	Math::Vector3 e(obb.extend);
//
//	if(newPos.x < -e.x || newPos.x > e.x ||
//	   newPos.y < -e.y || newPos.y > e.y ||
//	   newPos.z < -e.z || newPos.z > e.z)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//inline bool Intersect(const OBB& obb, const Ray& ray)
//{
	// translate to obb space

	//Math::Matrix matOBBInv = Inverse(Convert(obb.rot));

	//Math::Vector3 rayPos(ray.position.x, ray.position.y, ray.position.z);
	//Math::Vector3 rayDir(ray.direction.x, ray.direction.y, ray.direction.z);

	//Math::Vector3 o = TransformCoord(rayPos, matOBBInv);
	//Math::Vector3 d = TransformNormal(rayDir, matOBBInv);
	//Math::Vector3 i;
	//if(rayDir.x != 0 && rayDir.y != 0 && rayDir.z != 0)
	//{
	//	i = Vector3(1/rayDir.x, 1/rayDir.y, 1/rayDir.z);
	//}
	//Math::Vector3 e(obb.extend);
	//
	//f32 tmin = -F32_MAX;
	//f32 tmax = F32_MAX;

	//f32 t0x = ( - e.x - o.x ) * i.x;
	//f32 t1x = (   e.x - o.x ) * i.x;
	//tmin = Max(tmin, Min(t0x, t1x));
	//tmax = Min(tmax, Max(t0x, t1x));

	//f32 t0y = ( - e.y - o.y ) * i.y;
	//f32 t1y = (   e.y - o.y ) * i.y;
	//tmin = Max(tmin, Min(t0y, t1y));
	//tmax = Min(tmax, Max(t0y, t1y));

	//f32 t0z = ( - e.z - o.z ) * i.z;
	//f32 t1z = (   e.z - o.z ) * i.z;
	//tmin = Max(tmin, Min(t0z, t1z));
	//tmax = Min(tmax, Max(t0z, t1z));

	//return tmax >= tmin;
//}

inline Matrix Convert(const Quaternion& q)
{
	Matrix ret;

	ret._11 = 1.0f - (2 * q.y*q.y) - (2*q.z*q.z);
	ret._21 = (2 * q.x*q.y) - (2 *q.z*q.w); 
	ret._31 = (2 * q.x*q.z) - (2 *q.y*q.w); 

	ret._12 = (2 * q.x*q.y) + (2 *q.z*q.w);
	ret._22 = 1.0f - (2 * q.x*q.x) - (2*q.z*q.z);
	ret._32 = (2 * q.y*q.z) - (2 *q.x*q.w);

	ret._13 = (2 * q.x*q.z)- (2 *q.y*q.w);
	ret._23 = (2 * q.y*q.z) + (2 *q.x*q.w);
	ret._33 = 1.0f - (2 * q.x*q.x) - (2*q.y*q.y);

	return ret;

}

} // namespace Math