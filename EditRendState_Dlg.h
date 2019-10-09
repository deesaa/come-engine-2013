#define ID_ESSBUTTON1   31001
#define ID_ESSBUTTON2   31002
#define ID_ESSCOMBOBOX1 32001
#define ID_ESSCOMBOBOX2 32002

struct rendState
{
	std::wstring strRendStateType;
	std::wstring strRendStateValue[32];
	D3DRENDERSTATETYPE rendStateType;
	DWORD rendStateValue[32];
	DWORD numValues;

	rendState(){}
	rendState(std::wstring strType, D3DRENDERSTATETYPE type, std::vector<std::wstring> strValues, DWORD values[], DWORD size)
	{
		strRendStateType = strType;
		rendStateType = type;
		for(DWORD counter(0); counter != size;)
		{
			strRendStateValue[counter] = strValues[counter];
			rendStateValue[counter]	   = values[counter];
			counter += 1;
			numValues = counter;
		}
	}
};

class rendStateEditor_class
{
private:
	HWND mainWindow;
	HINSTANCE hInstance;

	std::vector<std::wstring> strValues;
	std::wstring strType;

	DWORD RSEComboboxID;
	DWORD RSEComboboxStartID;
	dlgWnd* RSEWindow;
	DWORD numInitedTypes;
	rendState* rendTypes[64];

	HWND RSSButton1, RSSButton2;
//	HWND ComboBox;
public:

	void initRendTypes()
	{
		rendTypes[numInitedTypes] = new rendState;
		strType = L"RS_FILLMODE";
		strValues.clear();
		strValues.push_back(L"FILL_WIREFRAME");
		strValues.push_back(L"FILL_POINT");
		strValues.push_back(L"FILL_SOLID");
		{
			DWORD values[] = {D3DFILL_WIREFRAME, D3DFILL_POINT, D3DFILL_SOLID};
			*rendTypes[numInitedTypes] = rendState(strType, D3DRS_FILLMODE, strValues, values, strValues.size());
		}
		numInitedTypes += 1;

		rendTypes[numInitedTypes] = new rendState;
		strType.clear();
		strType = L"RS_CULLMODE";
		strValues.clear();
		strValues.push_back(L"CULL_CW");
		strValues.push_back(L"CULL_NONE");
		strValues.push_back(L"CULL_CCW");
		{
			DWORD values[] = {D3DCULL_CW, D3DCULL_NONE, D3DCULL_CCW};
			*rendTypes[numInitedTypes] = rendState(strType, D3DRS_CULLMODE, strValues, values, strValues.size());
		}
		numInitedTypes += 1;

		rendTypes[numInitedTypes] = new rendState;
		strType.clear();
		strType = L"RS_LIGHTING";
		strValues.clear();
		strValues.push_back(L"LIGHTING_TRUE");
		strValues.push_back(L"LIGHTING_FALSE");
		{
			DWORD values[] = {TRUE, FALSE};
			*rendTypes[numInitedTypes] = rendState(strType, D3DRS_LIGHTING, strValues, values, strValues.size());
		}
		numInitedTypes += 1;
	}

	void initRendStateEditor(HWND bMainWindow, HINSTANCE bhInstance)
	{
		mainWindow = bMainWindow;
		hInstance  = bhInstance;
		numInitedTypes = 0;
		RSEComboboxStartID = 32001;
		RSEComboboxID	   = 32001;

		RSSButton1 = CreateWindow(L"button", L"Global Render State", WS_CHILD|WS_VISIBLE, 
			14, 802, 191, 20, mainWindow, (HMENU)ID_ESSBUTTON1, hInstance, NULL);

		RSSButton2 = CreateWindow(L"button", L"Edit Render State", WS_CHILD|WS_BORDER|WS_VISIBLE, 
			25, 480, 170, 23, mainWindow, (HMENU)ID_ESSBUTTON2, hInstance, NULL);

		this->initRendTypes();

		RSEWindow = new dlgWnd;
		RSEWindow->initDlgWnd(mainWindow, (DLGPROC)RSSDlgWndProc, hInstance);
		RSEWindow->dlgTemplate(WS_SIZEBOX|WS_POPUP|WS_CHILD|WS_OVERLAPPEDWINDOW,
			109, 50, 410, 400, "Render State");

		int x = 5;
		for(DWORD counter(0); counter != numInitedTypes;)
		{
			RSEWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, 
				5, x, 180, 80, RSEComboboxID, "combobox", "");
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
			SendMessage(RSEWindow->getItemHWND(RSEComboboxStartID + indent), CB_ADDSTRING, NULL, (LPARAM)(LPCTSTR)rendTypes[counter]->strRendStateType.c_str());
			for(DWORD counter_2(0); counter_2 != rendTypes[counter]->numValues;)
			{
				SendMessage(RSEWindow->getItemHWND(RSEComboboxStartID + indent + 1), CB_ADDSTRING, NULL, (LPARAM)(LPCTSTR)rendTypes[counter]->strRendStateValue[counter_2].c_str());
				counter_2 += 1;
			}
			indent += 2;
			counter += 1;
		}
	}

	void showRSSettingsWnd()
	{	RSEWindow->showDlgWindow();}

	~rendStateEditor_class()
	{
		for(;numInitedTypes != 0;)
		{
			numInitedTypes -= 1;
			delete rendTypes[numInitedTypes];
		}
		delete RSEWindow;
	}
};