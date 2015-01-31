#ifndef INCLUDED_ENGINE_METATYPES_H
#define INCLUDED_ENGINE_METATYPES_H

//====================================================================================================
// Filename:	MetaTypes.h
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Hash.h"

//====================================================================================================
// Namespace
//====================================================================================================

namespace Meta
{

enum Type
{
#define DEFTYPE(x) x##Type,
#include "MetaTypeDefines.tup"
#undef DEFTYPE
};

//----------------------------------------------------------------------------------------------------

static const char* Name[] =
{
#define DEFTYPE(x) #x,
#include "MetaTypeDefines.tup"
#undef DEFTYPE
};

//----------------------------------------------------------------------------------------------------

static const u64 Hash[] =
{
#define DEFTYPE(x) GetHash((u8*)#x),
#include "MetaTypeDefines.tup"
#undef DEFTYPE
};

} // namespace Meta

#endif // #ifndef INCLUDED_ENGINE_METATYPES_H