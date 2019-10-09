#define ID_OSSTATIC1 11001
#define ID_OSBUTTON1 12001
#define ID_OSBUTTON2 12002

#define ID_EOMSTATIC1 13001
#define ID_EOMEDIT1	  14001
#define ID_EOMEDIT2	  14002
#define ID_EOMEDIT3	  14003
#define ID_EOMEDIT4	  14004

#define ID_EOMSTATIC2 13002
#define ID_EOMEDIT5	  14005
#define ID_EOMEDIT6	  14006
#define ID_EOMEDIT7	  14007
#define ID_EOMEDIT8	  14008

#define ID_EOMSTATIC3 13003
#define ID_EOMEDIT9	  14009
#define ID_EOMEDIT10  14010
#define ID_EOMEDIT11  14011
#define ID_EOMEDIT12  14012

#define ID_EOMBUTTON1 15001

#define GET_OSEOBJECTMATERIALB 1
#define GET_OSELIGHTCOLORB	   2

#define ID_DLGSTATIC1 11002
#define ID_DLGSTATIC2 11003

#define BUFFERSIZE 6
WCHAR textBuffer[BUFFERSIZE];

class object_settings
{
private:
	dlgWnd* OSDWindow;

	HWND OSStatic, OSButton2, OSButton1;

	LPCTSTR OSClassName;

	D3DMATERIAL9* material;

public:
	void initObjectSettings(HWND mainWindow, HINSTANCE hInstance)
	{
		OSClassName = L"C.O.M.E. Engine : ObSettings"; 

		OSStatic = CreateWindow(L"static",  NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, 
			15, 450, 190, 350, mainWindow, (HMENU)ID_OSSTATIC1, hInstance, NULL);

		OSButton1 = CreateWindow(L"button",  L"Edit Object's Material", WS_CHILD|WS_BORDER, 
			25, 455, 170, 23, mainWindow, (HMENU)ID_OSBUTTON1, hInstance, NULL);

		OSButton2 = CreateWindow(L"button", L"Edit Color Of Light", WS_CHILD|WS_BORDER, 
			25, 480, 170, 23, mainWindow, (HMENU)ID_OSBUTTON2, hInstance, NULL);


		OSDWindow = new dlgWnd;
		OSDWindow->initDlgWnd(mainWindow, (DLGPROC)OSDlgWndProc, hInstance);
		OSDWindow->dlgTemplate(WS_SIZEBOX|WS_POPUP|WS_CHILD|WS_OVERLAPPEDWINDOW|WS_VISIBLE,
			225, 130, 320, 400, "Object Settings Editor");

		OSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 5, 70, 20, ID_EOMSTATIC1, "static", "Diffuse");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 15, 60, 20, ID_EOMEDIT1, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 15, 60, 20, ID_EOMEDIT2, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 15, 60, 20, ID_EOMEDIT3, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 15, 60, 20, ID_EOMEDIT4, "edit", "");

		OSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 25, 70, 20, ID_EOMSTATIC1, "static", "Ambient");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 35, 60, 20, ID_EOMEDIT5, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 35, 60, 20, ID_EOMEDIT6, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 35, 60, 20, ID_EOMEDIT7, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 35, 60, 20, ID_EOMEDIT8, "edit", "");

		OSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 45, 70, 20, ID_EOMSTATIC1, "static", "Specular");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 55, 60, 20, ID_EOMEDIT9, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 55, 60, 20, ID_EOMEDIT10, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 55, 60, 20, ID_EOMEDIT11, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 55, 60, 20, ID_EOMEDIT12, "edit", "");

		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE, 125, 15, 30, 100, ID_EOMBUTTON1, "button", "Set");

		OSDWindow->createModelessDlgWindow();
	}

	void fillObjectSettings(D3DMATERIAL9* bMaterial)
	{
		material = bMaterial;

		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Diffuse.r); 
		SendMessage(OSDWindow->getItemHWND(ID_EOMEDIT1), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Diffuse.g); 
		SendMessage(OSDWindow->getItemHWND(ID_EOMEDIT2), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Diffuse.b); 
		SendMessage(OSDWindow->getItemHWND(ID_EOMEDIT3), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Diffuse.a); 
		SendMessage(OSDWindow->getItemHWND(ID_EOMEDIT4), WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Ambient.r); 
		SendMessage(OSDWindow->getItemHWND(ID_EOMEDIT5), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Ambient.g); 
		SendMessage(OSDWindow->getItemHWND(ID_EOMEDIT6), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Ambient.b); 
		SendMessage(OSDWindow->getItemHWND(ID_EOMEDIT7), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Ambient.a); 
		SendMessage(OSDWindow->getItemHWND(ID_EOMEDIT8), WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Specular.r); 
		SendMessage(OSDWindow->getItemHWND(ID_EOMEDIT9), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Specular.g); 
		SendMessage(OSDWindow->getItemHWND(ID_EOMEDIT10), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Specular.b); 
		SendMessage(OSDWindow->getItemHWND(ID_EOMEDIT11), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Specular.a); 
		SendMessage(OSDWindow->getItemHWND(ID_EOMEDIT12), WM_SETTEXT, NULL, (LPARAM)textBuffer);
	}

	void applyChanges()
	{
		OSDWindow->getItemTxt(ID_EOMEDIT1, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Diffuse.r = (float)_wtof((LPWSTR)textBuffer); 
		OSDWindow->getItemTxt(ID_EOMEDIT2, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Diffuse.g = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOMEDIT3, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Diffuse.b = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOMEDIT4, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Diffuse.a = (float)_wtof((LPWSTR)textBuffer);

		OSDWindow->getItemTxt(ID_EOMEDIT5, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Ambient.r = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOMEDIT6, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Ambient.g = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOMEDIT7, (LPWSTR)textBuffer, BUFFERSIZE);;
		material->Ambient.b = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOMEDIT8, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Ambient.a = (float)_wtof((LPWSTR)textBuffer);

		OSDWindow->getItemTxt(ID_EOMEDIT9, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Specular.r = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOMEDIT10, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Specular.g = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOMEDIT11, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Specular.b = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOMEDIT12, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Specular.a = (float)_wtof((LPWSTR)textBuffer);

		this->fillObjectSettings(material);
	} 


	void showSettingsWnd()
	{
		OSDWindow->showDlgWindow();
	}

	void showOSButton(int b)
	{						
		ShowWindow(OSButton1, b);
		ShowWindow(OSButton2, SW_HIDE);	
	}

	void showLSButton(int b)
	{
		ShowWindow(OSButton2, b);							
		ShowWindow(OSButton1, SW_HIDE);
	}

	HWND getWindow(int windowType)
	{
		switch(windowType)
		{
		case GET_OSELIGHTCOLORB:
			return OSButton2;
		case GET_OSEOBJECTMATERIALB:
			return OSButton1;
		}
	}

	~object_settings()
	{
		delete OSDWindow;
	}
};
