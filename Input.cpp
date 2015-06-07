#include "Input.h"
#include <iostream>

#include <windows.h>
#include <gl/GL.h>
#include "glm/gtc/matrix_transform.hpp"

FInput::FInput() : MouseX(0), MouseY(0), bMouseLeft(false), bMouseRight(false), bMouseMiddle(false), bMouseLeftPressed(false), bMouseRightPressed(false), bMouseMiddlePressed(false)
{
	memset(Keys, 0, sizeof(Keys));
}

FInput::~FInput()
{

}

void FInput::SetMousePosition(int X, int Y)
{
	SetCursorPos(X, Y);
}

void FInput::HandleWindowsMessage(MSG Message)
{
	char MessageKey = Message.wParam;
	switch (Message.message)
	{
	case WM_KEYDOWN:
		if (Keys[MessageKey] == false)
		{
			Keys[MessageKey] = KeysPressed[MessageKey] = true;
		}
		break;
	case WM_KEYUP:
		Keys[MessageKey] = KeysPressed[MessageKey] = false;
		break;
	case WM_RBUTTONDOWN:
		if (bMouseRight == false)
			bMouseRight = bMouseRightPressed = true;
		break;
	case WM_MBUTTONDOWN:
		if (bMouseMiddle == false)
			bMouseMiddle = bMouseMiddlePressed = true;
		break;
	case WM_LBUTTONDOWN:
		if (bMouseLeft == false)
			bMouseLeft = bMouseLeftPressed = true;
		break;
	case WM_RBUTTONUP:
		bMouseRight = bMouseRightPressed = false;
		break;
	case WM_MBUTTONUP:
		bMouseMiddle = bMouseMiddlePressed = false;
		break;
	case WM_LBUTTONUP:
		bMouseLeft = bMouseLeftPressed = false;
		break;
	case WM_MOUSEMOVE:
		MouseX = LOWORD(Message.lParam);
		MouseY = HIWORD(Message.lParam);
		break;
	default:
		break;
	}
}

FRay FInput::GetMouseRay(const glm::vec3& CameraPosition) const
{
	glm::vec3 MouseRay;
	glm::vec2 MousePosition = GetMousePosition();
	MousePosition.y = Viewport[3] - MousePosition.y;

	MouseRay = glm::unProject(glm::vec3(MousePosition, 1.0f), ModelMatrix, ProjectionMatrix, Viewport);

	const float kRayLength = 100000.0f;

	glm::vec3 RayDirection = glm::normalize(MouseRay - CameraPosition);
	return FRay(CameraPosition, RayDirection, kRayLength);
}

void FInput::UpdateFrame()
{
	for (int i = 0; i < 256; i++)
	{
		KeysPressed[i] = false;
	}
	bMouseLeftPressed = bMouseMiddlePressed = bMouseRightPressed = false;

	glGetFloatv(GL_PROJECTION_MATRIX, &ProjectionMatrix[0][0]);
	glGetFloatv(GL_MODELVIEW_MATRIX, &ModelMatrix[0][0]);
	glGetIntegerv(GL_VIEWPORT, &Viewport[0]);
}

void FInput::CenterMouseScreen(class FWindow* Window)
{
	POINT WindowCenter;
	WindowCenter.x = Window->GetWidth() / 2;
	WindowCenter.y = Window->GetHeight() / 2;
	ClientToScreen(Window->GetHandle(), &WindowCenter);

	SetMousePosition(WindowCenter.x, WindowCenter.y);
}
