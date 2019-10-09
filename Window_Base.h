#define ID_BUTTON1 1001
#define ID_STATIC1 2001

class window_class
{
private:
	HWND mainWindow, OCButton, directXWindow;
	LPCTSTR mainWindowName, OCButtonName;
public:
	window_class(LPCTSTR windowName, LPCTSTR className, HINSTANCE hInstance, WNDPROC proc)
	{
		RegWndClass(proc, hInstance, className);

		mainWindow = CreateWindow(className, windowName, WS_OVERLAPPEDWINDOW|WS_VISIBLE, 
			0, 0, Width, Height, NULL, NULL, hInstance, NULL);

		OCButton   = CreateWindow(L"button", L"Create New Object", WS_CHILD|WS_VISIBLE|BS_CENTER, 
			12, 100, 190, 35, mainWindow, (HMENU)ID_BUTTON1, hInstance, NULL);

		directXWindow = CreateWindow(L"static", NULL, WS_CHILD|WS_BORDER, 
			217, 100, DirectXWidth, DirectXHeight, mainWindow, (HMENU)ID_STATIC1, hInstance, NULL);
	}

	HWND getWindowHandle()
	{
		return directXWindow;
	}
};


void RegWndClass(WNDPROC proc, HINSTANCE hInstance, LPCTSTR className)
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = proc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground =
		static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName  = 0;
	wc.lpszClassName = className;

	if(!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass - Failed", 0, 0);
	}
}