//====================================================================================================
// Filename:	Particle.cpp
// Created by:	Brett Huff
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "Particle.h"

#include "Constraints.h"

#include <Engine.h>

//====================================================================================================
// Class Definitions
//====================================================================================================

Particle::Particle()
	: pos(0.0f, 0.0f, 0.0f)
	, posOld(0.0f, 0.0f, 0.0f)
	, accel(0.0f, 0.0f, 0.0f)
	, radius(1.0f)
	, invMass(1.0f)
	, bounce(1.0f)
{

}
	
Particle::Particle(f32 x, f32 y, f32 z, f32 rad, f32 inverseMass)
	: pos(x, y, z)
	, posOld(x, y, z)
	, accel(0.0f, 0.0f, 0.0f)
	, radius(rad)
	, invMass(inverseMass)
	, bounce(1.0f)
{
}
	
void Particle::Render()
{
	SimpleDraw::AddSphere(pos, radius, Color::Blue(), 4, 2);
}

void Particle::SetPosition(f32 x, f32 y, f32 z)
{
	pos.x = posOld.x = x;
	pos.y = posOld.y = y;
	pos.z = posOld.z = z;
}
	
void Particle::SetVelocity(f32 x, f32 y, f32 z)
{
	posOld.x = pos.x - x;
	posOld.y = pos.y - y;
	posOld.z = pos.z - z;
}