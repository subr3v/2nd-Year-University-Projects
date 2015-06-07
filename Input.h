#ifndef INPUT_H
#define INPUT_H

#include <Windows.h>
#include "window.h"

#include <glm/glm.hpp>
#include "Geometry.h"

enum MouseButton
{
	MouseButton_Right,
	MouseButton_Left,
	MouseButton_Middle,
};

class FInput
{
public:
	FInput();
	~FInput();

	bool IsKeyDown(int Key) const
	{
		return Keys[Key];
	}

	bool IsKeyPressed(int Key) const
	{
		return KeysPressed[Key];
	}

	bool IsMouseButtonDown(MouseButton Button) const
	{
		switch(Button)
		{
		case MouseButton_Right:
			return bMouseRight;
		case MouseButton_Left:
			return bMouseLeft;
		case MouseButton_Middle:
			return bMouseMiddle;
		default:
			return false;
		}
	}

	bool IsMouseButtonPressed(MouseButton Button) const
	{
		switch (Button)
		{
		case MouseButton_Right:
			return bMouseRightPressed;
		case MouseButton_Left:
			return bMouseLeftPressed;
		case MouseButton_Middle:
			return bMouseMiddlePressed;
		default:
			return false;
		}
	}

	void UpdateFrame();

	glm::vec2 GetMousePosition() const { return glm::vec2(MouseX, MouseY); }
	void SetMousePosition(int X, int Y);
	void CenterMouseScreen(class FWindow* Window);
	FRay GetMouseRay(const glm::vec3& CameraPosition) const;
private:
	void HandleWindowsMessage(MSG Message);
	friend class FWindow;

	int MouseX, MouseY;
	bool Keys[256];
	bool KeysPressed[256];
	bool bMouseLeft, bMouseRight, bMouseMiddle;
	bool bMouseLeftPressed, bMouseRightPressed, bMouseMiddlePressed;

	glm::mat4 ModelMatrix;
	glm::mat4 ProjectionMatrix;
	glm::ivec4 Viewport;
};

#endif // INPUT_H