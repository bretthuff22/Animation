//====================================================================================================
// Filename:	Constraints.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "Constraints.h"

#include "Particle.h"

#include <Engine.h>

//====================================================================================================
// Class Definitions
//====================================================================================================


Spring::Spring(Particle* a, Particle* b, f32 restLength)
	: mParticleA(a)
	, mParticleB(b)
	, mRestLength(restLength)
{
	if(mRestLength <= 0.0f)
	{
		mRestLength = Math::Distance(a->pos, b->pos);
	}
}

	// Implements Constraint
void Spring::Apply()
{
	Math::Vector3 delta = mParticleB->pos - mParticleA->pos;
	f32 dist = Math::Magnitude(delta);
	f32 diff = (dist - mRestLength) / (dist* (mParticleA->invMass + mParticleB->invMass));
	mParticleA->pos += delta * diff * mParticleA->invMass;
	mParticleB->pos += delta * diff * mParticleB->invMass;
}
void Spring::Render()
{
	SimpleDraw::AddLine(mParticleA->pos, mParticleB->pos, Color::Green());
}