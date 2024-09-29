#include <windows.h>
#include "WindowState.h"

global global_state GlobalState;

LRESULT OSCallBack(HWND WindowHandle, UINT Massage, WPARAM WParam, LPARAM LParam) 
{
	LRESULT Result = {};
	switch (Massage) 
	{
		case WM_CLOSE:
		case WM_DESTROY:
		{GlobalState.IsRunning = false; } break;

		default:
		{Result = DefWindowProcA(WindowHandle, Massage, WParam, LParam); }
		break;
	}
	return Result;
}

b32 ProcessPixelColor(u32 X, u32 Y) 
{
	GlobalState.FrameBufferPixels[Y * GlobalState.FrameBufferWidth + X] = X + Y;
	return true;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	GlobalState.IsRunning = true;
	WNDCLASSA windowClass = {};
	{
		windowClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
		windowClass.lpfnWndProc = OSCallBack;
		windowClass.hInstance = hInstance;
		windowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
		windowClass.lpszClassName = WindowName;
	}

	if (!RegisterClassA(&windowClass)) { InvalidCodePath; }

	GlobalState.WindowHandle = CreateWindowExA(
		0,
		windowClass.lpszClassName,
		WindowName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WindowStartWidth,
		WindowStartHeight,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (!GlobalState.WindowHandle) { InvalidCodePath; }

	GlobalState.DeviceContext = GetDC(GlobalState.WindowHandle);
	if (!GlobalState.DeviceContext) { InvalidCodePath; }

	RECT clientRect = {};
	if (!GetClientRect(GlobalState.WindowHandle, &clientRect)) { InvalidCodePath; }

	GlobalState.FrameBufferWidth = clientRect.right - clientRect.left;
	GlobalState.FrameBufferHeight = clientRect.bottom - clientRect.top;
	GlobalState.FrameBufferPixels = new u32[GlobalState.FrameBufferWidth * GlobalState.FrameBufferHeight];

	while (GlobalState.IsRunning) 
	{
		MSG Message = {};
		while (PeekMessageA(&Message, GlobalState.WindowHandle, 0, 0, PM_REMOVE)) 
		{
			switch (Message.message)
			{
				case WM_QUIT:
				{GlobalState.IsRunning = false; } break;

				default:
				{	TranslateMessage(&Message); 
					DispatchMessage(&Message); 
				} break;
			}
		}

		BITMAPINFO Bitmapinfo = {};
		{
			Bitmapinfo.bmiHeader.biSize = sizeof(tagBITMAPINFOHEADER);
			Bitmapinfo.bmiHeader.biWidth = GlobalState.FrameBufferWidth;
			Bitmapinfo.bmiHeader.biHeight = GlobalState.FrameBufferHeight;
			Bitmapinfo.bmiHeader.biPlanes = 1;
			Bitmapinfo.bmiHeader.biBitCount = 32;
			Bitmapinfo.bmiHeader.biCompression = BI_RGB;
		}

		for (u32 Y = 0; Y < GlobalState.FrameBufferHeight; Y++) 
		{
			for (u32 X = 0; X < GlobalState.FrameBufferWidth; X++) 
			{
				if (!ProcessPixelColor(X, Y)) { InvalidCodePath; }
			}
		}

		RECT clientRect = {};
		if (!GetClientRect(GlobalState.WindowHandle, &clientRect)) { InvalidCodePath; }
		u32 clientWidth = clientRect.right - clientRect.left;
		u32 clientHeight = clientRect.bottom - clientRect.top;

		if (!StretchDIBits(GlobalState.DeviceContext, 0, 0, clientWidth, clientHeight, 0, 0, GlobalState.FrameBufferWidth, GlobalState.FrameBufferHeight, GlobalState.FrameBufferPixels, &Bitmapinfo, DIB_RGB_COLORS, SRCCOPY))
		{ InvalidCodePath; }
	}

	delete GlobalState.FrameBufferPixels;
	return 0;
}