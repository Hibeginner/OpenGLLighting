// OpenGLForWin32.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "OpenGLLighting.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HDC g_HDC;
HWND g_HWND;
bool exiting = false;
bool fullscreen = false;

long windowWidth = 800;
long windowHeight = 600;

CGfxOpenGL *g_glRender = NULL;
HiResTimer *g_hiResTimer = NULL;

void SetupPixelFormat(HDC hDC) {//�������ظ�ʽ������ʹ�õ����ظ�ʽ��opengl���豸����

	int nPixelFormat;
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0,0,0,0,0,0,
		0,
		0,
		0,
		0,0,0,0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};

	/*
	* �ú����Ƚϴ����������ظ�ʽ������OpenGL֧�ֵ����ظ�ʽ������һ�����ƥ������ظ�ʽ������
	* ������ֵ�ɴ���SetPixelFormatΪDC�������ظ�ʽ������ֵΪ0��ʾʧ�ܡ�
	* �ڱȽ����ظ�ʽʱ��ƥ�����ȼ�˳��Ϊ���ظ�ʽ�����ӽṹ�е���������
	* dwFlags��>cColorBits��>cAlphaBits��>cAccumBits
	*��>cDepthBits��>cStencilBits��>cAuxBuffers��>iLayerType
	* Ӳ��֧�ֵ����ظ�ʽ���ȡ�
	*/
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	SetPixelFormat(hDC, nPixelFormat, &pfd);
}

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	g_glRender = new CGfxOpenGL();
	g_hiResTimer = new HiResTimer();


	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_OPENGLLIGHTING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OPENGLLIGHTING));

	MSG msg;


	// Main message loop:
	/*while (GetMessage(&msg, nullptr, 0, 0))
	{
	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	}
	}*/
	g_glRender->Init();
	g_hiResTimer->Init();
	while (!exiting) {
		g_glRender->Prepare(g_hiResTimer->GetElapsedSecond(1));
		g_glRender->Render();
		SwapBuffers(g_HDC);//�ѸղŻ�����̨�ģ�������ǰ̨


		while (PeekMessage(&msg, g_HWND, NULL, NULL, PM_NOREMOVE)) {
			if (!GetMessage(&msg, NULL, 0, 0)) {
				exiting = true;
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	delete g_glRender;
	if (fullscreen) {
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);
	}
	else
	{

	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_OPENGLLIGHTING));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OPENGLBASE);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	DWORD dwStyle;
	DWORD dwExStyle;
	RECT windowRect;

	hInst = hInstance; // Store instance handle in our global variable

	windowRect.left = (long)0;
	windowRect.right = (long)windowWidth;
	windowRect.top = (long)0;
	windowRect.bottom = (long)windowHeight;

	if (fullscreen) {
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));//�ѽṹ����
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = windowWidth;
		dmScreenSettings.dmPelsHeight = windowHeight;
		dmScreenSettings.dmBitsPerPel = 32;//����RGBA8888 32λ
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSHEIGHT | DM_PELSWIDTH;
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			MessageBox(NULL, _T("Display mode failed"), NULL, MB_OK);
			fullscreen = false;
		}
	}
	if (fullscreen) {
		dwExStyle = WS_EX_APPWINDOW;//��չ�����ͨapp����
		dwStyle = WS_POPUP;//��������
		ShowCursor(false);
	}
	else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;//��ͨapp����+�߿�
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&windowRect, dwStyle, false, dwExStyle);//false��Ҫ�˵�

															   /*HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
															   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);*/
	hWnd = CreateWindowEx(NULL, szWindowClass, szTitle,
		dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, /*�ü��Ӵ��� �ü��ֵܴ��� Ϊ�˼ӿ���ʾ�ٶ�*/
		0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		NULL, NULL, hInst, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	g_HWND = hWnd;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	static HDC hdc;//�豸������
	static HGLRC hRC;//openGL����������
	int height, width;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);
		g_HDC = hdc;
		SetupPixelFormat(hdc);
		hRC = wglCreateContext(hdc);
		wglMakeCurrent(hdc, hRC);//��������opengl���������ڵ�ǰ��OpenGL������һ�����������ĸ�һ���豸�����Ĺ�����һ���豸�����ĸ�һ�����ڹ�����

		if (glewInit() != GLEW_OK) {
			MessageBox(NULL, _T("GLEW init error!"), 0, 0);
			exit(1);
		}

		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_SIZE://���ڸı�ʱ
		height = HIWORD(lParam);//���ڿ��
		width = LOWORD(lParam);
		//g_glRender->SetupProjection(width, height);
		g_glRender->ResizeScene(width, height);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		GetClientRect(hWnd, &rect);
		DrawText(hdc, TEXT("Hello Windows!"), -1, &rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_KEYDOWN:
		int fwKeys;
		LPARAM keyData;
		fwKeys = (int)wParam;
		keyData = lParam;
		switch (fwKeys) {
		case VK_SPACE:
			g_glRender->UpdateProjection(true);
			break;
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
	case WM_QUIT:
	case WM_DESTROY:
		wglMakeCurrent(g_HDC, NULL);//ȡ������������
		wglDeleteContext(hRC);
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
