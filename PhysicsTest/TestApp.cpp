//====================================================================================================
// Filename:	TestApp.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "TestApp.h"

//====================================================================================================
// Local Definitions
//====================================================================================================
namespace
{

	f32 kTimeStep = 1.0f / 60.0f;
	u32 kNumParticles = 10;
	f32 kParticleSpawnVariance = 2.0f;
	f32 kParticleVelocityRange = 10.0f;
	f32 kRadius = 0.1f;
	f32 kInverseMass = -1.0f;

	f32 lineSize = 10.0f;
	Color GridColor(0.5f, 0.5f, 0.5f, 0.0f);

	void DrawGroundPlane()
	{
		for (f32 i = -1 * lineSize; i <= lineSize; ++i)
		{
			SimpleDraw::AddLine(i*lineSize, 0.0f, -lineSize*lineSize, i*lineSize, 0.0f, lineSize*lineSize, GridColor);
			SimpleDraw::AddLine(-lineSize*lineSize, 0.0f, i*lineSize, lineSize*lineSize, 0.0f, i*lineSize, GridColor);
		}
	}

}
//====================================================================================================
// Class Definitions
//====================================================================================================


TestApp::TestApp()
	: mMouseX(-1)
	, mMouseY(-1)
	, mWorld( Math::Vector3(0.0f, -9.81f, 0.0f), kTimeStep) 
{
	memset(mKeyStates, 0, sizeof(mKeyStates));
}

//----------------------------------------------------------------------------------------------------

TestApp::~TestApp()
{

}

//----------------------------------------------------------------------------------------------------

void TestApp::OnInitialize(u32 width, u32 height)
{
	mWindow.Initialize(GetInstance(), GetAppName(), width, height);
	HookWindow(mWindow.GetWindowHandle());

	mTimer.Initialize();

	mGraphicsSystem.Initialize(mWindow.GetWindowHandle(), false);
	SimpleDraw::Initialize(mGraphicsSystem);
	
	const u32 windowWidth = mGraphicsSystem.GetWidth();
	const u32 windowHeight = mGraphicsSystem.GetHeight();

	mCamera.Setup(Math::kPiByTwo, (f32)windowWidth / (f32)windowHeight, 0.01f, 1000.0f);
}

//----------------------------------------------------------------------------------------------------

void TestApp::OnTerminate()
{
	SimpleDraw::Terminate();
	mGraphicsSystem.Terminate();
	mWindow.Terminate();
	mWorld.Purge();
}

//----------------------------------------------------------------------------------------------------

bool TestApp::OnInput(const InputEvent& evt)
{
	bool handled = false;

	switch (evt.type)
	{
	case InputEvent::KeyDown:
		{
			mKeyStates[evt.value] = true;
			handled = true;
			
			if (mKeyStates[VK_ESCAPE])
			{
				PostQuitMessage(0);
			}
			else if (mKeyStates[VK_F1])
			{
				mGraphicsSystem.ToggleFullscreen();
			}
		}
		break;
	case InputEvent::KeyUp:
		{
			if((mKeyStates[VK_SPACE]))
			{
				mWorld.Purge();
				for(u32 i = 0; i < kNumParticles; ++i)
				{
					//Particle* p = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(0.0f, kParticleSpawnVariance), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), 0.1f, -1.0f);

					//p->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);

					//mWorld.AddParticle(p);

					Particle* p0 = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(0.0f, kParticleSpawnVariance), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);
					Particle* p1 = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(0.0f, kParticleSpawnVariance), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);
					Particle* p2 = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(0.0f, kParticleSpawnVariance), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);
					Particle* p3 = new Particle(Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), Random::GetF(0.0f, kParticleSpawnVariance), Random::GetF(-kParticleSpawnVariance, kParticleSpawnVariance), kRadius, kInverseMass);

					p0->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);
					p1->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);
					p2->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);
					p3->SetVelocity(Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep, Random::GetF(0.0f, kParticleVelocityRange)*kTimeStep, Random::GetF(-kParticleVelocityRange, kParticleVelocityRange)*kTimeStep);

					mWorld.AddParticle(p0);
					mWorld.AddParticle(p1);
					mWorld.AddParticle(p2);
					mWorld.AddParticle(p3);

					Spring* s0 = new Spring(p0, p1, 1.0f);
					Spring* s1 = new Spring(p1, p2, 1.0f);
					Spring* s2 = new Spring(p2, p3, 1.0f);
					Spring* s3 = new Spring(p3, p0, 1.0f);
					Spring* s4 = new Spring(p0, p2, 1.0f);
					Spring* s5 = new Spring(p1, p3, 1.0f);

					mWorld.AddConstraint(s0);
					mWorld.AddConstraint(s1);
					mWorld.AddConstraint(s2);
					mWorld.AddConstraint(s3);
					mWorld.AddConstraint(s4);
					mWorld.AddConstraint(s5);

				}

				bool check = false;
			}
		
			mKeyStates[evt.value] = false;
			handled = true;
		}
		break;
	case InputEvent::MouseMove:
		{
			if (mMouseX != -1 && mMouseY != -1)
			{
				f32 deltaX = (f32)(evt.x - mMouseX);
				f32 deltaY = (f32)(evt.y - mMouseY);
				mCamera.Yaw(deltaX);
				mCamera.Pitch(deltaY);
			}
			mMouseX = evt.x;
			mMouseY = evt.y;
			handled = true;
		}
		break;
	}

	return handled;
}

//----------------------------------------------------------------------------------------------------

void TestApp::OnUpdate()
{
	if (mWindow.HandleMessage())
	{
		mRunning = false;
	}
	else
	{
		// Update our time
		mTimer.Update();

		// Camera movement
		const float kMoveSpeed = 100.0f;
		const float kTurnSpeed = 5.0f;
		if (mKeyStates[VK_UP] || mKeyStates['W'])
		{
			mCamera.Walk(kMoveSpeed * mTimer.GetElapsedTime());
		}
		else if (mKeyStates[VK_DOWN] || mKeyStates['S'])
		{
			mCamera.Walk(-kMoveSpeed * mTimer.GetElapsedTime());
		}
		else if (mKeyStates[VK_RIGHT] || mKeyStates['D'])
		{
			mCamera.Strafe(kMoveSpeed * mTimer.GetElapsedTime());
		}
		else if (mKeyStates[VK_LEFT] || mKeyStates['A'])
		{
			mCamera.Strafe(-kMoveSpeed * mTimer.GetElapsedTime());
		}
		
		
		// Render scene
		mGraphicsSystem.BeginRender(Color::Black());

		DrawGroundPlane();
		mWorld.StepSimulation(mTimer.GetElapsedTime());
		mWorld.Render();
	
		SimpleDraw::Render(mCamera);

		mGraphicsSystem.EndRender();
	}
}