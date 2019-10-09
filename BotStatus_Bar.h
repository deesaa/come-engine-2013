#define ID_BSBSTATIC1 10001
#define ID_BSBSTATIC2 10002
#define ID_BSBSTATIC3 10003
#define ID_BSBSTATIC4 10004
#define ID_BSBSTATIC5 10005
#define ID_BSBSTATIC6 10006
#define ID_BSBSTATIC7 10007
#define ID_BSBSTATIC8 10008

class botStatusBar_Class
{
private:

	HWND mainWindow;
	HINSTANCE hInstance;
	DWORD lastNumPickedVerts;

	int radix;

	HWND BSBStatic1, BSBStatic2;
	HWND BSBStatic3, BSBStatic4, BSBStatic5, BSBStatic6, BSBStatic7, BSBStatic8;

public:
	void initBStatBar(HWND bMainWindow, HINSTANCE bhInstance)
	{
		mainWindow = bMainWindow;
		hInstance  = bhInstance;
		radix = 10;
		lastNumPickedVerts = 0;
		
		BSBStatic1 = CreateWindow(L"static", L"Num Picked Verts", WS_CHILD|SS_CENTER, 
			217, 805, 120, 15, mainWindow, (HMENU)ID_BSBSTATIC1, hInstance, NULL);

		BSBStatic2 = CreateWindow(L"edit", L"0", WS_CHILD|ES_CENTER|ES_AUTOHSCROLL, 
			338, 805, 45, 15, mainWindow, (HMENU)ID_BSBSTATIC2, hInstance, NULL);


		BSBStatic3 = CreateWindow(L"static", L"X", WS_CHILD|SS_CENTER, 
			417, 805, 15, 15, mainWindow, (HMENU)ID_BSBSTATIC3, hInstance, NULL);

		BSBStatic4 = CreateWindow(L"edit", L"0", WS_CHILD|ES_CENTER|ES_AUTOHSCROLL, 
			434, 805, 125, 15, mainWindow, (HMENU)ID_BSBSTATIC4, hInstance, NULL);


		BSBStatic5 = CreateWindow(L"static", L"Y", WS_CHILD|SS_CENTER, 
			563, 805, 15, 15, mainWindow, (HMENU)ID_BSBSTATIC5, hInstance, NULL);

		BSBStatic6 = CreateWindow(L"edit", L"0", WS_CHILD|ES_CENTER|ES_AUTOHSCROLL, 
			580, 805, 125, 15, mainWindow, (HMENU)ID_BSBSTATIC6, hInstance, NULL);


		BSBStatic7 = CreateWindow(L"static", L"Z", WS_CHILD|SS_CENTER, 
			709, 805, 15, 15, mainWindow, (HMENU)ID_BSBSTATIC7, hInstance, NULL);

		BSBStatic8 = CreateWindow(L"edit", L"0", WS_CHILD|ES_CENTER|ES_AUTOHSCROLL, 
			726, 805, 125, 15, mainWindow, (HMENU)ID_BSBSTATIC8, hInstance, NULL);

	}

	void writeVertPos(D3DXVECTOR3* pos)
	{
		WCHAR textBuffer[32];

		swprintf((LPWSTR)textBuffer, 32, L"%f", (float)pos->x); 
		SendMessage(BSBStatic4, WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf((LPWSTR)textBuffer, 32, L"%f", (float)pos->y); 
		SendMessage(BSBStatic6, WM_SETTEXT, NULL, (LPARAM)textBuffer);

		swprintf((LPWSTR)textBuffer, 32, L"%f", (float)pos->z); 
		SendMessage(BSBStatic8, WM_SETTEXT, NULL, (LPARAM)textBuffer);
	}

	void writeNumPickedVerts(DWORD numPickedVerts)
	{
		if(numPickedVerts != lastNumPickedVerts)
		{
			lastNumPickedVerts = numPickedVerts;

			WCHAR textBuffer[32];

			swprintf((LPWSTR)textBuffer, 32, L"%d", lastNumPickedVerts); 
			SendMessage(BSBStatic2, WM_SETTEXT, NULL, (LPARAM)textBuffer);
		}
	}
	void showBar()
	{
		ShowWindow(BSBStatic1, SW_NORMAL);
		ShowWindow(BSBStatic2, SW_NORMAL);
		ShowWindow(BSBStatic3, SW_NORMAL);
		ShowWindow(BSBStatic4, SW_NORMAL);
		ShowWindow(BSBStatic5, SW_NORMAL);
		ShowWindow(BSBStatic6, SW_NORMAL);
		ShowWindow(BSBStatic7, SW_NORMAL);
		ShowWindow(BSBStatic8, SW_NORMAL);
	}
	void hideBar()
	{
		ShowWindow(BSBStatic1, SW_HIDE);
		ShowWindow(BSBStatic2, SW_HIDE);
		ShowWindow(BSBStatic3, SW_HIDE);
		ShowWindow(BSBStatic4, SW_HIDE);
		ShowWindow(BSBStatic5, SW_HIDE);
		ShowWindow(BSBStatic6, SW_HIDE);
		ShowWindow(BSBStatic7, SW_HIDE);
		ShowWindow(BSBStatic8, SW_HIDE);
	}
};