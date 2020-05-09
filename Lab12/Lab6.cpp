#include "stdafx.h"
#include "Lab6.h"
#include <cmath>

#define MAX_LOADSTRING 100

HINSTANCE hInst;								
TCHAR szTitle[MAX_LOADSTRING];				
TCHAR szWindowClass[MAX_LOADSTRING];		


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


	MSG msg;
	HACCEL hAccelTable;


	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LAB6, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);


	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB6));

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
	HBRUSH hBrush = CreateSolidBrush(RGB(216, 158, 216));
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
int winHeight = 300;

HANDLE thread1, thread2;

struct ThreadArgs {
	int time;
	int x, y;
} args1, args2;

DWORD WINAPI MoveObject(ThreadArgs* args) {
	while (true) {
		args->time -= 10;
		if (args->time < 0)
			args->time += winWidth;
		args->x = args->time;
		args->y = 100;
		Sleep(20);
	}
	return 0;
}

bool active = false;

#define THREADCOUNT 2 

HANDLE ghWriteEvent;
HANDLE ghThreads[THREADCOUNT];

DWORD WINAPI FirstThreadMove(LPVOID);
DWORD WINAPI SecondThreadMove(LPVOID);


void InitThreadsnEvent(void)
{
	int i;
	DWORD dwThreadID;

	ghWriteEvent = CreateEvent(
		NULL,              
		TRUE,             
		FALSE,              
		TEXT("WriteEvent")
	);

	if (ghWriteEvent == NULL)
	{
		return;
	}

	ghThreads[0] = CreateThread(
		NULL,             
		0,                
		FirstThreadMove,      
		NULL,             
		0,                
		&dwThreadID);

	ghThreads[1] = CreateThread(
		NULL,            
		0,               
		SecondThreadMove,        
		NULL,             
		0,              
		&dwThreadID);

	if (ghThreads[0] == NULL)
	{
		return;
	}
}
int X = 500;
int Y = 110;
CRITICAL_SECTION CriticalSection;

DWORD WINAPI FirstThreadMove(LPVOID lpParam)
{
	while (true) {
		EnterCriticalSection(&CriticalSection);
		Y = (Y + 5) % winHeight;
		LeaveCriticalSection(&CriticalSection);
		Sleep(1);
	}
	return 1;
}

DWORD WINAPI SecondThreadMove(LPVOID lpParam)
{
	while (true) {
		EnterCriticalSection(&CriticalSection);
		Y = (Y - 5) % winHeight;
		LeaveCriticalSection(&CriticalSection);
		Sleep(1);
	}
	return 1;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 1200, 400, NULL, NULL, hInstance, NULL);

	CreateWindow(L"BUTTON", L"Start", WS_CHILD | WS_VISIBLE,
		20, 10, 1000, 30, hWnd, (HMENU)START_BUTTON_ID, hInstance, NULL);
	CreateWindow(L"BUTTON", L"Stop", WS_CHILD | WS_VISIBLE,
		20, 300, 1000, 30, hWnd, (HMENU)STOP_BUTTON_ID, hInstance, NULL);

	args1.time = 0;
	args2.time = 100;

	thread1 = CreateThread(NULL, 0u, (LPTHREAD_START_ROUTINE)MoveObject, &args1, 0, NULL);
	SuspendThread(thread1);

	thread2 = CreateThread(NULL, 0u, (LPTHREAD_START_ROUTINE)MoveObject, &args2, 0, NULL);
	SuspendThread(thread2);

	if (!InitializeCriticalSectionAndSpinCount(&CriticalSection,
		0x00000400))
		return 1;
	InitThreadsnEvent();
	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

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
		switch (wmId)
		{
		case START_BUTTON_ID:
			if (!active) {
				ResumeThread(thread1);
				ResumeThread(thread2);

				SetTimer(hWnd, NULL, 20, NULL);

				active = true;
			}
			break;
		case STOP_BUTTON_ID:
			if (active) {
				SuspendThread(thread1);
				SuspendThread(thread2);
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
		InvalidateRect(hWnd, NULL, true);
		if (active)
			SetTimer(hWnd, NULL, 20, NULL);
		break;
	case WM_SIZE:
		winWidth = LOWORD(lParam);
		winHeight = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (args1.y != 0)
		{
			HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0));
			SelectObject(hdc, brush);

			brush = CreateSolidBrush(RGB(153, 65, 103));
			SelectObject(hdc, brush);
			Rectangle(hdc, args1.x, Y, args1.x + 100, Y + 100);
		}
		EndPaint(hWnd, &ps);
		break;
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
