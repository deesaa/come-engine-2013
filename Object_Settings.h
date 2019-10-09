#define ID_OSSTATIC1 11001
#define ID_OSBUTTON1 11001
#define ID_OSBUTTON2 11001

class object_settings
{
private:
	HWND OSStatic, OSButton1, OSButton2;
	HWND OSWindow;
	LPCTSTR OSClassName;
public:
	void initObjectSettings(HWND mainWindow, HINSTANCE hInstance)
	{
		OSClassName = L"C.O.M.E. Engine : ObSettings"; 

		OSStatic = CreateWindow(L"static",  NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, 
			15, 450, 190, 350, mainWindow, (HMENU)ID_OSSTATIC1, hInstance, NULL);

		OSButton1 = CreateWindow(L"button",  L"Edit Object's Material", WS_CHILD|WS_VISIBLE|WS_BORDER, 
			25, 455, 170, 23, mainWindow, (HMENU)ID_OSBUTTON1, hInstance, NULL);

		OSButton1 = CreateWindow(L"button", L"Edit Color Of Light", WS_CHILD|WS_VISIBLE|WS_BORDER, 
			25, 480, 170, 23, mainWindow, (HMENU)ID_OSBUTTON2, hInstance, NULL);

		RegWndClass(OSWndProc, hInstance, OSClassName);

		OSWindow = CreateWindow(OSClassName, L"Object Settings Editor", WS_OVERLAPPEDWINDOW|WS_POPUPWINDOW|WS_VISIBLE|WS_CHILD,
			400, 200, 600, 600, mainWindow, NULL, hInstance, NULL);
	}
};

LRESULT CALLBACK OSWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}