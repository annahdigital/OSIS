// Lab7.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Lab7.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hAdd;										// value button
HWND hClear;									// clear button
HWND hRight;									//  to the right button
HWND hDelete;									// delete button
HWND hListBox1;									// listbox 1
HWND hListBox2;									// listbox 2
HWND hEdit;										// edit button

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB7, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB7));
    MSG msg;
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)	 //Registers the window class.
{
	HBRUSH hBrush = CreateSolidBrush(RGB(216, 158, 216));;
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB7));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = hBrush;
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB7);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)		// Saves instance handle and creates main window
{
	//In this function, we save the instance handle in a global variable and
//        create and display the main program window.
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

int x = 50, x_width, x_end, x_middle;
TCHAR input_text[1024];

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) //Processes messages for the main window.
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT)& rect, 2);
	x_width = (rect.right + rect.left - 40 - 100) / 4;
	x_middle = (rect.right - 120) / 2;
	x_end = rect.right - 400;
	
    switch (message)
    {
	case WM_CREATE:                      
	{
		hAdd = CreateWindow(L"BUTTON", L"Add", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			x, 450, x_width, 40, hWnd, HMENU(IDM_ADD), hInst, NULL);

		hClear = CreateWindow(L"BUTTON", L"Clear", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			x + x_width + 10, 450, x_width, 40, hWnd, HMENU(IDM_CLEAR), hInst, NULL);

		hRight = CreateWindow(L"BUTTON", L"To right", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			x + 2*x_width + 20, 450, x_width, 40, hWnd, HMENU(IDM_RIGHT), hInst, NULL);

		hDelete = CreateWindow(L"BUTTON", L"Delete", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			x + 3 * x_width + 30, 450, x_width, 40, hWnd, HMENU(IDM_DELETE), hInst, NULL);

		hListBox1 = CreateWindow(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD,
			50, 10, x_middle, 400, hWnd, HMENU(IDC_LISTBOX1), hInst, NULL);

		hListBox2 = CreateWindow(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD,
			x_middle + 60, 10, x_middle, 400, hWnd, HMENU(IDC_LISTBOX2), hInst, NULL);

		hEdit = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,  
			200, 410, x_end, 20, hWnd, HMENU(IDM_EDIT), hInst, NULL);
		break;
	}
    case WM_COMMAND:			//process the application menu
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case IDM_ADD:
				GetWindowText(hEdit, input_text, _countof(input_text));
				SetWindowText(hEdit, nullptr);
				if (input_text[0] != _T('\0') && SendMessage(hListBox1, LB_FINDSTRINGEXACT, -1, (LPARAM)input_text) == LB_ERR)
				SendMessage(hListBox1, LB_ADDSTRING, NULL, (LPARAM)input_text);
				break;
			case IDM_DELETE:
				SendMessage(hListBox1, LB_DELETESTRING, 
					SendMessage(hListBox1, LB_GETCURSEL, NULL, NULL), NULL);
				SendMessage(hListBox2, LB_DELETESTRING, 
					SendMessage(hListBox2, LB_GETCURSEL, NULL, NULL), NULL);
				break;
			case IDM_RIGHT:
				SendMessage(hListBox1, LB_GETTEXT,
					SendMessage(hListBox1, LB_GETCURSEL, NULL, NULL), (LPARAM)input_text);
				if (SendMessage(hListBox2, LB_FINDSTRINGEXACT, -1, (LPARAM)input_text) == LB_ERR)
					SendMessage(hListBox2, LB_ADDSTRING, NULL, (LPARAM)input_text);
				break;
			case IDM_CLEAR:
				while (SendMessage(hListBox1, LB_GETCOUNT, NULL, NULL) > 0)
					SendMessage(hListBox1, LB_DELETESTRING, 0, NULL);
				while (SendMessage(hListBox2, LB_GETCOUNT, NULL, NULL) > 0)
					SendMessage(hListBox2, LB_DELETESTRING, 0, NULL);
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:		//Paint the main window
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:		// post a quit message and return
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

