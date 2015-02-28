//====================================================================================================
// Filename:	PhysicsWorld.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "PhysicsWorld.h"

#include "Constraints.h"

//====================================================================================================
// Class Definitions
//====================================================================================================

PhysicsWorld::PhysicsWorld(const Math::Vector3& gravity, f32 timeStep)
	: mGravity(gravity)
	, mTimer(0.0f)
	, mTimeStep(timeStep)
{

}
PhysicsWorld::~PhysicsWorld()
{
	Purge();
}

void PhysicsWorld::StepSimulation(f32 deltaTime)
{
	mTimer += deltaTime;
	if(mTimer >= mTimeStep)
	{
		mTimer -= mTimeStep;
		AccumulateForces();
		Integrate(mTimeStep);
		SatisfyConstraints();
	}
}


void PhysicsWorld::AddParticle(Particle* p)
{
	mParticles.push_back(p);

}

void PhysicsWorld::AddConstraint(Constraint* c)
{
	mConstraints.push_back(c);
}

void PhysicsWorld::Purge()
{
	const u32 kNumParticles = mParticles.size();
	for(u32 i = 0; i < kNumParticles; ++i)
	{
		SafeDelete(mParticles[i]);
	}
	mParticles.clear();

	const u32 kNumConstraints = mConstraints.size();
	for(u32 i = 0; i < kNumConstraints; ++i)
	{
		SafeDelete(mConstraints[i]);
	}
	mConstraints.clear();
}

void PhysicsWorld::Render()
{
	const u32 kNumParticles = mParticles.size();
	for(u32 i = 0; i < kNumParticles; ++i)
	{
		mParticles[i]->Render();
	}

	const u32 kNumConstraints = mConstraints.size();
	for(u32 i = 0; i < kNumConstraints; ++i)
	{
		mConstraints[i]->Render();
	}
}

void PhysicsWorld::AccumulateForces()
{
	const u32 kNumParticles = mParticles.size();
	for(u32 i = 0; i < kNumParticles; ++i)
	{
		Particle* p = mParticles[i];
		p->accel = mGravity;
	}
}
	
void PhysicsWorld::Integrate(f32 deltaTime)
{
	const u32 kNumParticles = mParticles.size();
	for(u32 i = 0; i < kNumParticles; ++i)
	{
		Particle* p = mParticles[i];
		Math::Vector3 temp(p->pos);
		p->pos = (p->pos * 2.0f) - p->posOld + (p->accel * deltaTime * deltaTime);
		p->posOld = temp;
	}
}

void PhysicsWorld::SatisfyConstraints()
{
	const u32 kNumConstraints = mConstraints.size();
	for(u32 n = 0; n < 5; ++n)
	{	
		for(u32 i = 0; i < kNumConstraints; ++i)
		{
			mConstraints[i]->Apply();
		}
	}
}