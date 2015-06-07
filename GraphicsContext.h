#ifndef GraphicsContext_h__
#define GraphicsContext_h__

#include <windows.h>
#include <gl/GL.h>

class FGraphicsContext
{
public:
	FGraphicsContext(HWND Hwnd);

	void SetWireframeMode(bool bWireframe);
	bool GetWireframeMode() { return bIsRenderingWireframe; }

	void ResetLighting();
	void EnableDepthTesting(bool bEnable);
	void EnableLighting(bool bEnable);
	void EnableTexturing(bool bEnable);

	void SwapBuffers();


private:
	bool CreatePixelFormat(HDC);
	void ResizeGLWindow(int Width, int Height);
	void InitializeOpenGL(int Width, int Height);

	bool bIsRenderingWireframe;
	HWND Hwnd;
	RECT ScreenRect;
	HDC	Hdc;
	HGLRC Hrc;
};

#endif // GraphicsContext_h__
