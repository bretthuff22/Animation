#ifndef INCLUDED_ENGINE_COMMON_H
#define INCLUDED_ENGINE_COMMON_H

//====================================================================================================
// Filename:	Common.h
// Created by:	Peter Chan
// Description:	Header for common includes, typedefs, and helper functions.
//====================================================================================================

//====================================================================================================
// Defines
//====================================================================================================

#define DIRECTINPUT_VERSION 0x0800

//====================================================================================================
// Includes
//====================================================================================================

// Standard headers
#include <windowsx.h>

// STL headers
#include <list>
#include <map>
#include <string>
#include <vector>

// DirectX headers
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <DirectXMath.h>

using namespace DirectX;

//====================================================================================================
// Typedefs
//====================================================================================================

typedef char				s8;
typedef short				s16;
typedef int					s32;
typedef __int64				s64;

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned __int64	u64;

typedef float				f32;
typedef double				f64;

//====================================================================================================
// Macros
//====================================================================================================

#if defined(_DEBUG)
	#define LOG(format, ...)\
	{\
		{\
			va_list va;\
			char buffer[1024];\
			va_start(va, (#format));\
			vsprintf_s(buffer, 1024, (#format), va);\
			va_end(va);\
			std::string message;\
			message += (buffer);\
			message += "\n";\
			OutputDebugStringA(message.c_str());\
		}\
	}
#else
	#define LOG(format, ...)
#endif

//----------------------------------------------------------------------------------------------------

#if defined(_DEBUG)
	#define ASSERT(condition, format, ...)\
	{\
		if (!(condition))\
		{\
			LOG(format, ...)\
			DebugBreak();\
		}\
	}
#else
	#define ASSERT(condition, format, ...)
#endif

//----------------------------------------------------------------------------------------------------

#define NONCOPYABLE(type)\
	type(const type&) = delete;\
	type& operator=(const type&) = delete;

//====================================================================================================
// Helper Functions
//====================================================================================================

template <typename T>
inline void SafeDelete(T*& ptr)
{
	delete ptr;
	ptr = nullptr;
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline void SafeDeleteArray(T*& ptr)
{
	delete[] ptr;
	ptr = nullptr;
}

//----------------------------------------------------------------------------------------------------

template <typename T>
inline void SafeRelease(T*& ptr)
{
	if (ptr != nullptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

#endif // #ifndef INCLUDED_ENGINE_COMMON_H