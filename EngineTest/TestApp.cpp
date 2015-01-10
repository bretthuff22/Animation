//====================================================================================================
// Filename:	TestApp.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "TestApp.h"

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
}

//----------------------------------------------------------------------------------------------------

void TestApp::OnTerminate()
{
	SimpleDraw::Terminate();
	mGraphicsSystem.Terminate();
	mWindow.Terminate();
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
	
		//SimpleDraw::AddLine(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, Color::Red());
		//SimpleDraw::AddLine(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, Color::Green());
		//SimpleDraw::AddLine(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, Color::Blue());

		//SimpleDraw::AddAABB(-5.0f, -5.0f, -5.0f, 5.0f, 5.0f, 5.0f, Color::Cyan());
		//for(u32 i = 1; i < 6; ++i)
		//{
		//	SimpleDraw::AddAABB(-5.0f, -5.0f, -5.0f, 5.0f * i, 5.0f * i, 5.0f * i, Color::Cyan());
		//}

		//SimpleDraw::AddAABB(-5.0f, -5.0f, -5.0f, 5.0f, 5.0f, 5.0f, Color::Cyan());
		//SimpleDraw::AddSphere(Math::Vector3(0.0f, 15.0f, 0.0f), 5.0f, Color::Magenta());
		//SimpleDraw::AddSphere(Math::Vector3(-5.0f, 7.5f, 0.0f), 2.5f, Color::Magenta());
		//SimpleDraw::AddSphere(Math::Vector3(5.0f, 7.5f, 0.0f), 2.5f, Color::Magenta());
		//SimpleDraw::AddSphere(Math::Vector3(0.0f, 7.5f, -5.0f), 2.5f, Color::Magenta());
		//SimpleDraw::AddSphere(Math::Vector3(0.0f, 7.5f, 5.0f), 2.5f, Color::Magenta());
		
		// Legs
		SimpleDraw::AddAABB(-5.0f, -5.0f, -5.0f, 0.0f, 15.0f, 0.0f, Color::Cyan());
		SimpleDraw::AddAABB(5.0f, -5.0f, -5.0f, 10.0f, 15.0f, 0.0f, Color::Cyan());
		
	
		SimpleDraw::AddScreenRect(100.0f, 100.0f, 200.0f, 500.0f, Color::Magenta());
		SimpleDraw::AddScreenCircle(150.0f, 300.0f, 50.0f, Color::Cyan());

		SimpleDraw::Render(mCamera);

		mGraphicsSystem.EndRender();
	}
}