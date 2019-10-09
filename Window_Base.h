#define ID_BUTTON1  1001
#define ID_BUTTON2  1002
#define ID_BUTTON3  1003

#define ID_STATIC1  2001

#define ID_LISTBOX1 3001

#define ID_EDIT1    4001

class window_class
{
private:
	HWND mainWindow, OCButton, COButton, directXWindow;
	HWND objectsList;
	HWND OSButton1, OSEdit1;
	LPCTSTR mainWindowName, OCButtonName;
public:
	void initWindows(LPCTSTR windowName, LPCTSTR className, HINSTANCE hInstance, WNDPROC proc)
	{
		RegWndClass(proc, hInstance, className);

		mainWindow = CreateWindow(className, windowName, WS_OVERLAPPEDWINDOW|WS_VISIBLE, 
			0, 0, Width, Height, NULL, NULL, hInstance, NULL);

		OCButton   = CreateWindow(L"button", L"Object Creator", WS_CHILD|WS_VISIBLE|BS_CENTER, 
			12, 100, 190, 35, mainWindow, (HMENU)ID_BUTTON1, hInstance, NULL);

		COButton   = CreateWindow(L"button", L"Create Object", WS_CHILD|BS_CENTER, 
			102, 135, 100, 17, mainWindow, (HMENU)ID_BUTTON2, hInstance, NULL);

		directXWindow = CreateWindow(L"static", NULL, WS_CHILD|WS_BORDER, 
			217, 100, DirectXWidth, DirectXHeight, mainWindow, (HMENU)ID_STATIC1, hInstance, NULL);

		objectsList = CreateWindow(L"listbox", NULL, WS_CHILD|LBS_STANDARD|WS_VISIBLE, 
			1360, 100, 200, 705, mainWindow, (HMENU)ID_LISTBOX1, hInstance, NULL);

		OSEdit1 = CreateWindow(L"edit", NULL, WS_CHILD|WS_BORDER|WS_VISIBLE|ES_LEFT|ES_AUTOHSCROLL, 
			35, 403, 150, 20, mainWindow, (HMENU)ID_EDIT1, hInstance, NULL);

		OSButton1 = CreateWindow(L"button", L"Set", WS_CHILD|WS_VISIBLE, 
			92, 424, 40, 20, mainWindow, (HMENU)ID_BUTTON3, hInstance, NULL);
	}

	UINT takeObjectFromList()
	{
		return ((UINT)SendMessage(objectsList, LB_GETCURSEL, 0, 0L) + 1);
	}

	HWND getWindowHandle()
	{
		return directXWindow;
	}
	HWND getCOButtonHandle()
	{
		return COButton;
	}
	HWND getObjectsList()
	{
		return objectsList;
	}
	HWND getNameEditor()
	{
		return OSEdit1;
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