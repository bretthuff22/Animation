#ifndef INCLUDED_ENGINE_ID_H
#define INCLUDED_ENGINE_ID_H

//====================================================================================================
// Filename:	ID.h
// Created by:	Brett Huff
// Description:	Class for a meta type ID.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "MetaTypes.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class ID
{
public:
	ID();
	ID(Meta::Type type, u8 instance, u16 index);

	Meta::Type GetType() const			{ return (Meta::Type)m.type; }
	u8 GetInstance() const				{ return m.instance; }
	u16 GetIndex() const				{ return m.index; }
	
	bool IsValid() const				{ return blob != U32_MAX; }
	void Invalidate()					{ blob = U32_MAX; }

	bool operator==(ID id) const		{ return (m.type == id.m.type) && (m.instance == id.m.instance) && (m.index == id.m.index); }
	bool operator!=(ID id) const		{ return (m.type != id.m.type) || (m.instance != id.m.instance) || (m.index != id.m.index); }
	bool operator<(ID id) const			{ return (blob < id.blob); }

private:
	union
	{
		struct
		{
			u8 type;
			u8 instance;
			u16 index;
		} m;

		u32 blob;
	};
};

#endif // #ifndef INCLUDED_ENGINE_ID_H