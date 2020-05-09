#include "stdafx.h"
#include "Lab6.h"
#include <cmath>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDC_LAB6, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LAB6, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB6));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	HBRUSH hBrush = CreateSolidBrush(RGB(216, 158, 216));;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB6));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = hBrush;
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_LAB6);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

#define START_BUTTON_ID 1001
#define STOP_BUTTON_ID 1002

int winWidth = 600;

HANDLE firstProgressThread, secondProgressThread;
bool active = false;
bool isFinished = false;

struct ThreadArgs {
	int time;
	int x, y;
} args1, args2;

DWORD WINAPI MoveFirstProgressBar(ThreadArgs* args) {
	while (true) {
		if (args->x < 800)
		{
			args->time += 5;
			args->x = args->time % winWidth;
			args->y = sin((double)(args->time) / 100) * 100 + 150;
			Sleep(50);
		}
	}
	return 0;
}

DWORD WINAPI MoveSecondProgressBar(ThreadArgs* args) {
	while (true) {
		if (args->x < 800)
		{
			args->time += 10;
			args->x = args->time % winWidth;
			args->y = sin((double)(args->time) / 100) * 100 + 150 + 100;
			Sleep(50);
		}
	}
	return 0;
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 1200, 400, NULL, NULL, hInstance, NULL);

	CreateWindow(L"BUTTON", L"Start", WS_CHILD | WS_VISIBLE,
		50, 10, 950, 30, hWnd, (HMENU)START_BUTTON_ID, hInstance, NULL);
	CreateWindow(L"BUTTON", L"Stop", WS_CHILD | WS_VISIBLE,
		50, 250, 950, 30, hWnd, (HMENU)STOP_BUTTON_ID, hInstance, NULL);

	args1.time = 0;
	args2.time = 100;

	firstProgressThread = CreateThread(NULL, 0u, (LPTHREAD_START_ROUTINE)MoveFirstProgressBar, &args1, 0, NULL);
	SuspendThread(firstProgressThread);

	secondProgressThread = CreateThread(NULL, 0u, (LPTHREAD_START_ROUTINE)MoveSecondProgressBar, &args2, 0, NULL);
	SuspendThread(secondProgressThread);

	if (!hWnd)
	{
		return FALSE;
	}
	if (!isFinished)
	{
		// updating window to show progress
		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);
	}

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case START_BUTTON_ID:
			if (!active && !isFinished)
			{
			start:
				ResumeThread(firstProgressThread);
				ResumeThread(secondProgressThread);

				SetTimer(hWnd, NULL, 20, NULL);
			}
			else if (isFinished)
			{
				args1.time = 0;
				args1.x = 0;
				args2.time = 100;
				args2.x = 0;

				InvalidateRect(hWnd, NULL, true);

				ResumeThread(firstProgressThread);
				ResumeThread(secondProgressThread);

				SetTimer(hWnd, NULL, 20, NULL);

				isFinished = false;
				active = true;
			}
			break;
		case STOP_BUTTON_ID:
			if (active) {
				SuspendThread(firstProgressThread);
				SuspendThread(secondProgressThread);
				active = false;
			}
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_TIMER:
		if (!isFinished)
		{
			InvalidateRect(hWnd, NULL, true);
			if (active)
				SetTimer(hWnd, NULL, 20, NULL);
		}
		break;
	case WM_SIZE:
		winWidth = LOWORD(lParam);
		break;
	case WM_PAINT:
		if (!isFinished)
		{
			// painting progress bars
			hdc = BeginPaint(hWnd, &ps);
			if (args1.y != 0)
			{
				HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
				SelectObject(hdc, pen);
				HBRUSH brush = CreateSolidBrush(RGB(153, 65, 103));

				Rectangle(hdc, 50, 70, 1000, 100);
				Rectangle(hdc, 50, 170, 1000, 200);


				// progress bar 1
				brush = args1.x < 800 ? CreateSolidBrush(RGB(153, 65, 103)) : CreateSolidBrush(RGB(76, 195, 177));
				SelectObject(hdc, brush);
				Rectangle(hdc, 50, 70, args1.x + 200, 100);

				// progress bar 2
				brush = args2.x < 800 ? CreateSolidBrush(RGB(153, 65, 103)) : CreateSolidBrush(RGB(76, 195, 177));
				SelectObject(hdc, brush);
				Rectangle(hdc, 50, 170, args2.x + 200, 200);
			}
			if (args1.x >= 800) {
				isFinished = true;
				TextOut(hdc, 500, 300, L"FINISHED", wcslen(L"FINISHED"));
				SuspendThread(firstProgressThread);
				SuspendThread(secondProgressThread);
				active = false;
			}
			EndPaint(hWnd, &ps);
			break;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
