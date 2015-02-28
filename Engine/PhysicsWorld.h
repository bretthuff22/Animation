#ifndef INCLUDED_ENGINE_PHYSICSWORLD_H
#define INCLUDED_ENGINE_PHYSICSWORLD_H

//====================================================================================================
// Filename:	PhysicsWorld.h
// Created by:	Peter Chan
// Description:	Class for a game object.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Engine.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

class Particle;
class Constraint;

//====================================================================================================
// Class Declarations
//====================================================================================================

class PhysicsWorld
{
public:
	PhysicsWorld(const Math::Vector3& gravity, f32 timeStep);
	~PhysicsWorld();

	void StepSimulation(f32 deltaTime);

	void AddParticle(Particle* p);
	void AddConstraint(Constraint* c);
	void Purge();

	void Render();
private:
	void AccumulateForces();
	void Integrate(f32 deltaTime);
	void SatisfyConstraints();
	std::vector<Particle*> mParticles;
	std::vector<Constraint*> mConstraints;
	Math::Vector3 mGravity;

	f32 mTimer;
	f32 mTimeStep;
};

#endif