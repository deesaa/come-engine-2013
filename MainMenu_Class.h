#define FM_FILEOPEN 40001
#define FM_FILESAVE 40002
#define EM_EDITMTRL 41001
#define EM_EDITRDST 41002
#define VM_WNDLOCAT 42001

class mainMenu_class
{
private:
	MENUITEMINFO menuItemInfo;

	HWND mainWindow;
	HMENU mainMenu; 
	HMENU fileMenu, editMenu, viewMenu;
public:

	void initMainMenu(HWND bMainWindow)
	{
		mainWindow = bMainWindow;

		mainMenu = CreateMenu();
		fileMenu = CreatePopupMenu();
		this->createMenuItem(fileMenu, L"&Open Project", 0, FM_FILEOPEN, NULL, FALSE, MFT_STRING);
		this->createMenuItem(fileMenu, L"&Save Project", 1, FM_FILESAVE, NULL, FALSE, MFT_STRING);
		this->createMenuItem(fileMenu, NULL, 2, NULL, NULL, FALSE, MFT_SEPARATOR);

		editMenu = CreatePopupMenu();
		this->createMenuItem(editMenu, L"&Edit Material", 0, EM_EDITMTRL, NULL, FALSE, MFT_STRING);
		this->createMenuItem(editMenu, L"&Edit Render State", 1, EM_EDITRDST, NULL, FALSE, MFT_STRING);

		this->createMenuItem(mainMenu, L"File", 0, NULL, fileMenu, FALSE, MFT_STRING);
		this->createMenuItem(mainMenu, L"Edit", 1, NULL, editMenu, FALSE, MFT_STRING);
		SetMenu(mainWindow, mainMenu);
		DrawMenuBar(mainWindow);
	}

	void createMenuItem(HMENU hMenu, WCHAR* str, UINT uIns, UINT itemID,
		HMENU bSubMenu, bool flag, UINT type)
	{
		ZeroMemory(&menuItemInfo, sizeof(menuItemInfo));
		menuItemInfo.cbSize = sizeof(MENUITEMINFO);
		menuItemInfo.fMask = MIIM_STATE|MIIM_TYPE|MIIM_SUBMENU|MIIM_ID;
		menuItemInfo.fType = type;
		menuItemInfo.fState = MFS_ENABLED;
		menuItemInfo.dwTypeData = (LPWSTR)str;
		menuItemInfo.cch = sizeof(str);
		menuItemInfo.wID = itemID;
		menuItemInfo.hSubMenu = bSubMenu;
		InsertMenuItem(hMenu, uIns, flag, &menuItemInfo);
	}
};