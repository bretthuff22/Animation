#ifndef INCLUDED_ENGINE_REPOSITORY_H
#define INCLUDED_ENGINE_REPOSITORY_H

//====================================================================================================
// Filename:	Repository.h
// Created by:	Brett Huff
// Description:	Templatized class for a type repository.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "ID.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class BaseRepository
{
public:
	BaseRepository(Meta::Type type) : mType(type) {}

	Meta::Type GetType() const { return mType; }

private:
	Meta::Type mType;
};

//----------------------------------------------------------------------------------------------------

template <typename T>
class Repository : public BaseRepository
{
public:
	Repository(Meta::Type type, u16 capacity);

	ID Allocate();
	void Free(ID& id);

	T& GetItem(ID id);
	const T& GetItem(ID id) const;

	T* FindItem(ID id);
	const T* FindItem(ID id) const;

protected:
	bool IsValid(ID id) const;

	std::vector<T> mData;
	std::vector<u8> mInstanceCount;
	std::vector<u16> mFreeSlots;
};

//====================================================================================================
// Inline Definitions
//====================================================================================================

#include "Repository.inl"

#endif // #include INCLUDED_ENGINE_REPOSITORY_H