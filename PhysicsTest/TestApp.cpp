//====================================================================================================
// Filename:	TestApp.cpp
// Created by:	Brett Huff
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "TestApp.h"
#include "PhysicsHelper.h"

//====================================================================================================
// Class Definitions
//====================================================================================================


TestApp::TestApp()
	: mMouseX(-1)
	, mMouseY(-1)
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
	mCamera.SetPosition(Math::Vector3(0.0f, 5.0f, -5.0f));
	mCamera.SetDirection(Math::Vector3(0.0f, -5.0f, 5.0f));


	PhysicsSettings settings;
	settings.gravity = PhysicsHelper::kGravity;
	settings.drag = PhysicsHelper::kDrag;
	settings.timeStep = PhysicsHelper::kTimeStep;
	mWorld.Setup(settings);

	Math::OBB obb(0,0,0,PhysicsHelper::lineWidth*PhysicsHelper::lineWidth, 1.0f, PhysicsHelper::lineHeight*PhysicsHelper::lineHeight);
	mWorld.AddOBB(obb);
	Math::OBB obb1(-8, 5, 0, 8.0f, 0.1f, 8.0f, 0,0,1, -0.4f);
	mWorld.AddOBB(obb1);
	Math::OBB obb2(8, 8, 0, 8.0f, 0.1f, 8.0f, 0,0,1, 0.4f);
	mWorld.AddOBB(obb2);

}

//----------------------------------------------------------------------------------------------------

void TestApp::OnTerminate()
{
	SimpleDraw::Terminate();
	mGraphicsSystem.Terminate();
	mWindow.Terminate();
	mWorld.ClearAll();
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
				mWorld.ClearDynamic();
				for(u32 i = 0; i < PhysicsHelper::kNumParticles; ++i)
				{
					PhysicsHelper::CreateTetrahedron(mWorld);
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
		
		mWorld.StepSimulation(mTimer.GetElapsedTime());
		// Render scene
		mGraphicsSystem.BeginRender(Color::Black());

		PhysicsHelper::DrawGroundPlane();
		
		mWorld.Render();
	
		SimpleDraw::Render(mCamera);

		mGraphicsSystem.EndRender();
	}
}