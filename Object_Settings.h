#define ID_OSSTATIC1  11001
#define ID_OSBUTTON1  12001
#define ID_OSBUTTON2  12002
/************************************************************************/
/*                                                                      */
/************************************************************************/
#define ID_EOSTATIC1  13001				
#define ID_EOEDIT1	  14001
#define ID_EOEDIT2	  14002
#define ID_EOEDIT3	  14003
#define ID_EOEDIT4	  14004

#define ID_EOSTATIC2  13002
#define ID_EOEDIT5	  14005
#define ID_EOEDIT6	  14006
#define ID_EOEDIT7	  14007
#define ID_EOEDIT8	  14008

#define ID_EOSTATIC3  13003
#define ID_EOEDIT9	  14009
#define ID_EOEDIT10   14010
#define ID_EOEDIT11   14011
#define ID_EOEDIT12   14012

#define ID_EOSTATIC4  13004
#define ID_EOEDIT13   14013
#define ID_EOEDIT14   14014
#define ID_EOEDIT15   14015
#define ID_EOEDIT16   14016

#define ID_EOSTATIC5  13005
#define ID_EOEDIT17   14017

#define ID_EOBUTTON1  15001
/************************************************************************/
/*                                                                      */
/************************************************************************/
#define ID_ELSTATIC1  23001
#define ID_ELEDIT1	  24001
#define ID_ELEDIT2	  24002
#define ID_ELEDIT3	  24003
#define ID_ELEDIT4	  24004

#define ID_ELSTATIC2  23002
#define ID_ELEDIT5	  24005
#define ID_ELEDIT6	  24006
#define ID_ELEDIT7	  24007
#define ID_ELEDIT8	  24008

#define ID_ELSTATIC3  23003
#define ID_ELEDIT9	  24009
#define ID_ELEDIT10   24010
#define ID_ELEDIT11   24011
#define ID_ELEDIT12   24012

#define ID_ELSTATIC4  23004
#define ID_ELEDIT13   24013
#define ID_ELEDIT14   24014
#define ID_ELEDIT15   24015
#define ID_ELEDIT16   24016

#define ID_ELSTATIC5  23005
#define ID_ELEDIT17   24017

#define ID_ELBUTTON1  25001
/************************************************************************/
/*                                                                      */
/************************************************************************/
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
	dlgWnd* LSDWindow;

	HWND OSStatic, OSButton2, OSButton1;

	D3DMATERIAL9* material;
	D3DLIGHT9* light;

