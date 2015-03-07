#include "Precompiled.h"
#include "PhysicsHelper.h"


void PhysicsHelper::DrawGroundPlane()
{
	for (f32 i = -1 * lineWidth; i <= lineWidth; ++i)
	{
		SimpleDraw::AddLine(i*lineWidth, 0.0f, -lineHeight*lineHeight, i*lineWidth, 0.0f, lineHeight*lineHeight, GridColor);
		SimpleDraw::AddLine(-lineWidth*lineWidth, 0.0f, i*lineHeight, lineWidth*lineWidth, 0.0f, i*lineHeight, GridColor);
	}
}

void PhysicsHelper::CreateTetrahedron(PhysicsWorld& world)
{
	Particle* p0 = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(1.0f, kParticleSpawnVariance + 1.0f), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);
	Particle* p1 = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(1.0f, kParticleSpawnVariance + 1.0f), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);
	Particle* p2 = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(1.0f, kParticleSpawnVariance + 1.0f), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);
	Particle* p3 = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(1.0f, kParticleSpawnVariance + 1.0f), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);

	p0->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);
	p1->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);
	p2->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);
	p3->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);

	world.AddParticle(p0);
	world.AddParticle(p1);
	world.AddParticle(p2);
	world.AddParticle(p3);

	Spring* s0 = new Spring(p0, p1, 1.0f);
	Spring* s1 = new Spring(p1, p2, 1.0f);
	Spring* s2 = new Spring(p2, p3, 1.0f);
	Spring* s3 = new Spring(p3, p0, 1.0f);
	Spring* s4 = new Spring(p0, p2, 1.0f);
	Spring* s5 = new Spring(p1, p3, 1.0f);

	world.AddConstraint(s0);
	world.AddConstraint(s1);
	world.AddConstraint(s2);
	world.AddConstraint(s3);
	world.AddConstraint(s4);
	world.AddConstraint(s5);
}

void PhysicsHelper::CreateCube(PhysicsWorld& world)
{
	Particle* p0 = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(1.0f, kParticleSpawnVariance + 1.0f), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);
	Particle* p1 = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(1.0f, kParticleSpawnVariance + 1.0f), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);
	Particle* p2 = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(1.0f, kParticleSpawnVariance + 1.0f), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);
	Particle* p3 = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(1.0f, kParticleSpawnVariance + 1.0f), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);
	Particle* p4 = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(1.0f, kParticleSpawnVariance + 1.0f), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);
	Particle* p5 = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(1.0f, kParticleSpawnVariance + 1.0f), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);
	Particle* p6 = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(1.0f, kParticleSpawnVariance + 1.0f), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);
	Particle* p7 = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(1.0f, kParticleSpawnVariance + 1.0f), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);

	p0->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);
	p1->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);
	p2->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);
	p3->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);
	p4->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);
	p5->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);
	p6->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);
	p7->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);

	world.AddParticle(p0);
	world.AddParticle(p1);
	world.AddParticle(p2);
	world.AddParticle(p3);
	world.AddParticle(p4);
	world.AddParticle(p5);
	world.AddParticle(p6);
	world.AddParticle(p7);


	Spring* s0 = new Spring(p0, p1, 1.0f);
	Spring* s1 = new Spring(p1, p2, 1.0f);
	Spring* s2 = new Spring(p2, p3, 1.0f);
	Spring* s3 = new Spring(p3, p0, 1.0f);

	Spring* s4 = new Spring(p0, p4, 1.0f);
	Spring* s5 = new Spring(p1, p5, 1.0f);
	Spring* s6 = new Spring(p2, p6, 1.0f);
	Spring* s7 = new Spring(p3, p7, 1.0f);

	Spring* s8 = new Spring(p4, p5, 1.0f);
	Spring* s9 = new Spring(p5, p6, 1.0f);
	Spring* s10 = new Spring(p6, p7, 1.0f);
	Spring* s11 = new Spring(p7, p4, 1.0f);

	f32 const kRootTwo = Math::Sqrt(2.0f);

	Spring* s12 = new Spring(p0, p2, kRootTwo);
	Spring* s13 = new Spring(p1, p3, kRootTwo);
	Spring* s14 = new Spring(p1, p6, kRootTwo);
	Spring* s15 = new Spring(p5, p2, kRootTwo);
	Spring* s16 = new Spring(p4, p6, kRootTwo);
	Spring* s17 = new Spring(p5, p7, kRootTwo);
	Spring* s18 = new Spring(p4, p3, kRootTwo);
	Spring* s19 = new Spring(p0, p7, kRootTwo);
	Spring* s20 = new Spring(p0, p5, kRootTwo);
	Spring* s21 = new Spring(p1, p4, kRootTwo);
	Spring* s22 = new Spring(p3, p6, kRootTwo);
	Spring* s23 = new Spring(p2, p7, kRootTwo);

	world.AddConstraint(s0);
	world.AddConstraint(s1);
	world.AddConstraint(s2);
	world.AddConstraint(s3);
	world.AddConstraint(s4);
	world.AddConstraint(s5);
	world.AddConstraint(s6);
	world.AddConstraint(s7);
	world.AddConstraint(s8);
	world.AddConstraint(s9);
	world.AddConstraint(s10);
	world.AddConstraint(s11);
	world.AddConstraint(s12);
	world.AddConstraint(s13);
	world.AddConstraint(s14);
	world.AddConstraint(s15);
	world.AddConstraint(s16);
	world.AddConstraint(s17);
	world.AddConstraint(s18);
	world.AddConstraint(s19);
	world.AddConstraint(s20);
	world.AddConstraint(s21);
	world.AddConstraint(s22);
	world.AddConstraint(s23);
}

