//====================================================================================================
// Filename:	GameObject.cpp
// Created by:	Brett Huff
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "GameObject.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

GameObject::GameObject()
	: mName("Unknown")
{
}

//----------------------------------------------------------------------------------------------------

GameObject::~GameObject()
{
}

//----------------------------------------------------------------------------------------------------

void GameObject::AddComponent(ID id)
{
	ASSERT(mComponentIDs.find(id.GetType()) == mComponentIDs.end(), "[GameObject] Duplicate component of type %u not allowed.", id.GetType());
	mComponentIDs.insert(std::make_pair(id.GetType(), id));
}

//----------------------------------------------------------------------------------------------------

void GameObject::AddService(Meta::Type type)
{
	mServices.push_back(type);
}

//----------------------------------------------------------------------------------------------------

ID GameObject::GetComponentID(Meta::Type type)
{
	auto iter = mComponentIDs.find(type);
	ASSERT(iter != mComponentIDs.end(), "[GameObject] Component type %u not found!", type);
	return iter->second;
}

//----------------------------------------------------------------------------------------------------

ID GameObject::FindComponentID(Meta::Type type)
{
	ID id;
	auto iter = mComponentIDs.find(type);
	if (iter != mComponentIDs.end())
	{
		id = iter->second;
	}
	return id;
}