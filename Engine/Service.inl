//====================================================================================================
// Filename:	Service.inl
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Class Definitions
//====================================================================================================

template <typename T>
Service<T>::Service()
{
}

//----------------------------------------------------------------------------------------------------

template <typename T>
Service<T>::~Service()
{
}

//----------------------------------------------------------------------------------------------------

template <typename T>
void Service<T>::Subscribe(ID objId)
{
	ASSERT(mSubscribers.find(objId) == mSubscribers.end(), "[Service] GameObject %u/%u already registered.", objId.GetIndex(), objId.GetInstance());

	T info;
	OnSubscribe(objId, info);
	mSubscribers.insert(std::make_pair(objId, info));
}

//----------------------------------------------------------------------------------------------------

template <typename T>
void Service<T>::Unsubscribe(ID objId)
{
	auto iter = mSubscribers.find(objId);
	if (iter != mSubscribers.end())
	{
		mSubscribers.erase(iter);
	}
}
