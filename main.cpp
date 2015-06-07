#include "window.h"
#include "Input.h"
#include "timer.h"
#include "Scene.h"
#include "GraphicsContext.h"

#include <sstream>

// Entry point. The program start here
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow)
{
	FWindow Window(hInstance, nCmdShow, 800, 600);
	FInput Input;
	FGraphicsContext GraphicsContext(Window.GetHandle());

	FTimer Timer;
	Timer.Initialize();

	FScene Scene(&Window, &GraphicsContext, &Input);

	Scene.Load();

	while (true)
	{
		if (Window.Update(&Input) == false)
			break;

		if (Input.IsKeyDown(VK_ESCAPE))
			break;

		Timer.Frame();
		Scene.Update(Timer.GetTime());
		Scene.Render();

		float FramesPerSecond = 1.0f / Timer.GetTime();

		std::ostringstream ss;
		ss << "Parallel Application - FPS " << (int)FramesPerSecond;
		Window.SetWindowTitle(ss.str());
	}

	Scene.Unload();

	return 0;
}
