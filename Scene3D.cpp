#include "Scene3D.h"
#include "SOIL.h"
#include "Primitives.h"
#include <cmath>

void Scene3D::Init(HWND* wnd, FInput* in)
{
	hwnd = wnd;
	input = in;

	GetClientRect(*hwnd, &screenRect);	//get rect into our handy global rect
	InitializeOpenGL(screenRect.right, screenRect.bottom); // initialise openGL

	// OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	//glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glEnable(GL_COLOR);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	myCrateTexture = SOIL_load_OGL_texture
	(
		"crate.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	myTextTexture = SOIL_load_OGL_texture
	(
		"grass1.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	cubeRotation = 0;
	cameraYaw = 0;
	cameraPitch = 0;

	model.Load("Models/mesh.obj", "Models/mage_Atlas-tex.png");
}

FPrimitiveInfo Scene3D::CreateSphere(int NumberOfSubdivisions)
{
	struct FSphereHelper
	{
		static FVertex3D CalculateSpherePoint(float Theta, float Delta)
		{
			return FVertex3D(cos(Theta) * sin(Delta), cos(Delta), sin(Theta) * sin(Delta));
		}
	};

	FPrimitiveInfo Primitive;
	Primitive.Initialise(NumberOfSubdivisions * NumberOfSubdivisions * 6, NumberOfSubdivisions * NumberOfSubdivisions * 6);

	float Theta = 0.0f; // Latitude
	float Delta = 0.0f; // Longitude

	float ThetaDifference = (M_PI) / NumberOfSubdivisions;
	float DeltaDifference = (2 * M_PI) / NumberOfSubdivisions;

	float DeltaTexcoord = 1.0f / NumberOfSubdivisions;

	int CurrentVertex = 0;
	int CurrentIndex = 0;

	float CurrentU = 0.0f;
	float CurrentV = 0.0f;

	for(int Longitude = 0; Longitude < NumberOfSubdivisions; Longitude++)
	{
		Theta = 0.0f;
		CurrentU = 0.0f;

		for(int Latitude = 0; Latitude < NumberOfSubdivisions; Latitude++)
		{

			Primitive.positions[CurrentVertex] = FSphereHelper::CalculateSpherePoint(Theta, Delta);
			Primitive.positions[CurrentVertex + 1] = FSphereHelper::CalculateSpherePoint(Theta, Delta + DeltaDifference);
			Primitive.positions[CurrentVertex + 2] = FSphereHelper::CalculateSpherePoint(Theta + ThetaDifference, Delta + DeltaDifference);

			Primitive.positions[CurrentVertex + 3] = FSphereHelper::CalculateSpherePoint(Theta, Delta);
			Primitive.positions[CurrentVertex + 4] = FSphereHelper::CalculateSpherePoint(Theta + ThetaDifference, Delta + DeltaDifference);
			Primitive.positions[CurrentVertex + 5] = FSphereHelper::CalculateSpherePoint(Theta + ThetaDifference, Delta);

			Primitive.texcoords[CurrentVertex] = FVertex2D(CurrentU, CurrentV);
			Primitive.texcoords[CurrentVertex + 1] = FVertex2D(CurrentU, CurrentV + DeltaTexcoord);
			Primitive.texcoords[CurrentVertex + 2] = FVertex2D(CurrentU + DeltaTexcoord, CurrentV + DeltaTexcoord);

			Primitive.texcoords[CurrentVertex + 3] = FVertex2D(CurrentU, CurrentV);
			Primitive.texcoords[CurrentVertex + 4] = FVertex2D(CurrentU + DeltaTexcoord, CurrentV + DeltaTexcoord);
			Primitive.texcoords[CurrentVertex + 5] = FVertex2D(CurrentU + DeltaTexcoord, CurrentV);

			for(int k = 0; k < 6; k++)
			{
				Primitive.indices[CurrentIndex + k] = CurrentVertex + k;
				Primitive.normals[CurrentVertex + k] = Primitive.positions[CurrentVertex + k];
			}

			CurrentVertex += 6;
			CurrentIndex += 6;

			Theta += ThetaDifference;
			CurrentU += DeltaTexcoord;
		}

		CurrentV += DeltaTexcoord;
		Delta += DeltaDifference;
	}

	return Primitive;
}

FPrimitiveInfo Scene3D::CreateDisc(int NumberOfSubdivisions)
{
	FPrimitiveInfo Primitive;
	Primitive.Initialise(NumberOfSubdivisions * 3, NumberOfSubdivisions * 3);

	int CurrentVertex = 0;
	float Theta = 0.0f;
	float DeltaTheta = (2 * M_PI) / NumberOfSubdivisions;
	for(int i = 0; i < NumberOfSubdivisions; i++)
	{
		float PreviousTheta = Theta;
		Theta += DeltaTheta;

		Primitive.positions[CurrentVertex] = FVertex3D(cos(PreviousTheta), sin(PreviousTheta), 0.0f);
		Primitive.positions[CurrentVertex + 1] = FVertex3D(0.0f, 0.0f, 0.0f);
		Primitive.positions[CurrentVertex + 2] = FVertex3D(cos(Theta), sin(Theta), 0.0f);

		Primitive.indices[CurrentVertex] = CurrentVertex;
		Primitive.indices[CurrentVertex + 1] = CurrentVertex + 1;
		Primitive.indices[CurrentVertex + 2] = CurrentVertex + 2;

		Primitive.normals[CurrentVertex] = Primitive.normals[CurrentVertex + 1] = Primitive.normals[CurrentVertex + 2] = FVertex3D(0.0f, 0.0f, 1.0f);

		for(int k = 0; k < 3; k++)
		{
			Primitive.texcoords[CurrentVertex + k] = FVertex2D(
				Primitive.positions[CurrentVertex + k].x * 0.5f + 0.5f,
				Primitive.positions[CurrentVertex + k].y * 0.5f + 0.5f);
		}


		CurrentVertex += 3;
	}

	return Primitive;
}

void setupDirectionalLight(GLint lightNumber, float *ambient, float *diffuse, float *direction)
{
	glLightfv(lightNumber, GL_AMBIENT,  ambient);
	glLightfv(lightNumber, GL_DIFFUSE,  diffuse);
	glLightfv(lightNumber, GL_POSITION, direction);
	glEnable(lightNumber);
}

void Scene3D::DrawScene(float dt) 
{
	HandleInput(dt);

	int i=0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Clear The Screen And The Depth Buffer
	
	glLoadIdentity();// load Identity Matrix
	
	glPushMatrix();

	GLfloat Light_Ambient[]  =	{ 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat Light_Diffuse[]  =   { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat Light_Position[] =   { -0.8f, 0.0f, 1.0f, 0.0f };

	setupDirectionalLight(GL_LIGHT0, Light_Ambient, Light_Diffuse, Light_Position);

	glPopMatrix();

	glLoadIdentity();// load Identity Matrix

	//set camera looking down the -z axis,  6 units away from the center
	gluLookAt(0, 0, -20, 0, 0, 0, 0, 1, 0); //Where we are, What we look at, and which way is up
	glRotatef(cameraYaw, 0, 1, 0);
	glRotatef(cameraPitch, 1, 0, 0);


	glDisable(GL_LIGHTING);
	glPushMatrix();
	glScalef(0.1f, 0.1f, 0.1f);

	model.Render();

	glRotatef(cubeRotation, 0, 1, 0);
	glScalef(2.0f, 2.0f, 2.0f);

	glPopMatrix();

	SwapBuffers(hdc);// Swap the frame buffers.
}		

void Scene3D::DrawSphere(float r, float g, float b, float a)
{
	glColor4f(r, g, b, a);
	gluSphere(gluNewQuadric(), 0.20, 20, 20);
}

int xTexture = 0;
int yTexture = 0;

void glLowerLeft()
{
	glTexCoord2f(xTexture * 0.25f, yTexture * 0.25f);
}

void glLowerRight()
{
	glTexCoord2f(xTexture * 0.25f + 0.25f, yTexture * 0.25f);
}

void glUpperLeft()
{
	glTexCoord2f(xTexture * 0.25f, yTexture * 0.25f + 0.25f);
}

void glUpperRight()
{
	glTexCoord2f(xTexture * 0.25f + 0.25f, yTexture * 0.25f + 0.25f);
}

void Scene3D::DrawCubeSkybox(float xScale, float yScale, float zScale)
{
	glPushMatrix();
	glScalef(xScale, yScale, zScale);

	const float kHalfCubeSize = 0.5f;
	glBegin(GL_TRIANGLES);

	const float kFaceWidth = 0.25f;

	xTexture = 1;
	yTexture = 2;

	// Bottom
	glColor3f(1.0f, 1.0f, 1.0f); 
	glNormal3f(0, -1, 0); glLowerLeft(); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize); 
	glNormal3f(0, -1, 0); glLowerRight(); glVertex3f(kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize); 
	glNormal3f(0, -1, 0); glUpperLeft(); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize); 
	glNormal3f(0, -1, 0); glUpperLeft(); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize);
	glNormal3f(0, -1, 0); glLowerRight(); glVertex3f(kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize); 
	glNormal3f(0, -1, 0); glUpperRight(); glVertex3f(kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize);

	xTexture = 1;
	yTexture = 0;

	// Top
	glColor3f(1.0f, 1.0f, 1.0f); 
	glNormal3f(0, 1, 0); glLowerLeft(); glVertex3f(kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(0, 1, 0); glLowerRight(); glVertex3f(-kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(0, 1, 0); glUpperLeft(); glVertex3f(kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);
	glNormal3f(0, 1, 0); glLowerRight(); glVertex3f(-kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(0, 1, 0); glUpperRight(); glVertex3f(-kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);
	glNormal3f(0, 1, 0); glUpperLeft(); glVertex3f(kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);

	xTexture = 1;
	yTexture = 1;

	// Front
	glColor3f(1.0f, 1.0f, 1.0f); 
	glNormal3f(0, 0, 1); glUpperRight(); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize);
	glNormal3f(0, 0, 1); glUpperLeft(); glVertex3f(kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize);
	glNormal3f(0, 0, 1); glLowerRight(); glVertex3f(-kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);
	glNormal3f(0, 0, 1); glUpperLeft(); glVertex3f(kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize);
	glNormal3f(0, 0, 1); glLowerLeft(); glVertex3f(kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);
	glNormal3f(0, 0, 1); glLowerRight(); glVertex3f(-kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);

	xTexture = 3;
	yTexture = 1;

	// Back
	glColor3f(1.0f, 1.0f, 1.0f); 
	glNormal3f(0, 0, -1); glLowerLeft(); glVertex3f(-kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(0, 0, -1); glLowerRight(); glVertex3f(kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(0, 0, -1); glUpperLeft(); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(0, 0, -1); glLowerRight(); glVertex3f(kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(0, 0, -1); glUpperRight(); glVertex3f(kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(0, 0, -1); glUpperLeft(); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize);

	xTexture = 0;
	yTexture = 1;

	// Left 
	glColor3f(1.0f, 1.0f, 1.0f); 
	glNormal3f(1, 0, 0); glUpperRight(); glVertex3f(kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize);
	glNormal3f(1, 0, 0); glUpperLeft(); glVertex3f(kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(1, 0, 0); glLowerRight(); glVertex3f(kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);
	glNormal3f(1, 0, 0); glUpperLeft(); glVertex3f(kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(1, 0, 0); glLowerLeft(); glVertex3f(kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(1, 0, 0); glLowerRight(); glVertex3f(kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);

	xTexture = 2;
	yTexture = 1;

	// Right
	glColor3f(1.0f, 1.0f, 1.0f); 
	glNormal3f(-1, 0, 0); glLowerLeft(); glVertex3f(-kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);
	glNormal3f(-1, 0, 0); glLowerRight(); glVertex3f(-kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(-1, 0, 0); glUpperLeft(); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize);
	glNormal3f(-1, 0, 0); glLowerRight(); glVertex3f(-kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(-1, 0, 0); glUpperRight(); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(-1, 0, 0); glUpperLeft(); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize);

	glEnd();

	glPopMatrix();
}

void Scene3D::DrawCube(float xScale, float yScale, float zScale)
{
	glPushMatrix();
	glScalef(xScale, yScale, zScale);

	const float kHalfCubeSize = 0.5f;
	glBegin(GL_TRIANGLES);

	// Bottom
	glColor3f(1.0f, 1.0f, 1.0f); 
	glNormal3f(0, -1, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize); 
	glNormal3f(0, -1, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize); 
	glNormal3f(0, -1, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize); 
	glNormal3f(0, -1, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize);
	glNormal3f(0, -1, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize); 
	glNormal3f(0, -1, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize);

	// Top
	glColor3f(1.0f, 1.0f, 1.0f); 
	glNormal3f(0, 1, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(0, 1, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(-kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(0, 1, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);
	glNormal3f(0, 1, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(-kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(0, 1, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(-kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);
	glNormal3f(0, 1, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);

	// Front
	glColor3f(1.0f, 1.0f, 1.0f); 
	glNormal3f(0, 0, 1); glTexCoord2f(0.0f, 0.0f); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize);
	glNormal3f(0, 0, 1); glTexCoord2f(1.0f, 0.0f); glVertex3f(kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize);
	glNormal3f(0, 0, 1); glTexCoord2f(0.0f, 1.0f); glVertex3f(-kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);
	glNormal3f(0, 0, 1); glTexCoord2f(1.0f, 0.0f); glVertex3f(kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize);
	glNormal3f(0, 0, 1); glTexCoord2f(1.0f, 1.0f); glVertex3f(kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);
	glNormal3f(0, 0, 1); glTexCoord2f(0.0f, 1.0f); glVertex3f(-kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);

	// Back
	glColor3f(1.0f, 1.0f, 1.0f); 
	glNormal3f(0, 0, -1); glTexCoord2f(0.0f, 0.0f); glVertex3f(-kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(0, 0, -1); glTexCoord2f(1.0f, 0.0f); glVertex3f(kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(0, 0, -1); glTexCoord2f(0.0f, 1.0f); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(0, 0, -1); glTexCoord2f(1.0f, 0.0f); glVertex3f(kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(0, 0, -1); glTexCoord2f(1.0f, 1.0f); glVertex3f(kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(0, 0, -1); glTexCoord2f(0.0f, 1.0f); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize);

	// Right 
	glColor3f(1.0f, 1.0f, 1.0f); 
	glNormal3f(1, 0, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize);
	glNormal3f(1, 0, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(1, 0, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);
	glNormal3f(1, 0, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(1, 0, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(1, 0, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);

	// Left
	glColor3f(1.0f, 1.0f, 1.0f); 
	glNormal3f(-1, 0, 0); glTexCoord2f(0.0f, 1.0f); glVertex3f(-kHalfCubeSize, kHalfCubeSize, kHalfCubeSize);
	glNormal3f(-1, 0, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(-kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(-1, 0, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize);
	glNormal3f(-1, 0, 0); glTexCoord2f(0.0f, 0.0f); glVertex3f(-kHalfCubeSize, kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(-1, 0, 0); glTexCoord2f(1.0f, 0.0f); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, -kHalfCubeSize);
	glNormal3f(-1, 0, 0); glTexCoord2f(1.0f, 1.0f); glVertex3f(-kHalfCubeSize, -kHalfCubeSize, kHalfCubeSize);

	glEnd();

	glPopMatrix();
}

void Scene3D::Resize()
{
	if(hwnd == NULL)
		return;

	GetClientRect(*hwnd, &screenRect);	
	ResizeGLWindow(screenRect.right, screenRect.bottom);	
}

void Scene3D::HandleInput(float dt)
{
	const float kCameraSpeed = 90;

	if(input->IsKeyDown(VK_RIGHT))
	{
		cameraYaw += kCameraSpeed * dt;
	}
	else if(input->IsKeyDown(VK_LEFT))
	{
		cameraYaw -= kCameraSpeed * dt;
	}
	else if(input->IsKeyDown(VK_DOWN))
	{
		cameraPitch += kCameraSpeed * dt;
	}
	else if(input->IsKeyDown(VK_UP))
	{
		cameraPitch -= kCameraSpeed * dt;
	}
}
