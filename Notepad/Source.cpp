#include <windows.h>
#include <shobjidl.h>
#include <commctrl.h>
#include <commdlg.h>
#ifndef UNICODE
#define UNICODE
#endif 
HWND g_hEdit;
HFONT g_hFont;
HWND g_hwnd;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR  lpszArgument, int nCmdShow) {
	const wchar_t CLASS_NAME[] = L"Note Pad";
	WNDCLASSEX wc;
	HWND hwnd;
	MSG msg;

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbSize = sizeof(WNDCLASSEX);

	//default icon and mouse-pointer
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(102));
	wc.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(101));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = MAKEINTRESOURCE(202);
	wc.cbClsExtra = 0;                      // No extra bytes after the window class 
	wc.cbWndExtra = 0;                      // structure or the window instance 
	// default background color
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	if (!RegisterClassEx(&wc)) {
		return 0;
	}

	hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Note Pad",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		0,
		hInstance,
		NULL
	);
	if (!hwnd) {
		MessageBox(NULL, L"Cant create the window...\nExit Now", L"ERROR", MB_OK | MB_ICONERROR);
		return 0;
	}
	g_hwnd = hwnd;
	ShowWindow(hwnd, nCmdShow);
	
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(203));

	bool done = false;
	while (!done) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				done = true;
			}
			else if (!TranslateAccelerator(g_hwnd, hAccel, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	// The program return-value is 0 - The value that PostQuitMessage() gave
	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		if (MessageBox(hwnd, L"Do you want to Quit", L"Note Pad", MB_OKCANCEL) == IDOK) {
			DestroyWindow(hwnd);
		}
		return 0;
	case WM_CREATE:
	{
		HWND hEdit;
		HFONT hFont;
		//For Edit Control...
		hEdit = CreateWindowEx(
			WS_EX_CLIENTEDGE,                   /* Extended p ossibilites for variation */
			L"EDIT",         /* Classname */
			L"",       /* Title Text */
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL, /* default window */
			1,       /* Windows decides the position */
			1,       /* where the window ends up on the screen */
			100,                
			100,                
			hwnd,        /* The window is a child-window to hwnd */
			(HMENU)201,                
			GetModuleHandle(NULL),       
			NULL                 
		);
		if (hEdit == NULL)
		{
			MessageBox(g_hwnd, L"Could not Create Edit control!!", L"Error", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
		}

		hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(FALSE, 0));

		g_hEdit = hEdit;
		g_hFont = hFont;

		RECT rcClient;
		GetClientRect(g_hwnd, &rcClient);
		SetWindowPos(g_hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);

	}

	break;
	case WM_SIZE:
	{
		RECT rcClient;
		GetClientRect(g_hwnd, &rcClient);
		SetWindowPos(g_hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
	}
	break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	
}