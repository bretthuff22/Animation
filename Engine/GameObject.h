#ifndef INCLUDED_ENGINE_GAMEOBJECT_H
#define INCLUDED_ENGINE_GAMEOBJECT_H

//====================================================================================================
// Filename:	GameObject.h
// Created by:	Brett Huff
// Description:	Class for a game object.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "ID.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class GameObject
{
public:
	typedef std::map<Meta::Type, ID> ComponentMap;
	typedef std::vector<Meta::Type> ServiceList;

	GameObject();
	~GameObject();

	void AddComponent(ID id);
	void AddService(Meta::Type type);

	ID GetComponentID(Meta::Type type);
	ID FindComponentID(Meta::Type type);
	
	const char* GetName() const		{ return mName.c_str(); }
	ID GetID() const				{ return mID; }

private:
	std::string mName;
	ID mID;

	ComponentMap mComponentIDs;
	ServiceList mServices;
};

#endif // #ifndef INCLUDED_ENGINE_GAMEOBJECT_H