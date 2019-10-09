class dlgWnd
{
private:
	WORD* p;
	WORD* pDlgTemplate;
	WORD* dlgItems;

	int cxChar, cyChar;
	DWORD dlgUnit;
	int dlgWiUnit, dlgHeUnit; 

	HWND parent;
	HWND modelessDlgWnd;
	DLGPROC dlgProc;
	HINSTANCE hInstance;
public:
	void initDlgWnd(HWND bParent, DLGPROC bDlgProc, HINSTANCE bhInstance)
	{
		parent  = bParent;
		dlgProc = bDlgProc;
		hInstance = bhInstance;

		TEXTMETRIC tm; 
		HDC hcd = GetDC(parent);
		GetTextMetrics(hcd, &tm);
		ReleaseDC(parent, hcd);
		cxChar = tm.tmAveCharWidth + 1;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		dlgUnit = GetDialogBaseUnits();

		dlgWiUnit = LOWORD(dlgUnit);
		dlgHeUnit = HIWORD(dlgUnit);

		cxChar = cxChar*4/dlgWiUnit;
		cyChar = cyChar*8/dlgHeUnit;
	}

	LPWORD lpwAlign(LPWORD lpln)
	{
		ULONG ul = (ULONG)lpln;
		ul += 3; ul >>= 2; ul <<= 2;
		return (LPWORD)ul;
	}

	int nCopyAnsiToWideChar(LPWORD lpWCStr, LPSTR lpAnsiln)
	{
		int cchAnsi = strlen(lpAnsiln);
		return MultiByteToWideChar(GetACP(), MB_PRECOMPOSED, 
			lpAnsiln, cchAnsi, (LPWSTR)lpWCStr, cchAnsi) + 1;
	}

	void dlgTemplate(DWORD lStyle, int x, int y, 
		int cX, int cY, LPSTR txt)
	{
		pDlgTemplate = p = (PWORD)LocalAlloc(LPTR, 8192);

		*p++ = LOWORD(lStyle);
		*p++ = HIWORD(lStyle); 
		*p++ = NULL;
		*p++ = NULL;
		dlgItems = p;
		*p++;
		*p++ = x;
		*p++ = y;
		*p++ = cX*4/dlgWiUnit;
		*p++ = cY*8/dlgHeUnit;
		*p++ = NULL;
		*p++ = NULL;
		int nChar = this->nCopyAnsiToWideChar(p, txt);
		p += nChar;
		p = this->lpwAlign((LPWORD)p);
	}

	void dlgItemTemplate(DWORD lStyle, int x, int y, int cX, int cY, 
		WORD dlgItemID, LPSTR classname, LPSTR txt)
	{
		*p++ = LOWORD(lStyle);
		*p++ = HIWORD(lStyle);
		*p++ = NULL;
		*p++ = NULL;
		*p++ = x;
		*p++ = y;
		*p++ = cX*4/dlgWiUnit;
		*p++ = cY*8/dlgHeUnit;
		*p++ = dlgItemID;

		int nChar = this->nCopyAnsiToWideChar(p, classname);
		p += nChar;
		if(strlen(txt) > 0)
			nChar = this->nCopyAnsiToWideChar(p, txt);
		else
			nChar = this->nCopyAnsiToWideChar(p, "");

		p += nChar;
		*p++ = NULL;
		p = this->lpwAlign((LPWORD)p);

		*dlgItems += 1;
	}

	void createModalDlgWindow()
	{
		DialogBoxIndirect(hInstance, (LPDLGTEMPLATE)pDlgTemplate,
			parent, (DLGPROC)dlgProc);
	}

	HWND createModelessDlgWindow()
	{
		 return modelessDlgWnd = CreateDialogIndirect(hInstance, (LPDLGTEMPLATE)pDlgTemplate,
			parent, (DLGPROC)dlgProc);
	}

	void getItemTxt(int itemID, LPWSTR txtBuffer, int bufferSize)
	{
		GetDlgItemText(modelessDlgWnd, itemID, txtBuffer, sizeof(WCHAR)*bufferSize);
		
	}

	HWND getItemHWND(int itemID)
	{
		return GetDlgItem(modelessDlgWnd, itemID);
	}

	HWND getHWND()
	{
		 return modelessDlgWnd;
	}

	bool isVisible()
	{
		return IsWindowVisible(modelessDlgWnd);
	}

	void showDlgWindow()
	{
		if(IsWindowVisible(modelessDlgWnd))
			ShowWindow(modelessDlgWnd, SW_HIDE);
		else
			ShowWindow(modelessDlgWnd, SW_NORMAL);
	}

	~dlgWnd()
	{
		LocalFree(LocalHandle(pDlgTemplate));
	}
};