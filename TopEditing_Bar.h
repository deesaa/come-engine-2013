#define ID_TEBGROUPBOX1 22001
#define ID_TEBBUTTON1   21001
#define ID_TEBBUTTON2   21002
#define ID_TEBBUTTON3   21003

#define ID_TEBGROUPBOX2 22002
#define ID_TEBBUTTON4   21004
#define ID_TEBBUTTON5   21005

class topEditingBar_Class
{
private:
	HWND mainWindow;
	HINSTANCE hInstance;

	HWND TEBGroupBox1, TEBGroupBox2;
	HWND TEBButton1, TEBButton2, TEBButton3;
	HWND TEBButton4, TEBButton5;

public:
	void initTEBar(HWND bMainWindow, HINSTANCE bhInstance)
	{
		mainWindow = bMainWindow;
		hInstance  = bhInstance;

		TEBGroupBox1 = CreateWindow(L"button", L"Don't Move Along", WS_CHILD|BS_GROUPBOX, 
			292, 40, 128, 60, mainWindow, (HMENU)ID_TEBGROUPBOX1, hInstance, NULL);

		TEBButton1 = CreateWindow(L"button", L"X", WS_CHILD|BS_PUSHBUTTON|BS_AUTOCHECKBOX, 
			300, 60, 35, 35, mainWindow, (HMENU)ID_TEBBUTTON1, hInstance, NULL);

		TEBButton2 = CreateWindow(L"button", L"Y", WS_CHILD|BS_PUSHBUTTON|BS_AUTOCHECKBOX, 
			338, 60, 35, 35, mainWindow, (HMENU)ID_TEBBUTTON2, hInstance, NULL);

		TEBButton3 = CreateWindow(L"button", L"Z", WS_CHILD|BS_PUSHBUTTON|BS_AUTOCHECKBOX, 
			376, 60, 35, 35, mainWindow, (HMENU)ID_TEBBUTTON3, hInstance, NULL);

		TEBGroupBox2 = CreateWindow(L"button", L"Space", WS_CHILD|BS_GROUPBOX, 
			425, 40, 131, 60, mainWindow, (HMENU)ID_TEBGROUPBOX2, hInstance, NULL);

		TEBButton4 = CreateWindow(L"button", L"View", WS_CHILD|BS_PUSHBUTTON|BS_AUTORADIOBUTTON|BS_CENTER, 
			430, 65, 55, 25, mainWindow, (HMENU)ID_TEBBUTTON4, hInstance, NULL);

		TEBButton5 = CreateWindow(L"button", L"World", WS_CHILD|BS_PUSHBUTTON|BS_AUTORADIOBUTTON|BS_CENTER, 
			490, 65, 60, 25, mainWindow, (HMENU)ID_TEBBUTTON5, hInstance, NULL);
	}

	void getAxis(bool* axisX, bool* axisY, bool* axisZ)
	{
		if(SendMessage(TEBButton1, BM_GETCHECK, 0, 0L))
			*axisX = TRUE;
		if(SendMessage(TEBButton2, BM_GETCHECK, 0, 0L))
			*axisY = TRUE;
		if(SendMessage(TEBButton3, BM_GETCHECK, 0, 0L))
			*axisZ = TRUE;
	}

	void showBar()
	{
		ShowWindow(TEBGroupBox1, SW_NORMAL);
		ShowWindow(TEBButton1, SW_NORMAL);
		ShowWindow(TEBButton2, SW_NORMAL);
		ShowWindow(TEBButton3, SW_NORMAL);
		ShowWindow(TEBGroupBox2, SW_NORMAL);
		ShowWindow(TEBButton4, SW_NORMAL);
		ShowWindow(TEBButton5, SW_NORMAL);
	}

	void hideBar()
	{
		ShowWindow(TEBGroupBox1, SW_HIDE);
		ShowWindow(TEBButton1, SW_HIDE);
		ShowWindow(TEBButton2, SW_HIDE);
		ShowWindow(TEBButton3, SW_HIDE);
		ShowWindow(TEBGroupBox2, SW_HIDE);
		ShowWindow(TEBButton4, SW_HIDE);
		ShowWindow(TEBButton5, SW_HIDE);
	}
};