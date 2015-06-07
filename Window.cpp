#include "window.h"
#include "Input.h"

FWindow::FWindow(HINSTANCE hInstance, int nCmdShow, int Width, int Height) : WindowWidth(Width), WindowHeight(Height)
{
	RegisterWindow(hInstance);

	Hwnd = CreateWindow ("FirstWindowClass",
		"Graphics Programming Coursework",
		WS_OVERLAPPED | WS_THICKFRAME | WS_MAXIMIZEBOX,
		0,
		0,
		Width,
		Height,
		NULL,
		NULL,
		hInstance,
		NULL);

	SetWindowLong(Hwnd, GWL_USERDATA, (LONG)this);

	ShowWindow(Hwnd, nCmdShow);
	UpdateWindow(Hwnd);
}

FWindow::~FWindow()
{
	DestroyWindow(Hwnd);
}

bool FWindow::Update(class FInput* InputManager)
{
	InputManager->UpdateFrame();

	MSG Message;
	while (PeekMessage(&Message,NULL,0,0,PM_REMOVE))
	{
		if (Message.message == WM_QUIT)
			return false;
		if (InputManager != nullptr)
		{
			InputManager->HandleWindowsMessage(Message);
		}
		TranslateMessage (&Message);
		DispatchMessage (&Message);
	}

	return true;
}

// Defines the window
void FWindow::RegisterWindow(HINSTANCE hInstance)
{
	WNDCLASSEX  wcex;

	wcex.cbSize        = sizeof (wcex);
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = S_WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = 0; 
	wcex.hCursor       = LoadCursor (NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
	wcex.lpszMenuName  = NULL;
	wcex.lpszClassName = "FirstWindowClass";
	wcex.hIconSm       = 0;

	RegisterClassEx (&wcex);
}

// Attempts to create the window and display it
BOOL FWindow::InitialiseMyWindow(HINSTANCE hInstance, int nCmdShow)
{
	Hwnd = CreateWindow ("FirstWindowClass",
		"My First Window",
		WS_OVERLAPPED | WS_MAXIMIZEBOX,
		0,
		0,
		WindowWidth,
		WindowHeight,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!Hwnd)
	{
		return FALSE;
	}

	ShowWindow(Hwnd, nCmdShow);
	UpdateWindow(Hwnd);
	return TRUE;
}

LRESULT CALLBACK FWindow::WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		break;

	case WM_SIZE:

		break;

	case WM_PAINT:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc (hwnd, message, wParam, lParam);
}

void FWindow::ShowSimpleMessage(LPSTR messageText)
{
	MessageBox (NULL, messageText,  "MessageBox", MB_OK);
}

// handles window messages
LRESULT CALLBACK FWindow::S_WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FWindow* window = (FWindow*)GetWindowLong(hwnd,GWL_USERDATA);
	return window->WndProc(hwnd, message, wParam, lParam);
}

int FWindow::GetWidth()
{
	RECT wRect;
	GetWindowRect(Hwnd, &wRect);
	return wRect.right - wRect.left;
}

int FWindow::GetHeight()
{
	RECT wRect;
	GetWindowRect(Hwnd, &wRect);
	return wRect.bottom - wRect.top;
}

void FWindow::SetWindowTitle(const std::string& Title)
{
	SetWindowText(Hwnd, Title.c_str());
}
