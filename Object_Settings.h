#define ID_OSSTATIC1 11001
#define ID_OSBUTTON1 12001
#define ID_OSBUTTON2 12002

#define ID_EOMSTATIC1 13001
#define ID_EOMEDIT1	  14001
#define ID_EOMEDIT2	  14002
#define ID_EOMEDIT3	  14003

#define ID_EOMSTATIC2 13002
#define ID_EOMEDIT4	  14004
#define ID_EOMEDIT5	  14005
#define ID_EOMEDIT6	  14006

#define ID_EOMSTATIC3 13003
#define ID_EOMEDIT7	  14007
#define ID_EOMEDIT8	  14008
#define ID_EOMEDIT9	  14009

#define ID_EOMBUTTON1 15001

#define GET_OSEOBJECTMATERIALB 1
#define GET_OSELIGHTCOLORB	   2

LPCTSTR textBuffer[32];

class object_settings
{
private:
	HWND OSStatic, OSButton1, OSButton2;
	HWND EOMStatic1, EOMStatic2, EOMStatic3;
	HWND EOMEditDiffuse1,  EOMEditDiffuse2,  EOMEditDiffuse3;
	HWND EOMEditAmbient1,  EOMEditAmbient2,  EOMEditAmbient3;
	HWND EOMEditSpecular1, EOMEditSpecular2, EOMEditSpecular3;
	HWND EOMSet;
	HWND OSWindow;
	LPCTSTR OSClassName;

	D3DMATERIAL9* material;

public:
	void initObjectSettings(HWND mainWindow, HINSTANCE hInstance)
	{
		OSClassName = L"C.O.M.E. Engine : ObSettings"; 

		OSStatic = CreateWindow(L"static",  NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, 
			15, 450, 190, 350, mainWindow, (HMENU)ID_OSSTATIC1, hInstance, NULL);

		OSButton1 = CreateWindow(L"button", L"Edit Color Of Light", WS_CHILD|WS_BORDER, 
			25, 480, 170, 23, mainWindow, (HMENU)ID_OSBUTTON2, hInstance, NULL);

		OSButton2 = CreateWindow(L"button",  L"Edit Object's Material", WS_CHILD|WS_BORDER, 
			25, 455, 170, 23, mainWindow, (HMENU)ID_OSBUTTON1, hInstance, NULL);

		RegWndClass(OSWndProc, hInstance, OSClassName);

		OSWindow = CreateWindow(OSClassName, L"Object Settings Editor", WS_OVERLAPPEDWINDOW|WS_POPUPWINDOW|WS_CHILD|WS_VISIBLE,
			225, 130, 259, 400, mainWindow, NULL, hInstance, NULL);

		EOMStatic1 = CreateWindow(L"static", L"Diffuse", WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER,
			15, 15, 70, 20, OSWindow, (HMENU)ID_EOMSTATIC1, hInstance, NULL);
		EOMEditDiffuse1 = CreateWindow(L"edit", NULL, WS_CHILD|WS_VISIBLE|ES_CENTER|ES_NUMBER|WS_BORDER|ES_AUTOHSCROLL, 
			15, 36, 60, 20, OSWindow, (HMENU)ID_EOMEDIT1, hInstance, NULL);
		EOMEditDiffuse2 = CreateWindow(L"edit", NULL, WS_CHILD|WS_VISIBLE|ES_CENTER|ES_NUMBER|WS_BORDER|ES_AUTOHSCROLL, 
			76, 36, 60, 20, OSWindow, (HMENU)ID_EOMEDIT2, hInstance, NULL);
		EOMEditDiffuse3 = CreateWindow(L"edit", NULL, WS_CHILD|WS_VISIBLE|ES_CENTER|ES_NUMBER|WS_BORDER|ES_AUTOHSCROLL, 
			138, 36, 60, 20, OSWindow, (HMENU)ID_EOMEDIT3, hInstance, NULL);

		EOMStatic2 = CreateWindow(L"static", L"Ambient", WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER,
			15, 60, 70, 20, OSWindow, (HMENU)ID_EOMSTATIC2, hInstance, NULL);
		EOMEditAmbient1 = CreateWindow(L"edit", NULL, WS_CHILD|WS_VISIBLE|ES_CENTER|ES_NUMBER|WS_BORDER|ES_AUTOHSCROLL, 
			15, 81, 60, 20, OSWindow, (HMENU)ID_EOMEDIT4, hInstance, NULL);
		EOMEditAmbient2 = CreateWindow(L"edit", NULL, WS_CHILD|WS_VISIBLE|ES_CENTER|ES_NUMBER|WS_BORDER|ES_AUTOHSCROLL, 
			76, 81, 60, 20, OSWindow, (HMENU)ID_EOMEDIT5, hInstance, NULL);
		EOMEditAmbient3 = CreateWindow(L"edit", NULL, WS_CHILD|WS_VISIBLE|ES_CENTER|ES_NUMBER|WS_BORDER|ES_AUTOHSCROLL, 
			138, 81, 60, 20, OSWindow, (HMENU)ID_EOMEDIT6, hInstance, NULL);

		EOMStatic3 = CreateWindow(L"static", L"Specular", WS_CHILD|WS_BORDER|WS_VISIBLE|SS_CENTER,
			15, 105, 70, 20, OSWindow, (HMENU)ID_EOMSTATIC3, hInstance, NULL);
		EOMEditSpecular1 = CreateWindow(L"edit", NULL, WS_CHILD|WS_VISIBLE|ES_CENTER|ES_NUMBER|WS_BORDER|ES_AUTOHSCROLL, 
			15, 126, 60, 20, OSWindow, (HMENU)ID_EOMEDIT7, hInstance, NULL);
		EOMEditSpecular2 = CreateWindow(L"edit", NULL, WS_CHILD|WS_VISIBLE|ES_CENTER|ES_NUMBER|WS_BORDER|ES_AUTOHSCROLL, 
			76, 126, 60, 20, OSWindow, (HMENU)ID_EOMEDIT8, hInstance, NULL);
		EOMEditSpecular3 = CreateWindow(L"edit", NULL, WS_CHILD|WS_VISIBLE|ES_CENTER|ES_NUMBER|WS_BORDER|ES_AUTOHSCROLL, 
			138, 126, 60, 20, OSWindow, (HMENU)ID_EOMEDIT9, hInstance, NULL);

		EOMSet = CreateWindow(L"button", L"Set", WS_CHILD|WS_VISIBLE, 
			199, 35, 30, 111, OSWindow, (HMENU)ID_EOMBUTTON1, hInstance, NULL);

		SendMessage(EOMEditDiffuse1, WM_SETTEXT, NULL, (LPARAM)L"2");
		SendMessage(EOMEditDiffuse2, WM_SETTEXT, NULL, (LPARAM)L"2");
		SendMessage(EOMEditDiffuse3, WM_SETTEXT, NULL, (LPARAM)L"8");
	}

