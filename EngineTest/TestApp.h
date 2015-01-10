#ifndef INCLUDED_TESTAPP_H
#define INCLUDED_TESTAPP_H

//====================================================================================================
// Filename:	TestApp.h
// Created by:	Peter Chan
// Description:	Class for the test application.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <Engine.h>

//====================================================================================================
// Class Declarations
//====================================================================================================

class TestApp : public Application
{
public:
	TestApp();
	virtual ~TestApp();
	
private:
	virtual void OnInitialize(u32 width, u32 height);
	virtual void OnTerminate();
	virtual bool OnInput(const InputEvent& evt);
	virtual void OnUpdate();
	
	Window mWindow;
	Timer mTimer;

	GraphicsSystem mGraphicsSystem;
	Camera mCamera;

	bool mKeyStates[256];

	s32 mMouseX;
	s32 mMouseY;
};

#endif // #ifndef INCLUDED_TESTAPP_H