#ifndef INCLUDED_ENGINE_RESOURCE_H
#define INCLUDED_ENGINE_RESOURCE_H

//====================================================================================================
// Filename:	Resource.h
// Created by:	Peter Chan
// Description:	Base class for a reference counted resource.
//====================================================================================================

//====================================================================================================
// Class Declarations
//====================================================================================================

class Resource
{
public:
	Resource()
		: mRefCount(0)
	{}

	virtual ~Resource()
	{
		// Empty
	}

	void AddRef()
	{
		++mRefCount;
	}

	void Release()
	{
		ASSERT(mRefCount > 0, "[Resource] Cannot release resource with zero references!");
		if (--mRefCount <= 0)
		{
			delete this;
		}
	}

	u32 GetRefCount() const
	{
		return mRefCount;
	}

private:
	u32 mRefCount;
};

#endif // #ifndef INCLUDED_ENGINE_RESOURCE_H