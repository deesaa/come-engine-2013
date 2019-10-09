#define ID_ESSBUTTON1   30001
#define ID_ESSBUTTON2   30002

class rendStateEditor_class
{
private:
	HWND mainWindow;
	HINSTANCE hInstance;
	rendStateTypes_class* RSTypes;
	rendState_class* rendState;
	DWORD numInitedTypes;

	DWORD RSEComboboxID;
	DWORD RSEComboboxStartID;
	dlgWnd* RSEWindow;

	HWND RSSButton1;

public:

	void initRendStateEditor(HWND bMainWindow, HINSTANCE bhInstance, rendStateTypes_class* bRSTypes)
	{
		mainWindow = bMainWindow;
		hInstance  = bhInstance;
		RSTypes    = bRSTypes;
		numInitedTypes = RSTypes->numInitedTypes;
		RSEComboboxStartID = 32001;
		RSEComboboxID	   = 32001;

		RSSButton1 = CreateWindow(L"button", L"Edit Render State", WS_CHILD|WS_BORDER, 
			25, Height * 0.5 + 24, 170, 23, mainWindow, (HMENU)ID_ESSBUTTON1, hInstance, NULL);

		RSEWindow = new dlgWnd;
		RSEWindow->initDlgWnd(mainWindow, (DLGPROC)RSEDlgWndProc, hInstance);
		RSEWindow->dlgTemplate(WS_SIZEBOX|WS_POPUP|WS_CHILD|WS_OVERLAPPEDWINDOW,
			109, 50, 410, 400, "Render State");

		int x = 5;
		RSEWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE, 187, 5, 30, 380, ID_ESSBUTTON2, "button", "Set");
		
		for(DWORD counter(0); counter != numInitedTypes;)
		{
			RSEWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|SS_CENTER|WS_BORDER, 
				5, x, 180, 25, RSEComboboxID, "static", "");
			RSEComboboxID += 1;

			RSEWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, 
				96, x, 180, 80, RSEComboboxID, "combobox", "");
			RSEComboboxID += 1;

			x += 15;
			counter += 1;
		}

		RSEWindow->createModelessDlgWindow();

		DWORD indent(0);
		for(DWORD counter(0); counter != numInitedTypes;)
		{
			SendMessage(RSEWindow->getItemHWND(RSEComboboxStartID + indent), WM_SETTEXT, NULL, (LPARAM)(LPCTSTR)RSTypes->rendStateType[counter]->strRendStateType.c_str());
			for(DWORD counter_2(0); counter_2 != RSTypes->rendStateType[counter]->numValues;)
			{
				SendMessage(RSEWindow->getItemHWND(RSEComboboxStartID + indent + 1), CB_ADDSTRING, NULL, (LPARAM)(LPCTSTR)RSTypes->rendStateType[counter]->strRendStateValues[counter_2].c_str());
				counter_2 += 1;
			}
			indent += 2;
			counter += 1;
		}
	}

	void fillRSEditor(rendState_class* bRendState)
	{
		rendState = bRendState;
		DWORD indent(1);
		for(DWORD counter(0); counter != rendState->numElements;)
		{
			SendMessage(RSEWindow->getItemHWND(RSEComboboxStartID + indent), CB_SETCURSEL, *rendState->rendStateValue[counter], 0L);
			indent += 2;
			counter += 1;
		}
	}

	void setChanges()
	{
		DWORD indent(0);
		DWORD el;
		for(DWORD counter(0); counter != numInitedTypes;)
		{
			el = SendMessage(RSEWindow->getItemHWND(RSEComboboxStartID + indent + 1), CB_GETCURSEL, 0, 0L);

			*rendState->rendStateValue[counter] = el;
			indent += 2;
			counter += 1;
		}
	}

	void showRSSettingsButton()
	{	ShowWindow(RSSButton1, SW_NORMAL);}

	void hideRSSettingsButton()
	{	ShowWindow(RSSButton1, SW_HIDE);}

	void showRSSettingsWnd()
	{	RSEWindow->showDlgWindow();}

	~rendStateEditor_class()
	{
		delete RSEWindow;
	}
};