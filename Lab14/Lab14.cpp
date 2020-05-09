#include "stdafx.h"
#include "lab14.h"
#include <strsafe.h>

#define MAX_LOADSTRING 100

HINSTANCE hInst;							
TCHAR szTitle[MAX_LOADSTRING];				
TCHAR szWindowClass[MAX_LOADSTRING];		

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND hEdit, hButtonSearch, hListBox;

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
	LoadString(hInstance, IDC_lab14, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_lab14));

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
	HBRUSH hBrush = CreateSolidBrush(RGB(216, 158, 216));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_lab14));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = hBrush;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_lab14);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

#define EDIT_ID 1001
#define BUTTON_SEARCH_ID 1002
#define LISTBOX_ID 1003
int x = 1000;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   hInst = hInstance; 
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1000, 550, NULL, NULL, hInstance, NULL);
   hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", NULL, WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
	   40, 40, 800, 20, hWnd, (HMENU)EDIT_ID, hInst, NULL);
   hButtonSearch = CreateWindow(L"button", L"Find", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
	   860, 35, 80, 30, hWnd, (HMENU)BUTTON_SEARCH_ID, hInst, NULL);
   hListBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"listbox", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL,
	   40, 80, 900, 400, hWnd, (HMENU)LISTBOX_ID, hInst, NULL);

   if (!hWnd) return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}


void SearchRecursion(HKEY hKeyRoot, LPTSTR lpSubKey, TCHAR* s)
{
	if (--x <= 0) return;
	HKEY hKey;
	FILETIME fileTimeWrite;
	DWORD dwSize;
	TCHAR szName[MAX_PATH];
	LPTSTR endStr;
	LONG result;

	result = RegOpenKeyEx(hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);
	if (result != ERROR_SUCCESS) return;

	if (wcsstr(_wcslwr(lpSubKey), _wcslwr(s)) != NULL)
		SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)lpSubKey);

	endStr = lpSubKey + lstrlen(lpSubKey);
	if (*(endStr - 1) != TEXT('\\'))
	{
		*endStr = TEXT('\\');
		endStr++;
		*endStr = TEXT('\0');
	}
	dwSize = MAX_PATH;
	result = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
		NULL, NULL, &fileTimeWrite);

	if (result == ERROR_SUCCESS)
	{
		int index = 0;
		do {
			index++;
			StringCchCopy(endStr, MAX_PATH * 2, szName);
			SearchRecursion(hKeyRoot, lpSubKey, s);
			if (x <= 0) return;
			dwSize = MAX_PATH;
			result = RegEnumKeyEx(hKey, index, szName, &dwSize, NULL,
				NULL, NULL, &fileTimeWrite);
		} while (result == ERROR_SUCCESS);
	}
	endStr--;
	*endStr = TEXT('\0');
	RegCloseKey(hKey);
}

void SearchFunc(TCHAR* s) {
	SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
	LPTSTR lpSubKey = TEXT("System");
	TCHAR szSearchKey[MAX_PATH * 2];
	StringCchCopy(szSearchKey, MAX_PATH * 2, lpSubKey);
	x = 1000;
	SearchRecursion(HKEY_LOCAL_MACHINE, szSearchKey, s);
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
		switch (wmId)
		{
		case BUTTON_SEARCH_ID:
			{
				int len;
				len = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
				TCHAR buffer[256];
				SendMessage(hEdit, WM_GETTEXT, len + 1, (LPARAM)buffer);
				SearchFunc(buffer);
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
