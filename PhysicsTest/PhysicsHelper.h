#ifndef INCLUDED_ENGINE_PHYSICSHELPER_H
#define INCLUDED_ENGINE_PHYSICSHELPER_H

#include <Engine.h>

namespace PhysicsHelper
{
	const f32 kTimeStep = 1.0f / 60.0f;
	const u32 kNumParticles = 10;
	const f32 kParticleSpawnVariance = 2.0f;
	const f32 kParticleVelocityRange = 10.0f;
	const f32 kParticleHeight = 7.0f;
	const f32 kRadius = 0.1f;
	const f32 kInverseMass = -1.0f;
	const Math::Vector3 kGravity = Math::Vector3(0.0f, -9.81f, 0.0f);
	const f32 kDrag = 0.01f;
	const u32 kClothWidth = 10;
	const u32 kClothHeight = 10;

	const f32 lineWidth = 10.0f;
	const f32 lineHeight = 10.0f;
	const Color GridColor(0.5f, 0.5f, 0.5f, 0.0f);

	void DrawGroundPlane();

	void CreateTetrahedron(PhysicsWorld& world);

	void CreateCube(PhysicsWorld& world);

	void CreateCloth(PhysicsWorld& world);
};

#endif