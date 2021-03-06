//====================================================================================================
// Filename:	Hash.cpp
// Created by:	Brett Huff
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "Hash.h"

//====================================================================================================
// Function Definitions
//====================================================================================================

u64 GetHash(u8* bytes)
{
	// Based on djb2 implementation:
	// http://www.cse.yorku.ca/~oz/hash.html

	u64 hash = 5381;
	s32 c = *bytes++;
	while (c != 0)
	{
		hash = ((hash << 5) + hash) + c;
		c = *bytes++;
	}
	return hash;
}