void PhysicsHelper::CreateCloth(PhysicsWorld& world)
{
	Particle* p = new Particle[kClothWidth*kClothHeight];

	for(u32 i = 0; i < kClothWidth*kClothHeight; ++i)
	{
		p[i] = Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(1.0f, kParticleSpawnVariance + 1.0f), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);
		p[i].SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);
		world.AddParticle(&p[i]);
	}

	f32 const kRootTwo = Math::Sqrt(2.0f);

	//Spring* s0 = new Spring(p0, p1, 1.0f);
	//Spring* s1 = new Spring(p1, p2, 1.0f);
	//Spring* s2 = new Spring(p3, p4, 1.0f);
	//Spring* s3 = new Spring(p4, p5, 1.0f);
	//Spring* s4 = new Spring(p6, p7, 1.0f);
	//Spring* s5 = new Spring(p7, p8, 1.0f);
	//Spring* s6 = new Spring(p0, p3, 1.0f);
	//Spring* s7 = new Spring(p3, p6, 1.0f);
	//Spring* s8 = new Spring(p1, p4, 1.0f);
	//Spring* s9 = new Spring(p4, p7, 1.0f);
	//Spring* s10 = new Spring(p2, p5, 1.0f);
	//Spring* s11 = new Spring(p5, p8, 1.0f);

	//Spring* s12 = new Spring(p0, p4, kRootTwo);
	//Spring* s13 = new Spring(p4, p8, kRootTwo);
	//Spring* s14 = new Spring(p3, p1, kRootTwo);
	//Spring* s15 = new Spring(p1, p5, kRootTwo);
	//Spring* s16 = new Spring(p6, p4, kRootTwo);
	//Spring* s17 = new Spring(p4, p2, kRootTwo);
	//Spring* s18 = new Spring(p3, p7, kRootTwo);
	//Spring* s19 = new Spring(p7, p5, kRootTwo);

	//world.AddConstraint(s0);
	//world.AddConstraint(s1);
	//world.AddConstraint(s2);
	//world.AddConstraint(s3);
	//world.AddConstraint(s4);
	//world.AddConstraint(s5);
	//world.AddConstraint(s6);
	//world.AddConstraint(s7);
	//world.AddConstraint(s8);
	//world.AddConstraint(s9);
	//world.AddConstraint(s10);
	//world.AddConstraint(s11);
	//world.AddConstraint(s12);
	//world.AddConstraint(s13);
	//world.AddConstraint(s14);
	//world.AddConstraint(s15);
	//world.AddConstraint(s16);
	//world.AddConstraint(s17);
	//world.AddConstraint(s18);
	//world.AddConstraint(s19);
}