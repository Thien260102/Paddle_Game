//winmain.cpp - Windows framework source code file
#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>
#include <stdio.h>
#include "dxgraphics.h"
#include "game.h"

//window event callback function
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		//release Direct3D device
		if (d3ddev != NULL)
			d3ddev->Release();

		//release Direct3D object
		if (d3d != NULL)
			d3d->Release();

		// call the front-end to shutdown function
		Game_End(hwnd);

		//tell Windows to kill this program
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//helper function to set up the window properties
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	//fill the struct with info
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = APPTITILE;
	wc.hIconSm = NULL;

	//set up the window with the class info
	return RegisterClassEx(&wc);
}

// entry point for a Window program
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	MSG msg;
	HWND hwnd;

	//register the class 
	MyRegisterClass(hInstance);

	//set up the screen in windowed or fullscreen mode?
	DWORD style;
	if (FULLSCREEN)
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	else
		style = WS_OVERLAPPED;

	//create new window
	hwnd = CreateWindow(
		APPTITILE,
		APPTITILE,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	//was there an error creating window
	if (!hwnd)
		return FALSE;

	//display the window
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	if (!Init_Direct3D(hwnd, SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN))
		return 0;

	//initialize the game
	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"Error initializing the game", L"Error", MB_OK);
		return 0;
	}


	//main message loop
	int done = 0;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//look for quit message
			if (msg.message == WM_QUIT)
				done = 1;

			//decode and pass messages on to WinProc
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			Game_Run(hwnd);
	}
	return msg.wParam;

}