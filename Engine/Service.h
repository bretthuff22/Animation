#ifndef INCLUDED_ENGINE_SERVICE_H
#define INCLUDED_ENGINE_SERVICE_H

//====================================================================================================
// Filename:	Service.h
// Created by:	Peter Chan
// Description:	Templatized base class for a game logic service. The service maintains a collection
//				of subscribers for processing.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "ID.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

template <typename T>
class Service
{
public:
	Service();
	virtual ~Service();

	void Subscribe(ID objId);
	void Unsubscribe(ID objId);

private:
	virtual void OnSubscribe(ID objId, T& info) = 0;

protected:
	NONCOPYABLE(Service);

	typedef std::map<ID, T> Subscribers;

	Subscribers mSubscribers;
};

//====================================================================================================
// Inline Definitions
//====================================================================================================

#include "Service.inl"

#endif // #ifndef INCLUDED_ENGINE_SERVICE_H