public:
	void initLightSettings(HWND mainWindow, HINSTANCE hInstance)
	{
		OSButton2 = CreateWindow(L"button", L"Edit Light", WS_CHILD|WS_BORDER, 
			25, 480, 170, 23, mainWindow, (HMENU)ID_OSBUTTON2, hInstance, NULL);

		LSDWindow = new dlgWnd;
		LSDWindow->initDlgWnd(mainWindow, (DLGPROC)LSDlgWndProc, hInstance);
		LSDWindow->dlgTemplate(WS_SIZEBOX|WS_POPUP|WS_CHILD|WS_OVERLAPPEDWINDOW,
			109, 50, 290, 400, "Light Settings Editor");

		LSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 5, 70, 20, ID_ELSTATIC1, "static", "Diffuse");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 15, 60, 20, ID_ELEDIT1, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 15, 60, 20, ID_ELEDIT2, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 15, 60, 20, ID_ELEDIT3, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 15, 60, 20, ID_ELEDIT4, "edit", "");

		LSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 25, 70, 20, ID_ELSTATIC2, "static", "Ambient");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 35, 60, 20, ID_ELEDIT5, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 35, 60, 20, ID_ELEDIT6, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 35, 60, 20, ID_ELEDIT7, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 35, 60, 20, ID_ELEDIT8, "edit", "");

		LSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 45, 70, 20, ID_ELSTATIC3, "static", "Specular");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 55, 60, 20, ID_ELEDIT9, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 55, 60, 20, ID_ELEDIT10, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 55, 60, 20, ID_ELEDIT11, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 55, 60, 20, ID_ELEDIT12, "edit", "");

		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE, 125, 15, 30, 100, ID_ELBUTTON1, "button", "Set");

		LSDWindow->createModelessDlgWindow();
	}

	void initObjectSettings(HWND mainWindow, HINSTANCE hInstance)
	{
		//По хорошему нужно бы вынести эту функцию за скобки...
		OSStatic = CreateWindow(L"static",  NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, 
			15, 450, 190, 350, mainWindow, (HMENU)ID_OSSTATIC1, hInstance, NULL);

		OSButton1 = CreateWindow(L"button",  L"Edit Object", WS_CHILD|WS_BORDER, 
			25, 455, 170, 23, mainWindow, (HMENU)ID_OSBUTTON1, hInstance, NULL);

		OSDWindow = new dlgWnd;
		OSDWindow->initDlgWnd(mainWindow, (DLGPROC)OSDlgWndProc, hInstance);
		OSDWindow->dlgTemplate(WS_SIZEBOX|WS_POPUP|WS_CHILD|WS_OVERLAPPEDWINDOW,
			109, 50, 290, 400, "Object Settings Editor");

		OSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 5, 70, 20, ID_EOSTATIC1, "static", "Diffuse");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 15, 60, 20, ID_EOEDIT1, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 15, 60, 20, ID_EOEDIT2, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 15, 60, 20, ID_EOEDIT3, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 15, 60, 20, ID_EOEDIT4, "edit", "");

		OSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 25, 70, 20, ID_EOSTATIC1, "static", "Ambient");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 35, 60, 20, ID_EOEDIT5, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 35, 60, 20, ID_EOEDIT6, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 35, 60, 20, ID_EOEDIT7, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 35, 60, 20, ID_EOEDIT8, "edit", "");

		OSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 45, 70, 20, ID_EOSTATIC1, "static", "Specular");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 55, 60, 20, ID_EOEDIT9, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 55, 60, 20, ID_EOEDIT10, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 55, 60, 20, ID_EOEDIT11, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 55, 60, 20, ID_EOEDIT12, "edit", "");

		OSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 65, 70, 20, ID_EOSTATIC4, "static", "Emissive");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 75, 60, 20, ID_EOEDIT13, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 75, 60, 20, ID_EOEDIT14, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 75, 60, 20, ID_EOEDIT15, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 75, 60, 20, ID_EOEDIT16, "edit", "");

		OSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 85, 70, 20, ID_EOSTATIC5, "static", "Power");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 95, 60, 20, ID_EOEDIT17, "edit", "");

		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE, 125, 15, 30, 100, ID_EOBUTTON1, "button", "Set");

		OSDWindow->createModelessDlgWindow();
	}

	void fillObjectSettings(D3DMATERIAL9* bMaterial)
	{
		material = bMaterial;

		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Diffuse.r); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT1), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Diffuse.g); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT2), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Diffuse.b); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT3), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Diffuse.a); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT4), WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Ambient.r); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT5), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Ambient.g); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT6), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Ambient.b); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT7), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Ambient.a); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT8), WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Specular.r); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT9), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Specular.g); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT10), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Specular.b); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT11), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Specular.a); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT12), WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Emissive.r); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT13), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Emissive.g); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT14), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Emissive.b); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT15), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Emissive.a); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT16), WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)material->Power); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT17), WM_SETTEXT, NULL, (LPARAM)textBuffer);
	}

	void fillLightSettings(D3DLIGHT9* bLight)
	{
		light = bLight;

		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)light->Diffuse.r); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT1), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)light->Diffuse.g); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT2), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)light->Diffuse.b); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT3), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)light->Diffuse.a); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT4), WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)light->Ambient.r); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT5), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)light->Ambient.g); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT6), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)light->Ambient.b); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT7), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)light->Ambient.a); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT8), WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)light->Specular.r); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT9), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)light->Specular.g); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT10), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)light->Specular.b); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT11), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFERSIZE, L"%f", (float)light->Specular.a); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT12), WM_SETTEXT, NULL, (LPARAM)textBuffer);
	}

	void applyEOChanges()
	{
		OSDWindow->getItemTxt(ID_EOEDIT1, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Diffuse.r = (float)_wtof((LPWSTR)textBuffer); 
		OSDWindow->getItemTxt(ID_EOEDIT2, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Diffuse.g = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT3, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Diffuse.b = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT4, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Diffuse.a = (float)_wtof((LPWSTR)textBuffer);

		OSDWindow->getItemTxt(ID_EOEDIT5, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Ambient.r = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT6, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Ambient.g = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT7, (LPWSTR)textBuffer, BUFFERSIZE);;
		material->Ambient.b = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT8, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Ambient.a = (float)_wtof((LPWSTR)textBuffer);

		OSDWindow->getItemTxt(ID_EOEDIT9, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Specular.r = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT10, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Specular.g = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT11, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Specular.b = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT12, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Specular.a = (float)_wtof((LPWSTR)textBuffer);

		OSDWindow->getItemTxt(ID_EOEDIT13, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Emissive.r = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT14, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Emissive.g = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT15, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Emissive.b = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT16, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Emissive.a = (float)_wtof((LPWSTR)textBuffer);

		OSDWindow->getItemTxt(ID_EOEDIT17, (LPWSTR)textBuffer, BUFFERSIZE);
		material->Power = (float)_wtof((LPWSTR)textBuffer);

		this->fillObjectSettings(material);
	} 

	void applyELChanges()
	{
		LSDWindow->getItemTxt(ID_ELEDIT1, (LPWSTR)textBuffer, BUFFERSIZE);
		light->Diffuse.r = (float)_wtof((LPWSTR)textBuffer); 
		LSDWindow->getItemTxt(ID_ELEDIT2, (LPWSTR)textBuffer, BUFFERSIZE);
		light->Diffuse.g = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT3, (LPWSTR)textBuffer, BUFFERSIZE);
		light->Diffuse.b = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT4, (LPWSTR)textBuffer, BUFFERSIZE);
		light->Diffuse.a = (float)_wtof((LPWSTR)textBuffer);

		LSDWindow->getItemTxt(ID_ELEDIT5, (LPWSTR)textBuffer, BUFFERSIZE);
		light->Ambient.r = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT6, (LPWSTR)textBuffer, BUFFERSIZE);
		light->Ambient.g = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT7, (LPWSTR)textBuffer, BUFFERSIZE);;
		light->Ambient.b = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT8, (LPWSTR)textBuffer, BUFFERSIZE);
		light->Ambient.a = (float)_wtof((LPWSTR)textBuffer);

		LSDWindow->getItemTxt(ID_ELEDIT9, (LPWSTR)textBuffer, BUFFERSIZE);
		light->Specular.r = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT10, (LPWSTR)textBuffer, BUFFERSIZE);
		light->Specular.g = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT11, (LPWSTR)textBuffer, BUFFERSIZE);
		light->Specular.b = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT12, (LPWSTR)textBuffer, BUFFERSIZE);
		light->Specular.a = (float)_wtof((LPWSTR)textBuffer);

		this->fillLightSettings(light);
	}

	void showOSettingsWnd()
	{	OSDWindow->showDlgWindow();}

	void showLSettingsWnd()
	{	LSDWindow->showDlgWindow();}

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
		delete LSDWindow;
	}
};
