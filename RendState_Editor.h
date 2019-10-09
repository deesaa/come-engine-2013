#define ID_ESSBUTTON1   30001
#define ID_ESSBUTTON2   30002
#define ID_ESSBUTTON3   30003
#define ID_ESSBUTTON4   30004

class rendStateEditor_class
{
private:
	HWND mainWindow;
	HINSTANCE hInstance;
	rendStateTypes_class* RSTypes;
	rendState_class* rendState;
	DWORD numInitedTypes;

	WORD RSEComboboxID;
	WORD RSEComboboxStartID;
	dlgWnd* RSEWindow;

	HWND RSSButton1;

	ID3DXConstantTable* TransformConstantTable;
	ID3DXBuffer* strShader;
	ID3DXBuffer* errorBuffer;

	bool isShaderMode;
	bool isShaderReady;

public:

	void initRendStateEditor(HWND bMainWindow, HINSTANCE bhInstance, rendStateTypes_class* bRSTypes)
	{
		mainWindow = bMainWindow;
		hInstance  = bhInstance;
		RSTypes    = bRSTypes;
		TransformConstantTable = NULL;
		strShader = errorBuffer = NULL;
		numInitedTypes = RSTypes->numInitedTypes;
		RSEComboboxStartID = 32001;
		RSEComboboxID	   = 32001;
		isShaderMode = isShaderReady = FALSE;

		RSSButton1 = CreateWindow(L"button", L"Edit Render State", WS_CHILD|WS_BORDER, 
			25, int(Height * 0.5) + 24, 170, 23, mainWindow, (HMENU)ID_ESSBUTTON1, hInstance, NULL);

		RSEWindow = new dlgWnd;
		RSEWindow->initDlgWnd(mainWindow, (DLGPROC)RSEDlgWndProc, hInstance);
		RSEWindow->dlgTemplate(WS_SIZEBOX|WS_POPUP|WS_CHILD|WS_OVERLAPPEDWINDOW,
			109, 50, 410, 400, "Render State");

		int x = 15;
		RSEWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, 5, 5, 75, 20, ID_ESSBUTTON3, "button", "Shader");

		RSEWindow->dlgItemTemplate(WS_CHILD|BS_CENTER, 60, 4, 180, 22, ID_ESSBUTTON4, "button", "Load Shader");

		RSEWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE, 187, 5, 30, 380, ID_ESSBUTTON2, "button", "Set");
		for(DWORD counter(0); counter != numInitedTypes;)
		{
			RSEWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|SS_CENTER|WS_BORDER, 
				5, x, 180, 25, RSEComboboxID, "static", "");
			RSEComboboxID += 1;

			RSEWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, 
				96, x, 180, numInitedTypes*30, RSEComboboxID, "combobox", "");
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
		isShaderMode = rendState->isShader;
		this->showMode();
		DWORD indent(1);
		for(DWORD counter(0); counter != numInitedTypes;)
		{
			SendMessage(RSEWindow->getItemHWND(RSEComboboxStartID + indent), CB_SETCURSEL, rendState->vecRendStateValue.at(counter), 0L);
			indent += 2;
			counter += 1;
		}
	}


	void loadShader()
	{
		HRESULT hr;
		WCHAR fileDir[260];
		WCHAR szFile[260] = L"Shader_";  
		WCHAR szFilter[260] = L".txt\0*.*\0";  
		WCHAR szTitle[260] = L"Load Shader";
		OPENFILENAME ofn;

		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.lpstrFile = (LPWSTR)szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.nFilterIndex = 1;
		ofn.nMaxCustFilter = sizeof(szFilter);
		ofn.lpstrFilter = (LPWSTR)szFilter;
		ofn.lpstrTitle = (LPWSTR)szTitle;
		ofn.nMaxFileTitle = sizeof(szTitle);
		ofn.lpstrInitialDir = (LPWSTR)fileDir;
		ofn.Flags = OFN_OVERWRITEPROMPT;

		if(GetOpenFileName(&ofn))
		{
			hr = D3DXCompileShaderFromFile(
				(LPWSTR)ofn.lpstrFile,  // им€ файла шейдера
				0,
				0,
				"Main",           // им€ точки входа
				"vs_2_0",         // верси€ шейдеров
				D3DXSHADER_DEBUG, // компил€ци€ дл€ отладки
				&strShader,
				&errorBuffer,
				&TransformConstantTable);

			if(FAILED(hr))
				MessageBox(RSEWindow->getHWND(), L"Can't compile shader", L"Error", MB_OK|MB_HELP);
		}
	}

	void showErrorBuffer()
	{
		if(errorBuffer != NULL)
			MessageBoxA(NULL, (char*)errorBuffer->GetBufferPointer(), "ErrorBuffer", MB_OK);
	}

	void swapMode()
	{
		if(IsWindowVisible(RSEWindow->getItemHWND(RSEComboboxStartID)))
			isShaderMode = TRUE;
		else
			isShaderMode = FALSE;
		this->showMode();
	}

	void setChanges()
	{
		DWORD indent(0);
		DWORD el;

		for(DWORD counter(0); counter != numInitedTypes;)
		{
			el = SendMessage(RSEWindow->getItemHWND(RSEComboboxStartID + indent + 1), CB_GETCURSEL, 0, 0L);
			rendState->vecRendStateValue.at(counter) = el;
			indent += 2;
			counter += 1;
		}

	}

	void showMode()
	{
		if(isShaderMode == FALSE)
		{
			ShowWindow(RSEWindow->getItemHWND(ID_ESSBUTTON4), SW_HIDE);
			SendMessage(RSEWindow->getItemHWND(ID_ESSBUTTON3), BM_SETCHECK, 0, 0L);
			for(DWORD counter(0); counter != numInitedTypes*2;)
			{
				ShowWindow(RSEWindow->getItemHWND(RSEComboboxStartID + counter), SW_NORMAL);
				counter += 1;
			}
		}

		if(isShaderMode == TRUE)
		{
			ShowWindow(RSEWindow->getItemHWND(ID_ESSBUTTON4), SW_NORMAL);
			SendMessage(RSEWindow->getItemHWND(ID_ESSBUTTON3), BM_SETCHECK, 1, 0L);
			for(DWORD counter(0); counter != numInitedTypes*2;)
			{
				ShowWindow(RSEWindow->getItemHWND(RSEComboboxStartID + counter), SW_HIDE);
				counter += 1;
			}
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
		if(TransformConstantTable != NULL)
			TransformConstantTable->Release();
		if(strShader != NULL)
			strShader->Release();
		if(errorBuffer != NULL)
			errorBuffer->Release();
		delete RSEWindow;
	}
};