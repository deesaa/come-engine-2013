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
/************************************************************************/
/*                                                                      */
/************************************************************************/
#define ID_EOCEDIT1   30001
#define ID_EOCEDIT2   30002
#define ID_EOCEDIT3   30003
#define ID_EOCEDIT4   30004

#define ID_ELCEDIT1   30005
#define ID_ELCEDIT2   30006
#define ID_ELCEDIT3   30007
#define ID_ELCEDIT4   30008

#define BUFFER		  6
#define	LONGBUFFER	  14

#define PALETTESIZE   256

WCHAR textBuffer[BUFFER];
WCHAR longTextBuffer[LONGBUFFER];

class object_settings
{
private:
	dlgWnd* OSDWindow;
	dlgWnd* LSDWindow;

	HWND OSStatic, OSButton2, OSButton1;

	HWND mainWindow;
	HINSTANCE hInstance;

	CHOOSECOLOR chooseColor;

	D3DMATERIAL9* material;
	D3DLIGHT9* light;

public:
	void initSettings(HWND bMainWindow, HINSTANCE bhInstance)
	{
		mainWindow = bMainWindow;
		hInstance  = bhInstance;
	}
	void initLightSettings()
	{
		OSButton2 = CreateWindow(L"button", L"Edit Light", WS_CHILD|WS_BORDER, 
			25, Height * 0.5, 170, 23, mainWindow, (HMENU)ID_OSBUTTON2, hInstance, NULL);

		LSDWindow = new dlgWnd;
		LSDWindow->initDlgWnd(mainWindow, (DLGPROC)LSDlgWndProc, hInstance);
		LSDWindow->dlgTemplate(WS_SIZEBOX|WS_POPUP|WS_CHILD|WS_OVERLAPPEDWINDOW,
			109, 50, 310, 400, "Light Settings Editor");

		LSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 5, 70, 20, ID_ELSTATIC1, "static", "Diffuse");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 15, 60, 20, ID_ELEDIT1, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 15, 60, 20, ID_ELEDIT2, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 15, 60, 20, ID_ELEDIT3, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 15, 60, 20, ID_ELEDIT4, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|WS_BORDER, 125, 15, 20, 20, ID_ELCEDIT1, "button", ">");

		LSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 25, 70, 20, ID_ELSTATIC2, "static", "Ambient");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 35, 60, 20, ID_ELEDIT5, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 35, 60, 20, ID_ELEDIT6, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 35, 60, 20, ID_ELEDIT7, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 35, 60, 20, ID_ELEDIT8, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|WS_BORDER, 125, 35, 20, 20, ID_ELCEDIT2, "button", ">");

		LSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 45, 70, 20, ID_ELSTATIC3, "static", "Specular");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 55, 60, 20, ID_ELEDIT9, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 55, 60, 20, ID_ELEDIT10, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 55, 60, 20, ID_ELEDIT11, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 55, 60, 20, ID_ELEDIT12, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|WS_BORDER, 125, 55, 20, 20, ID_ELCEDIT3, "button", ">");

		LSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 65, 80, 20, ID_ELSTATIC4, "static", "Attenuation");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 75, 60, 20, ID_ELEDIT13, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 75, 60, 20, ID_ELEDIT14, "edit", "");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 75, 60, 20, ID_ELEDIT15, "edit", "");

		LSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 85, 80, 20, ID_ELSTATIC5, "static", "Range");
		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 95, 120, 20, ID_ELEDIT16, "edit", "");

		LSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE, 135, 15, 30, 180, ID_ELBUTTON1, "button", "Set");

		LSDWindow->createModelessDlgWindow();
	}

	void initObjectSettings()
	{
		OSButton1 = CreateWindow(L"button",  L"Edit Material", WS_CHILD|WS_BORDER, 
			25, Height * 0.5, 170, 23, mainWindow, (HMENU)ID_OSBUTTON1, hInstance, NULL);

		OSDWindow = new dlgWnd;
		OSDWindow->initDlgWnd(mainWindow, (DLGPROC)OSDlgWndProc, hInstance);
		OSDWindow->dlgTemplate(WS_SIZEBOX|WS_POPUP|WS_CHILD|WS_OVERLAPPEDWINDOW,
			109, 50, 310, 400, "Object Settings Editor");

		OSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 5, 70, 20, ID_EOSTATIC1, "static", "Diffuse");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 15, 60, 20, ID_EOEDIT1, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 15, 60, 20, ID_EOEDIT2, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 15, 60, 20, ID_EOEDIT3, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 15, 60, 20, ID_EOEDIT4, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|WS_BORDER, 125, 15, 20, 20, ID_EOCEDIT1, "button", ">");

		OSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 25, 70, 20, ID_EOSTATIC1, "static", "Ambient");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 35, 60, 20, ID_EOEDIT5, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 35, 60, 20, ID_EOEDIT6, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 35, 60, 20, ID_EOEDIT7, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 35, 60, 20, ID_EOEDIT8, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|WS_BORDER, 125, 35, 20, 20, ID_EOCEDIT2, "button", ">");

		OSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 45, 70, 20, ID_EOSTATIC1, "static", "Specular");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 55, 60, 20, ID_EOEDIT9, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 55, 60, 20, ID_EOEDIT10, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 55, 60, 20, ID_EOEDIT11, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 55, 60, 20, ID_EOEDIT12, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|WS_BORDER, 125, 55, 20, 20, ID_EOCEDIT3, "button", ">");

		OSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 65, 70, 20, ID_EOSTATIC4, "static", "Emissive");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 75, 60, 20, ID_EOEDIT13, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 35, 75, 60, 20, ID_EOEDIT14, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 65, 75, 60, 20, ID_EOEDIT15, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 95, 75, 60, 20, ID_EOEDIT16, "edit", "");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|WS_BORDER, 125, 75, 20, 20, ID_EOCEDIT4, "button", ">");

		OSDWindow->dlgItemTemplate(WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER, 5, 85, 70, 20, ID_EOSTATIC5, "static", "Power");
		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE|ES_CENTER|WS_BORDER|ES_AUTOHSCROLL, 5, 95, 60, 20, ID_EOEDIT17, "edit", "");

		OSDWindow->dlgItemTemplate(WS_CHILD|WS_VISIBLE, 135, 15, 30, 180, ID_EOBUTTON1, "button", "Set");

		OSDWindow->createModelessDlgWindow();
	}

	void fillObjectSettings(D3DMATERIAL9* bMaterial)
	{
		material = bMaterial;

		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Diffuse.r); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT1), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Diffuse.g); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT2), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Diffuse.b); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT3), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Diffuse.a); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT4), WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Ambient.r); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT5), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Ambient.g); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT6), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Ambient.b); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT7), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Ambient.a); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT8), WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Specular.r); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT9), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Specular.g); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT10), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Specular.b); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT11), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Specular.a); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT12), WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Emissive.r); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT13), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Emissive.g); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT14), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Emissive.b); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT15), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Emissive.a); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT16), WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Power); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT17), WM_SETTEXT, NULL, (LPARAM)textBuffer);
	}

	void fillLightSettings(D3DLIGHT9* bLight)
	{
		light = bLight;	

		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Diffuse.r); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT1), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Diffuse.g); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT2), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Diffuse.b); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT3), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Diffuse.a); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT4), WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Ambient.r); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT5), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Ambient.g); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT6), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Ambient.b); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT7), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Ambient.a); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT8), WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Specular.r); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT9), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Specular.g); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT10), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Specular.b); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT11), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Specular.a); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT12), WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Attenuation0); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT13), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Attenuation1); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT14), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Attenuation2); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT15), WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf ((LPWSTR)longTextBuffer, LONGBUFFER, L"%f", (float)light->Range); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT16), WM_SETTEXT, NULL, (LPARAM)longTextBuffer);

		switch(light->Type)
		{
		case D3DLIGHT_POINT:
			this->ShowWndsForPointLight();
			break;
		case D3DLIGHT_DIRECTIONAL:
			this->ShowWndsForDirectionLight();
			break;
		}
	}

	void ShowWndsForDirectionLight()
	{	
		ShowWindow(LSDWindow->getItemHWND(ID_ELSTATIC4), SW_HIDE);
		ShowWindow(LSDWindow->getItemHWND(ID_ELEDIT13), SW_HIDE);
		ShowWindow(LSDWindow->getItemHWND(ID_ELEDIT14), SW_HIDE);
		ShowWindow(LSDWindow->getItemHWND(ID_ELEDIT15), SW_HIDE);
		ShowWindow(LSDWindow->getItemHWND(ID_ELSTATIC5), SW_HIDE);
		ShowWindow(LSDWindow->getItemHWND(ID_ELEDIT16), SW_HIDE);

		MoveWindow(LSDWindow->getItemHWND(ID_ELBUTTON1), 271, 30, 30, 100, TRUE);
	}

	void ShowWndsForPointLight()
	{	
		ShowWindow(LSDWindow->getItemHWND(ID_ELSTATIC4), SW_NORMAL);
		ShowWindow(LSDWindow->getItemHWND(ID_ELEDIT13), SW_NORMAL);
		ShowWindow(LSDWindow->getItemHWND(ID_ELEDIT14), SW_NORMAL);
		ShowWindow(LSDWindow->getItemHWND(ID_ELEDIT15), SW_NORMAL);
		ShowWindow(LSDWindow->getItemHWND(ID_ELSTATIC5), SW_NORMAL);
		ShowWindow(LSDWindow->getItemHWND(ID_ELEDIT16), SW_NORMAL);

		MoveWindow(LSDWindow->getItemHWND(ID_ELBUTTON1), 271, 30, 30, 180, TRUE);
	}

	void applyEOChanges()
	{
		OSDWindow->getItemTxt(ID_EOEDIT1, (LPWSTR)textBuffer, BUFFER);
		material->Diffuse.r = (float)_wtof((LPWSTR)textBuffer); 
		OSDWindow->getItemTxt(ID_EOEDIT2, (LPWSTR)textBuffer, BUFFER);
		material->Diffuse.g = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT3, (LPWSTR)textBuffer, BUFFER);
		material->Diffuse.b = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT4, (LPWSTR)textBuffer, BUFFER);
		material->Diffuse.a = (float)_wtof((LPWSTR)textBuffer);

		OSDWindow->getItemTxt(ID_EOEDIT5, (LPWSTR)textBuffer, BUFFER);
		material->Ambient.r = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT6, (LPWSTR)textBuffer, BUFFER);
		material->Ambient.g = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT7, (LPWSTR)textBuffer, BUFFER);;
		material->Ambient.b = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT8, (LPWSTR)textBuffer, BUFFER);
		material->Ambient.a = (float)_wtof((LPWSTR)textBuffer);

		OSDWindow->getItemTxt(ID_EOEDIT9, (LPWSTR)textBuffer, BUFFER);
		material->Specular.r = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT10, (LPWSTR)textBuffer, BUFFER);
		material->Specular.g = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT11, (LPWSTR)textBuffer, BUFFER);
		material->Specular.b = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT12, (LPWSTR)textBuffer, BUFFER);
		material->Specular.a = (float)_wtof((LPWSTR)textBuffer);

		OSDWindow->getItemTxt(ID_EOEDIT13, (LPWSTR)textBuffer, BUFFER);
		material->Emissive.r = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT14, (LPWSTR)textBuffer, BUFFER);
		material->Emissive.g = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT15, (LPWSTR)textBuffer, BUFFER);
		material->Emissive.b = (float)_wtof((LPWSTR)textBuffer);
		OSDWindow->getItemTxt(ID_EOEDIT16, (LPWSTR)textBuffer, BUFFER);
		material->Emissive.a = (float)_wtof((LPWSTR)textBuffer);

		OSDWindow->getItemTxt(ID_EOEDIT17, (LPWSTR)textBuffer, BUFFER);
		material->Power = (float)_wtof((LPWSTR)textBuffer);

		this->fillObjectSettings(material);
	} 

	void applyELChanges()
	{
		LSDWindow->getItemTxt(ID_ELEDIT1, (LPWSTR)textBuffer, BUFFER);
		light->Diffuse.r = (float)_wtof((LPWSTR)textBuffer); 
		LSDWindow->getItemTxt(ID_ELEDIT2, (LPWSTR)textBuffer, BUFFER);
		light->Diffuse.g = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT3, (LPWSTR)textBuffer, BUFFER);
		light->Diffuse.b = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT4, (LPWSTR)textBuffer, BUFFER);
		light->Diffuse.a = (float)_wtof((LPWSTR)textBuffer);

		LSDWindow->getItemTxt(ID_ELEDIT5, (LPWSTR)textBuffer, BUFFER);
		light->Ambient.r = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT6, (LPWSTR)textBuffer, BUFFER);
		light->Ambient.g = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT7, (LPWSTR)textBuffer, BUFFER);;
		light->Ambient.b = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT8, (LPWSTR)textBuffer, BUFFER);
		light->Ambient.a = (float)_wtof((LPWSTR)textBuffer);

		LSDWindow->getItemTxt(ID_ELEDIT9, (LPWSTR)textBuffer, BUFFER);
		light->Specular.r = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT10, (LPWSTR)textBuffer, BUFFER);
		light->Specular.g = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT11, (LPWSTR)textBuffer, BUFFER);
		light->Specular.b = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT12, (LPWSTR)textBuffer, BUFFER);
		light->Specular.a = (float)_wtof((LPWSTR)textBuffer);

		LSDWindow->getItemTxt(ID_ELEDIT13, (LPWSTR)textBuffer, BUFFER);
		light->Attenuation0 = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT14, (LPWSTR)textBuffer, BUFFER);
		light->Attenuation1 = (float)_wtof((LPWSTR)textBuffer);
		LSDWindow->getItemTxt(ID_ELEDIT15, (LPWSTR)textBuffer, BUFFER);
		light->Attenuation2 = (float)_wtof((LPWSTR)textBuffer);

		LSDWindow->getItemTxt(ID_ELEDIT16, (LPWSTR)longTextBuffer, LONGBUFFER);
		light->Range = (float)_wtof((LPWSTR)longTextBuffer);

		this->fillLightSettings(light);
	}

	void editODiffuseColor()
	{
		initPalette(mainWindow, &material->Diffuse);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Diffuse.r); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT1), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Diffuse.g); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT2), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Diffuse.b); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT3), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Diffuse.a); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT4), WM_SETTEXT, NULL, (LPARAM)textBuffer);
	}
	void editOAmbientColor()
	{
		initPalette(mainWindow, &material->Ambient);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Ambient.r); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT5), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Ambient.g); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT6), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Ambient.b); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT7), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Ambient.a); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT8), WM_SETTEXT, NULL, (LPARAM)textBuffer);
	}
	void editOSpecularColor()
	{
		initPalette(mainWindow, &material->Specular);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Specular.r); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT9), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Specular.g); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT10), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Specular.b); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT11), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Specular.a); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT12), WM_SETTEXT, NULL, (LPARAM)textBuffer);
	}
	void editOEmissiveColor()
	{
		initPalette(mainWindow, &material->Emissive);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Emissive.r); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT13), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Emissive.g); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT14), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Emissive.b); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT15), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)material->Emissive.a); 
		SendMessage(OSDWindow->getItemHWND(ID_EOEDIT16), WM_SETTEXT, NULL, (LPARAM)textBuffer);
	}

	void editLDiffuseColor()
	{
		initPalette(mainWindow, &light->Diffuse);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Diffuse.r); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT1), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Diffuse.g); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT2), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Diffuse.b); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT3), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Diffuse.a); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT4), WM_SETTEXT, NULL, (LPARAM)textBuffer);
	}
	void editLAmbientColor()
	{
		initPalette(mainWindow, &light->Ambient);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Ambient.r); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT5), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Ambient.g); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT6), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Ambient.b); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT7), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Ambient.a); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT8), WM_SETTEXT, NULL, (LPARAM)textBuffer);
	}
	void editLSpecularColor()
	{
		initPalette(mainWindow, &light->Specular);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Specular.r); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT9), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Specular.g); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT10), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Specular.b); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT11), WM_SETTEXT, NULL, (LPARAM)textBuffer);
		swprintf ((LPWSTR)textBuffer, BUFFER, L"%f", (float)light->Specular.a); 
		SendMessage(LSDWindow->getItemHWND(ID_ELEDIT12), WM_SETTEXT, NULL, (LPARAM)textBuffer);
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

BOOL initPalette(HWND hwnd, D3DCOLORVALUE* component)
{
	CHOOSECOLOR cc;
	COLORREF aclrCust[16];
	int i;

	for (i = 0; i < 16; i++)
		aclrCust[i] = RGB(255, 255, 255);

	memset(&cc, 0, sizeof(CHOOSECOLOR));

	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hwnd;
	cc.rgbResult = RGB(component->r*225, component->g*255, component->b*255);
	cc.lpCustColors = aclrCust;
	cc.Flags = CC_RGBINIT|CC_FULLOPEN;

	ChooseColor(&cc);
		
	if(GetRValue(cc.rgbResult)*0.004f > 1)
		component->r = 1.0f;
	else
		component->r = GetRValue(cc.rgbResult)*0.004f;

	if(GetGValue(cc.rgbResult)*0.004f > 1)
		component->g = 1.0f;
	else
		component->g = GetGValue(cc.rgbResult)*0.004f;

	if(GetBValue(cc.rgbResult)*0.004f > 1)
		component->b = 1.0f;
	else
		component->b = GetBValue(cc.rgbResult)*0.004f;

	return TRUE;
}
