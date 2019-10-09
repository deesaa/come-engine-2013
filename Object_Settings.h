#define ID_OSSTATIC1 11001

class object_settings
{
private:
	HWND OSStatic;
public:
	void initObjectSettings(HWND mainWindow, HINSTANCE hInstance)
	{
		OSStatic = CreateWindow(L"static",  NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, 
			15, 450, 190, 350, mainWindow, (HMENU)ID_OSSTATIC1, hInstance, NULL);
	}
};