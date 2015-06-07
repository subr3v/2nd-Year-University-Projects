#ifndef SCENE3D_H
#define SCENE3D_H

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Input.h"
#include <memory>
#include "Shape.h"
#include "Primitives.h"
#include "Model.h"

#define COLOUR_DEPTH 16	//Colour depth

struct GLDisplayList
{
public:
	GLDisplayList()
	{
		handle = glGenLists(1);
	}

	void Begin()
	{
		glNewList(handle, GL_COMPILE);
	}

	void End()
	{
		glEndList();
	}

	void Call()
	{
		glCallList(handle);
	}
private:
	GLuint handle;
};

/*
class Scene3D
{
public:
	void Init(HWND*, FInput*);	//initialse function
	void DrawScene(float);	// render scene
	void Resize();

protected:
	bool CreatePixelFormat(HDC);
	void ResizeGLWindow(int, int);	//width and height
	void InitializeOpenGL(int, int); // width and height
	void HandleInput(float);

	void DrawRobotArm();

	void DrawSolarSystem();

	void DrawCube(float xScale, float yScale, float zScale);
	void DrawCubeSkybox(float xScale, float yScale, float zScale);
	void DrawSphere(float r, float g, float b, float a);
	void DrawPlane();

	FPrimitiveInfo CreateDisc(int NumberOfSubdivisions);
	FPrimitiveInfo CreateSphere(int NumberOfSubdivisions);

	float cubeRotation;

	float cameraYaw;
	float cameraPitch;

	Model model;

	FPrimitiveInfo Primitive;

	//vars
	Shape shape;
	HWND* hwnd;
	FInput* input;
	RECT screenRect;
	HDC	hdc;
	HGLRC hrc;			//hardware RENDERING CONTEXT
	int s_wdith, s_height;
	unsigned int myCrateTexture;
	unsigned int myTextTexture;

	std::unique_ptr<GLDisplayList> CubeList;
};
*/

#endif