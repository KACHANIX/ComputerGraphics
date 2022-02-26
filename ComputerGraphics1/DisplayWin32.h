#pragma once

#include <activation.h>

class DisplayWin32
{
public:
	int ClientHeight = 0;
	int ClientWidth = 0;

	HWND hWnd = nullptr;
	WNDCLASSEX wc = {};
	static HMODULE Module;

	DisplayWin32(LPCWSTR windowName, int width, int height, WNDPROC WndProc);
};
