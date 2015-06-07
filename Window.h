#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include <stdio.h>
#include <string>

class FWindow
{
public:
	FWindow(HINSTANCE hInstance, int nCmdShow, int Width, int Height);
	~FWindow();

	bool Update(class FInput* InputManager);
	HWND GetHandle() { return Hwnd; }

	int GetWidth();
	int GetHeight();

	static void ShowSimpleMessage(LPSTR MessageText);

	void SetWindowTitle(const std::string& Title);
private:
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	void RegisterWindow(HINSTANCE);
	BOOL InitialiseMyWindow(HINSTANCE, int);
	static LRESULT CALLBACK S_WndProc (HWND, UINT, WPARAM, LPARAM);

	HWND Hwnd;
	int WindowWidth, WindowHeight;
};

#endif // WINDOW_H