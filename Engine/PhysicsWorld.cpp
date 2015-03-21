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

namespace
{
	void DrawOBB(Math::OBB& obb)
	{
		Math::Matrix matTrans = Math::Matrix::Translation(obb.center);
		Math::Matrix matRot = Math::Matrix::RotationQuaternion(obb.rot);
		Math::Matrix toWorld = matRot * matTrans;

		Math::Vector3 p0(- obb.extend.x, - obb.extend.y, - obb.extend.z);
		Math::Vector3 p1(- obb.extend.x, - obb.extend.y, + obb.extend.z);
		Math::Vector3 p2(- obb.extend.x, + obb.extend.y, + obb.extend.z);
		Math::Vector3 p3(- obb.extend.x, + obb.extend.y, - obb.extend.z);
		Math::Vector3 p4(+ obb.extend.x, - obb.extend.y, - obb.extend.z);
		Math::Vector3 p5(+ obb.extend.x, - obb.extend.y, + obb.extend.z);
		Math::Vector3 p6(+ obb.extend.x, + obb.extend.y, + obb.extend.z);
		Math::Vector3 p7(+ obb.extend.x, + obb.extend.y, - obb.extend.z);

		p0 = Math::TransformCoord(p0, toWorld);
		p1 = Math::TransformCoord(p1, toWorld);
		p2 = Math::TransformCoord(p2, toWorld);
		p3 = Math::TransformCoord(p3, toWorld);
		p4 = Math::TransformCoord(p4, toWorld);
		p5 = Math::TransformCoord(p5, toWorld);
		p6 = Math::TransformCoord(p6, toWorld);
		p7 = Math::TransformCoord(p7, toWorld);

		SimpleDraw::AddLine(p0, p1, Color::Magenta());
		SimpleDraw::AddLine(p1, p2, Color::Magenta());
		SimpleDraw::AddLine(p2, p3, Color::Magenta());
		SimpleDraw::AddLine(p3, p0, Color::Magenta());
		SimpleDraw::AddLine(p4, p5, Color::Magenta());
		SimpleDraw::AddLine(p5, p6, Color::Magenta());
		SimpleDraw::AddLine(p6, p7, Color::Magenta());
		SimpleDraw::AddLine(p7, p4, Color::Magenta());
		SimpleDraw::AddLine(p0, p4, Color::Magenta());
		SimpleDraw::AddLine(p1, p5, Color::Magenta());
		SimpleDraw::AddLine(p2, p6, Color::Magenta());
		SimpleDraw::AddLine(p3, p7, Color::Magenta());

	}
}

//====================================================================================================
// Class Definitions
//====================================================================================================

PhysicsWorld::PhysicsWorld()
	: mTimer(0.0f)
{

}
PhysicsWorld::~PhysicsWorld()
{
	ClearAll();
}

void PhysicsWorld::Setup(const PhysicsSettings& settings)
{
	mSettings = settings;
}

void PhysicsWorld::StepSimulation(f32 deltaTime)
{
	mTimer += deltaTime;
	if(mTimer >= mSettings.timeStep)
	{
		mTimer -= mSettings.timeStep;
		AccumulateForces();
		Integrate();
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

void PhysicsWorld::AddOBB(Math::OBB& obb)
{
	mOBBs.push_back(obb);
}

void PhysicsWorld::ClearDynamic()
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

void PhysicsWorld::ClearAll()
{
	ClearDynamic();

	mOBBs.clear();

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

	const u32 kNumOBBs = mOBBs.size();
	for(u32 i = 0; i < kNumOBBs; ++i)
	{
		DrawOBB(mOBBs[i]);
	}
}

void PhysicsWorld::AccumulateForces()
{
	const u32 kNumParticles = mParticles.size();
	for(u32 i = 0; i < kNumParticles; ++i)
	{
		Particle* p = mParticles[i];
		p->accel = mSettings.gravity;
	}
}
	
void PhysicsWorld::Integrate()
{
	const f32 kGamma = 1.0f - mSettings.drag;
	const f32 kTimeStepSqr = Math::Sqr(mSettings.timeStep);
	const u32 kNumParticles = mParticles.size();
	for(u32 i = 0; i < kNumParticles; ++i)
	{
		Particle* p = mParticles[i];
		Math::Vector3 temp(p->pos);
		p->pos = p->pos + ((p->pos - p->posOld) * kGamma) + (p->accel * kTimeStepSqr);
		p->posOld = temp;
	}
}

void PhysicsWorld::SatisfyConstraints()
{
	const u32 kNumParticles = mParticles.size();
	const u32 kNumConstraints = mConstraints.size();
	const u32 kNumOBBs = mOBBs.size();
	for(u32 n = 0; n < 1; ++n)
	{	
		for(u32 i = 0; i < kNumConstraints; ++i)
		{
			mConstraints[i]->Apply();
		}

		for(u32 i = 0; i < kNumParticles; ++i)
		{
			Particle* p = mParticles[i];

			// intersection 
				// transfer particle to local obb space
				// if particle in obb, (point box intersection test)
					// do (ray intersection test) to update particle by reflecting the normal of the side using the exact contact point
			for(u32 j = 0; j < kNumOBBs; ++j)
			{
				if (Math::Intersect(p->pos, mOBBs[j]))
				{
					Math::Vector3 vel(p->pos - p->posOld);
					Math::Vector3 dir = Math::Normalize(vel);

					Math::Ray ray(p->posOld, dir);
					Math::Vector3 point, normal;
					Math::GetContactPoint(ray, mOBBs[j], point, normal);

					Math::Vector3 velN = normal * Math::Dot(normal, vel);
					Math::Vector3 velT = vel - velN;

					p->pos = point + (normal*0.005f);
					p->posOld = p->pos - (velT - velN*p->bounce);

				}
			}
			
		}
	}
}