//====================================================================================================
// Filename:	Repository.inl
// Created by:	Brett Huff
//====================================================================================================

template <typename T>
Repository<T>::Repository(Meta::Type type, u16 capacity)
	: BaseRepository(type)
{
	mData.reserve(capacity);
	mInstanceCount.resize(capacity, 0);
	mFreeSlots.reserve(capacity);
}

//----------------------------------------------------------------------------------------------------

template <typename T>
ID Repository<T>::Allocate()
{
	ASSERT(!mFreeSlots.empty() || mData.size() < mData.capacity(), "Repository (type = %d) is full!", mType);
	if (mFreeSlots.empty() && mData.size() >= mData.capacity())
	{
		return ID();
	}

	u16 index = 0;
	if (!mFreeSlots.empty())
	{
		index = mFreeSlots.back();
		mFreeSlots.pop_back();
	}
	else
	{
		index = (u16)mData.size();
		mData.push_back(T());
	}
	return ID(GetType(), mInstanceCount[index], index);
}

//----------------------------------------------------------------------------------------------------

template <typename T>
void Repository<T>::Free(ID& id)
{
	if (IsValid(id))
	{
		const u16 index = id.GetIndex();

		// Free slot
		mData[index] = T();
		++mInstanceCount[index];
		mFreeSlots.push_back(index);

		// Invalidate ID
		id = ID::INVALID;
	}
}

//----------------------------------------------------------------------------------------------------

template <typename T>
T& Repository<T>::GetItem(ID id)
{
	ASSERT(IsValid(id), "Failed to get item. ID %d/%d/%d is invalid.", id.GetType(), id.GetInstance(), id.GetIndex());
	return mData[id.GetIndex()];
}

//----------------------------------------------------------------------------------------------------

template <typename T>
const T& Repository<T>::GetItem(ID id) const
{
	ASSERT(IsValid(id), "Failed to get item. ID %d/%d/%d is invalid.", id.GetType(), id.GetInstance(), id.GetIndex());
	return mData[id.GetIndex()];
}

//----------------------------------------------------------------------------------------------------

template <typename T>
T* Repository<T>::FindItem(ID id)
{
	const Repository<T>* me = static_cast<const Repository<T>*>(this);
	return const_cast<T*>(me->FindItem(id));
}

//----------------------------------------------------------------------------------------------------

template <typename T>
const T* Repository<T>::FindItem(ID id) const
{
	const T* item = nullptr;
	if (IsValid(id))
	{
		item = &mData[id.GetIndex()];
	}
	return item;
}

//----------------------------------------------------------------------------------------------------

template <typename T>
bool Repository<T>::IsValid(ID id) const
{
	const u8 type = id.GetType();
	const u8 instance = id.GetInstance();
	const u16 index = id.GetIndex();
	return type == GetType() && index < mData.size() && instance == mInstanceCount[index];
}