#define ID_ESSBUTTON1   31001
#define ID_ESSBUTTON2   31002
#define ID_ESSCOMBOBOX1 32001
#define ID_ESSCOMBOBOX2 32002

struct rendState
{
	std::wstring rendStateType;
	std::wstring rendStateValue[32];

	rendState(){}
	rendState(std::wstring type, std::vector<std::wstring> values, DWORD size)
	{
		rendStateType = type;
		for(DWORD counter(0); counter != size;)
		{
			rendStateValue[counter] = values[counter];
			counter += 1;
		}
	}
};

class rendStateEditor_class
{
private:
	HWND mainWindow;
	HINSTANCE hInstance;

	dlgWnd* RSSWindow;
	DWORD numInitedTypes;
	rendState* rendTypes[64];

	HWND RSSButton1, RSSButton2;
//	HWND ComboBox;
public:

	void initRendTypes()
	{
		rendTypes[numInitedTypes] = new rendState;
		std::vector<std::wstring> values;
		values.push_back(L"FILL_WIREFRAME");
		values.push_back(L"FILL_POINT");
		values.push_back(L"FILL_SOLID");
		std::wstring type     = L"RS_FILLMODE";
		*rendTypes[numInitedTypes] = rendState(type, values, values.size());
		numInitedTypes += 1;
	}

	void initRendStateEditor(HWND bMainWindow, HINSTANCE bhInstance)
	{
		mainWindow = bMainWindow;
		hInstance  = bhInstance;
		numInitedTypes = 0;

		RSSButton1 = CreateWindow(L"button", L"Global Render State", WS_CHILD|WS_VISIBLE, 
			14, 802, 191, 20, mainWindow, (HMENU)ID_ESSBUTTON1, hInstance, NULL);

		RSSButton2 = CreateWindow(L"button", L"Edit Render State", WS_CHILD|WS_BORDER|WS_VISIBLE, 
			25, 480, 170, 23, mainWindow, (HMENU)ID_ESSBUTTON2, hInstance, NULL);

		RSSWindow = new dlgWnd;
		RSSWindow->initDlgWnd(mainWindow, (DLGPROC)RSSDlgWndProc, hInstance);
		RSSWindow->dlgTemplate(WS_SIZEBOX|WS_POPUP|WS_CHILD|WS_OVERLAPPEDWINDOW,
			109, 50, 410, 400, "Render State");

		RSSWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|CBS_SORT|CBS_DROPDOWNLIST, 
			5, 5, 180, 80, ID_ESSCOMBOBOX1, "combobox", "");
		RSSWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|CBS_SORT|CBS_DROPDOWNLIST, 
			96, 5, 180, 80, ID_ESSCOMBOBOX2, "combobox", "");

		RSSWindow->createModelessDlgWindow();

		this->initRendTypes();

		SendMessage(RSSWindow->getItemHWND(ID_ESSCOMBOBOX1), CB_ADDSTRING, NULL, (LPARAM)(LPCTSTR)rendTypes[0]->rendStateType.c_str());
		//endMessage(RSSWindow->getItemHWND(ID_ESSCOMBOBOX1), CB_ADDSTRING, NULL, (LPARAM)L"RS_CULLMODE");

		SendMessage(RSSWindow->getItemHWND(ID_ESSCOMBOBOX2), CB_ADDSTRING, NULL, (LPARAM)(LPCTSTR)rendTypes[0]->rendStateValue[0].c_str());
		SendMessage(RSSWindow->getItemHWND(ID_ESSCOMBOBOX2), CB_ADDSTRING, NULL, (LPARAM)(LPCTSTR)rendTypes[0]->rendStateValue[1].c_str());
		SendMessage(RSSWindow->getItemHWND(ID_ESSCOMBOBOX2), CB_ADDSTRING, NULL, (LPARAM)(LPCTSTR)rendTypes[0]->rendStateValue[2].c_str());
	}

	void showRSSettingsWnd()
	{	RSSWindow->showDlgWindow();}

	~rendStateEditor_class()
	{
		for(;numInitedTypes != 0;)
		{
			numInitedTypes -= 1;
			delete rendTypes[numInitedTypes];
		}
		delete RSSWindow;
	}
};