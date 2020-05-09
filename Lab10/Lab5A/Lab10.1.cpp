

#include "stdafx.h"
#include "Lab10.1.h"

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

HWND SecondWindow;

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
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LAB5A, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB5A));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB5A));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LAB5A);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

#define RBUTTON11_ID 1001
#define RBUTTON12_ID 1002
#define RBUTTON13_ID 1003

#define RBUTTON21_ID 1004
#define RBUTTON22_ID 1005
#define RBUTTON23_ID 1006
#define RBUTTON24_ID 1007

#define CHECKBOX_ID 1008

// flags for messages that we're sending to the second window
#define DRAW_MESSAGE "DRAW"
#define COLOR_MESSAGE "COLOR"
#define SHAPE_MESSAGE "SHAPE"

UINT WM_DRAW;
UINT WM_COLOR;
UINT WM_SHAPE;


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 400, 300, NULL, NULL, hInstance, NULL);

   // initializing buttons for the first window
   CreateWindow(L"button", L"Red",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   10, 10, 100, 30, hWnd, (HMENU)RBUTTON11_ID, hInstance, NULL);
   CreateWindow(L"button", L"Green",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   10, 35, 100, 30, hWnd, (HMENU)RBUTTON12_ID, hInstance, NULL);
   CreateWindow(L"button", L"Blue",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   10, 60, 100, 30, hWnd, (HMENU)RBUTTON13_ID, hInstance, NULL);

   CreateWindow(L"button", L"Rhombus",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   150, 10, 120, 30, hWnd, (HMENU)RBUTTON21_ID, hInstance, NULL);
   CreateWindow(L"button", L"Square",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   150, 35, 120, 30, hWnd, (HMENU)RBUTTON22_ID, hInstance, NULL);
   CreateWindow(L"button", L"Circle",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   150, 60, 120, 30, hWnd, (HMENU)RBUTTON23_ID, hInstance, NULL);
   CreateWindow(L"button", L"Star!",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   150, 85, 120, 30, hWnd, (HMENU)RBUTTON24_ID, hInstance, NULL);

   CheckRadioButton(hWnd, RBUTTON11_ID, RBUTTON13_ID, RBUTTON11_ID);
   CheckRadioButton(hWnd, RBUTTON21_ID, RBUTTON24_ID, RBUTTON21_ID);

   CreateWindow(L"button", L"Draw",
	   WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
	   80, 170, 150, 40, hWnd, (HMENU)CHECKBOX_ID, hInstance, NULL);

   WM_DRAW = RegisterWindowMessage((LPCWSTR)DRAW_MESSAGE);
   WM_COLOR = RegisterWindowMessage((LPCWSTR)COLOR_MESSAGE);
   WM_SHAPE = RegisterWindowMessage((LPCWSTR)SHAPE_MESSAGE);

   SecondWindow = FindWindow(NULL, L"Lab10.2");

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
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:

		if (wmId >= RBUTTON11_ID && wmId <= RBUTTON13_ID) {
			SendMessage(SecondWindow, WM_COLOR, 0, wmId - RBUTTON11_ID);

			CheckRadioButton(hWnd, RBUTTON11_ID, RBUTTON13_ID, wmId);
		}
		else if (wmId >= RBUTTON21_ID && wmId <= RBUTTON24_ID) {
			SendMessage(SecondWindow, WM_SHAPE, 0, wmId - RBUTTON21_ID);

			CheckRadioButton(hWnd, RBUTTON21_ID, RBUTTON24_ID, wmId);
		}
		else switch (wmId)
		{
		case CHECKBOX_ID:
			{
				SendMessage(SecondWindow, WM_DRAW, 0, 0);

				HWND hWndCheck = GetDlgItem(hWnd, CHECKBOX_ID);
				LRESULT state = SendMessage(hWndCheck, BM_GETCHECK, 0, 0);
				// checking state of the checkbox
				if (state == BST_CHECKED)
				{
					SendMessage(SecondWindow, WM_DRAW, 0, 1);
				}
				if (state == BST_UNCHECKED)
				{
					SendMessage(SecondWindow, WM_DRAW, 0, 0);
				}
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
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