	void fillObjectSettings(D3DMATERIAL9* bMaterial)
	{
		material = bMaterial;

		wsprintf ((LPWSTR)textBuffer, L"%d", (int)material->Diffuse.r);
		SendMessage(EOMEditDiffuse1, WM_SETTEXT, NULL, (LPARAM)textBuffer);
		wsprintf ((LPWSTR)textBuffer, L"%d", (int)material->Diffuse.g);
		SendMessage(EOMEditDiffuse2, WM_SETTEXT, NULL, (LPARAM)textBuffer);
		wsprintf ((LPWSTR)textBuffer, L"%d", (int)material->Diffuse.b);
		SendMessage(EOMEditDiffuse3, WM_SETTEXT, NULL, (LPARAM)textBuffer);

		wsprintf ((LPWSTR)textBuffer, L"%d", (int)material->Ambient.r);
		SendMessage(EOMEditAmbient1, WM_SETTEXT, NULL, (LPARAM)textBuffer);
		wsprintf ((LPWSTR)textBuffer, L"%d", (int)material->Ambient.g);
		SendMessage(EOMEditAmbient2, WM_SETTEXT, NULL, (LPARAM)textBuffer);
		wsprintf ((LPWSTR)textBuffer, L"%d", (int)material->Ambient.b);
		SendMessage(EOMEditAmbient3, WM_SETTEXT, NULL, (LPARAM)textBuffer);

		wsprintf ((LPWSTR)textBuffer, L"%d", (int)material->Specular.r);
		SendMessage(EOMEditSpecular1, WM_SETTEXT, NULL, (LPARAM)textBuffer);
		wsprintf ((LPWSTR)textBuffer, L"%d", (int)material->Specular.g);
		SendMessage(EOMEditSpecular2, WM_SETTEXT, NULL, (LPARAM)textBuffer);
		wsprintf ((LPWSTR)textBuffer, L"%d", (int)material->Specular.b);
		SendMessage(EOMEditSpecular3, WM_SETTEXT, NULL, (LPARAM)textBuffer);
	}

	void applyChanges()
	{
		GetDlgItemText(OSWindow,ID_EOMEDIT1,(LPWSTR)textBuffer,sizeof(textBuffer));
		material->Diffuse.r = (float)atof((const char*)textBuffer);
		GetDlgItemText(OSWindow,ID_EOMEDIT2,(LPWSTR)textBuffer,sizeof(textBuffer));
		material->Diffuse.g = (float)atof((const char*)textBuffer);
		GetDlgItemText(OSWindow,ID_EOMEDIT3,(LPWSTR)textBuffer,sizeof(textBuffer));
		material->Diffuse.b = (float)atof((const char*)textBuffer);

		GetDlgItemText(OSWindow,ID_EOMEDIT4,(LPWSTR)textBuffer,sizeof(textBuffer));
		material->Ambient.r = (float)atof((const char*)textBuffer);
		GetDlgItemText(OSWindow,ID_EOMEDIT5,(LPWSTR)textBuffer,sizeof(textBuffer));
		material->Ambient.g = (float)atof((const char*)textBuffer);
		GetDlgItemText(OSWindow,ID_EOMEDIT6,(LPWSTR)textBuffer,sizeof(textBuffer));
		material->Ambient.b = (float)atof((const char*)textBuffer);

		GetDlgItemText(OSWindow,ID_EOMEDIT7,(LPWSTR)textBuffer,sizeof(textBuffer));
		material->Specular.r = (float)atof((const char*)textBuffer);
		GetDlgItemText(OSWindow,ID_EOMEDIT8,(LPWSTR)textBuffer,sizeof(textBuffer));
		material->Specular.g = (float)atof((const char*)textBuffer);
		GetDlgItemText(OSWindow,ID_EOMEDIT9,(LPWSTR)textBuffer,sizeof(textBuffer));
		material->Specular.b = (float)atof((const char*)textBuffer);

		this->fillObjectSettings(material);
	}

	void showObjectSettings(int b)
	{						
		ShowWindow(OSButton2, b);
		ShowWindow(OSButton1, SW_HIDE);	
	}

	void showLightSettings(int b)
	{
		ShowWindow(OSButton1, b);							
		ShowWindow(OSButton2, SW_HIDE);
	}

	HWND getWindow(int windowType)
	{
		switch(windowType)
		{
		case GET_OSELIGHTCOLORB:
			return OSButton1;
		case GET_OSEOBJECTMATERIALB:
			return OSButton2;
		}
	}
};
