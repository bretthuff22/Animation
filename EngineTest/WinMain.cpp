#include "TestApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	TestApp myApp;
	myApp.Initialize(hInstance, "EngineTest", 1024, 768);

	while(myApp.IsRunning())
	{
		myApp.Update();
	}
	myApp.Terminate();
	return 0;
}
