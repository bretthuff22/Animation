#ifndef INCLUDED_ENGINE_Constraints_H
#define INCLUDED_ENGINE_Constraints_H

//====================================================================================================
// Filename:	Constraints.h
// Created by:	Peter Chan
// Description:	Class for a game object.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================
#include "EngineMath.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class Particle;

//====================================================================================================
// Class Declarations
//====================================================================================================

class Constraint
{
public:
	virtual ~Constraint() {}
	virtual void Apply() = 0;
	virtual void Render() {}
};

class Spring : public Constraint
{
public: 
	Spring(Particle* a, Particle* b, f32 restLength = -1.0f);

	// Implements Constraint
	virtual void Apply();
	virtual void Render();

protected:
	Particle* mParticleA;
	Particle* mParticleB;

	f32 mRestLength;

};

#endif