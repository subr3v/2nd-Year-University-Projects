#include "GraphicsContext.h"
#include "SOIL.h"
#include <gl/gl.h>
#include <gl/glu.h>

#define COLOUR_DEPTH 16	//Colour depth

FGraphicsContext::FGraphicsContext(HWND Hwnd)
{
	this->Hwnd = Hwnd;

	GetClientRect(Hwnd, &ScreenRect);	//get rect into our handy global rect
	InitializeOpenGL(ScreenRect.right, ScreenRect.bottom); // initialise openGL

	// OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Gray Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glEnable(GL_COLOR);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glStencilMask(0xFF);
	glClearStencil(0);
}

bool FGraphicsContext::CreatePixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd = { 0 };
	int pixelformat;

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// Set the size of the structure
	pfd.nVersion = 1;							// Always set this to 1
	// Pass in the appropriate OpenGL flags
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.dwLayerMask = PFD_MAIN_PLANE;			// standard mask (this is ignored anyway)
	pfd.iPixelType = PFD_TYPE_RGBA;				// RGB and Alpha pixel type
	pfd.cColorBits = COLOUR_DEPTH;				// Here we use our #define for the color bits
	pfd.cDepthBits = COLOUR_DEPTH;				// Ignored for RBA
	pfd.cStencilBits = COLOUR_DEPTH;			// nothing for stencil
	pfd.cAccumBits = 0;							// nothing for accumulation

	//Gets a best match on the pixel format as passed in from device
	if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == false)
	{
		MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
		return false;
	}

	//sets the pixel format if its ok. 
	if (SetPixelFormat(hdc, pixelformat, &pfd) == false)
	{
		MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
		return false;
	}

	return true;
}

void FGraphicsContext::ResizeGLWindow(int Width, int Height)
{
	if (Height == 0)// Prevent A Divide By Zero error
	{
		Height = 1;// Make the Height Equal One
	}

	glViewport(0, 0, Width, Height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//calculate aspect ratio
	gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 1, 10000.0f);

	glMatrixMode(GL_MODELVIEW);// Select The Modelview Matrix
	glLoadIdentity();// Reset The Modelview Matrix
}

void FGraphicsContext::InitializeOpenGL(int Width, int Height)
{
	Hdc = GetDC(Hwnd);//  sets  global HDC

	if (!CreatePixelFormat(Hdc))//  sets  pixel format
		PostQuitMessage(0);

	Hrc = wglCreateContext(Hdc);	//  creates  rendering context from  hdc
	wglMakeCurrent(Hdc, Hrc);		//	Use this HRC.

	ResizeGLWindow(Width, Height);	// Setup the Screen
}

void FGraphicsContext::SwapBuffers()
{
	::SwapBuffers(Hdc);// Swap the frame buffers.
}

void FGraphicsContext::SetWireframeMode(bool bWireframe)
{
	GLenum PolygonMode = bWireframe ? GL_LINE : GL_FILL;
	glPolygonMode(GL_BACK, PolygonMode);
	glPolygonMode(GL_FRONT, PolygonMode);
	bIsRenderingWireframe = bWireframe;
}

void FGraphicsContext::EnableLighting(bool bEnable)
{
	if (bEnable)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
}

void FGraphicsContext::EnableTexturing(bool bEnable)
{
	if (bEnable)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}

void FGraphicsContext::EnableDepthTesting(bool bEnable)
{
	if (bEnable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void FGraphicsContext::ResetLighting()
{
	for (int i = 0; i < 8; i++)
	{
		glDisable(GL_LIGHT0 + i);
	}
}

