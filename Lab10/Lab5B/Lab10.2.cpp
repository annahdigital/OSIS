

#include "stdafx.h"
#include "Lab10.2.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;					
TCHAR szTitle[MAX_LOADSTRING];				
TCHAR szWindowClass[MAX_LOADSTRING];			

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

typedef struct WM_DRAW_data {
	BOOL draw;
	UINT color;
	UINT shape;
};

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
	LoadString(hInstance, IDC_LAB5B, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB5B));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB5B));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LAB5B);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

// messages from the first window
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
	   CW_USEDEFAULT, 0, 400, 350, NULL, NULL, hInstance, NULL);

   WM_DRAW = RegisterWindowMessage((LPCWSTR)DRAW_MESSAGE);
   WM_COLOR = RegisterWindowMessage((LPCWSTR)COLOR_MESSAGE);
   WM_SHAPE = RegisterWindowMessage((LPCWSTR)SHAPE_MESSAGE);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

bool draw = false;
int color = 0, shape = 0;
int drawX = -1, drawY = -1;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	if (message == WM_DRAW) {
		draw = lParam;
	} else
	if (message == WM_COLOR) {
		color = lParam;
		InvalidateRect(hWnd, NULL, true);
	}
	else
	if (message == WM_SHAPE) {
		shape = lParam;
		InvalidateRect(hWnd, NULL, true);
	}
	else
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		drawX = LOWORD(lParam);
		drawY = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		if (draw && drawX != -1) {
			HBRUSH brush = NULL;
			switch (color)
			{
			case 0:
				brush = CreateSolidBrush(RGB(255, 0, 0));
				break;
			case 1:
				brush = CreateSolidBrush(RGB(0, 255, 0));
				break;
			case 2:
				brush = CreateSolidBrush(RGB(0, 0, 255));
				break;
			default:
				break;
			}
			SelectObject(hdc, brush);

			switch (shape)
			{
			case 0:
				{
				// drawing the rombus
				POINT p[4] = { drawX + 20, drawY + 0, drawX + 40, drawY + 20,
						drawX + 20, drawY + 40, drawX + 0, drawY + 20 };
					Polygon(hdc, p, 4);
				}
				break;
			case 1:
				// drawing the square
				Rectangle(hdc, drawX, drawY, drawX + 40, drawY + 40);
				break;
			case 2:
				// drawing the circle
				Ellipse(hdc, drawX, drawY, drawX + 40, drawY + 40);
				break;
			case 3:
				{
				// drawing the star!
				POINT p[10] = { 20, 15, 25, 8, 30, 15, 40, 18,
								32, 23, 35, 30, 25, 25, 15, 30,
								18, 22, 10, 18 };
					for (int i = 0; i < 10; i++) {
						p[i].x += drawX;
						p[i].y += drawY;
					}
					Polygon(hdc, p, 10);
				}
				break;
			default:
				break;
			}
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
