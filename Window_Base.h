/**Идентификаторы для второстепенных окон**/
//Кнопки
#define ID_BUTTON1  1001
#define ID_BUTTON2  1002
#define ID_BUTTON3  1003
#define ID_BUTTON4  1004
#define ID_BUTTON5  1005
#define ID_BUTTON6  1006
#define ID_BUTTON7  1007
#define ID_BUTTON8  1008
#define ID_BUTTON9  1009
#define ID_BUTTON10 1010
//Статические окна 
#define ID_STATIC1  2001
//Окна списков
#define ID_LISTBOX1 3001
#define ID_LISTBOX2 3002
#define ID_LISTBOX3 3003
#define ID_LISTBOX4 3004
//Окна текстовых редакторов
#define ID_EDIT1    4001

/**Идентификаторы возвращаемых окон*/
#define GET_D3DWINDOW			1
#define GET_OCCREATEOBJECT		2
#define GET_OBJECTLIST			3
#define GET_OBJECTNAMEEDITOR	4
#define GET_MAINWINDOW			5
#define GET_LIGHTOBJECTLIST     6
#define GET_NEWDIRLIGHTBUTTON	7
#define GET_NEWPOINTLIGHTBUTTON 8
#define GET_NEWSPOTLIGHTBUTTON	9
#define GET_CAMOBJECTLIST		10
#define GET_NEWCAMOBJECTBUTTON  11
#define GET_SUBSETSLIST			12


class window_class
{
private:
	HWND mainWindow, OCButton, COButton, CDLButton, CPLButton, CSLButton, CCButton, directXWindow;   //Окна, относящиеся к созданию объектов
	HWND objectsList, lightObjectsList, camObjectsList, subsetsList, showOList, showLList, showCList;   //Окна, относящиеся к спискам
	HWND OSButton1, OSEdit1;   //Редактирование названия
	LPCTSTR mainWindowName;	   //Название окна

public:
	//Инициализация окон GUI
	void initWindows(LPCTSTR windowName, LPCTSTR className, HINSTANCE hInstance, WNDPROC proc)
	{
		RegWndClass(proc, hInstance, className);   //Регистрируем собственный класс окна

		//Основное окно, имеющее собственный класс окна и обработчик сообщений
		mainWindow = CreateWindow(className, windowName, WS_OVERLAPPEDWINDOW|WS_VISIBLE, 
			0, 0, Width, Height, NULL, NULL, hInstance, NULL);

		//Кнопка, открывающая создатель/редактор объектов
		OCButton = CreateWindow(L"button", L"Object Creator", WS_CHILD|WS_VISIBLE|BS_CENTER, 
			Width * 0.01, 100, 190, 35, mainWindow, (HMENU)ID_BUTTON1, hInstance, NULL);

		//Кнопка, создающая новый объект
		COButton = CreateWindow(L"button", L"Create Object", WS_CHILD|BS_CENTER, 
			Width * 0.04, 135, 100, 20, mainWindow, (HMENU)ID_BUTTON2, hInstance, NULL);

		CDLButton = CreateWindow(L"button", L"Create Directional Light", WS_CHILD|BS_CENTER, 
			Width * 0.017, 175, 170, 20, mainWindow, (HMENU)ID_BUTTON4, hInstance, NULL);

		CPLButton = CreateWindow(L"button", L"Create Point Light", WS_CHILD|BS_CENTER, 
			Width * 0.017, 195, 170, 20, mainWindow, (HMENU)ID_BUTTON7, hInstance, NULL);

		CSLButton = CreateWindow(L"button", L"Create Spot Light", WS_CHILD|BS_CENTER, 
			Width * 0.017, 215, 170, 20, mainWindow, (HMENU)ID_BUTTON8, hInstance, NULL);

		CCButton = CreateWindow(L"button", L"Create Camera", WS_CHILD|BS_CENTER, 
			Width * 0.017, 155, 170, 20, mainWindow, (HMENU)ID_BUTTON9, hInstance, NULL);

		//Окно, подключаемое к DirectX
		directXWindow = CreateWindow(L"static", NULL, WS_CHILD|WS_BORDER, 
			DirectXLeft, DirectXTop, DirectXWidth, DirectXHeight, mainWindow, (HMENU)ID_STATIC1, hInstance, NULL);

		//Глобальный перечень всех созданных объектов
		int i = 100;
		int x = Height * 0.38;
		objectsList = CreateWindow(L"listbox", NULL, WS_CHILD|LBS_STANDARD, 
			Width - 240, i, 200, x, mainWindow, (HMENU)ID_LISTBOX1, hInstance, NULL);

		subsetsList = CreateWindow(L"listbox", NULL, WS_CHILD|LBS_STANDARD, 
			Width - 240, x + i, 200, x - 50, mainWindow, (HMENU)ID_LISTBOX4, hInstance, NULL);  

		lightObjectsList = CreateWindow(L"listbox", NULL, WS_CHILD|LBS_STANDARD, 
			Width - 240, 100, 200, Height - 195, mainWindow, (HMENU)ID_LISTBOX2, hInstance, NULL);

		camObjectsList = CreateWindow(L"listbox", NULL, WS_CHILD|LBS_STANDARD, 
			Width - 240, 100, 200, Height - 195, mainWindow, (HMENU)ID_LISTBOX3, hInstance, NULL);  


		//Текстовый редактор для редактирования названия объекта								//!!!!!!!!!!!!!!!!!
		OSEdit1 = CreateWindow(L"edit", NULL, WS_CHILD|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL, 
			35, 403, 150, 20, mainWindow, (HMENU)ID_EDIT1, hInstance, NULL);

		//Кнопка, применяющая строку в редакторе к названию объекта								//!!!!!!!!!!!!!!!!!
		OSButton1 = CreateWindow(L"button", L"Set New Name", WS_CHILD, 
			46, 424, 127, 20, mainWindow, (HMENU)ID_BUTTON3, hInstance, NULL);

		showOList    = CreateWindow(L"button", L"Objects", WS_CHILD|WS_VISIBLE, 
			Width - 240, 79, 70, 20, mainWindow, (HMENU)ID_BUTTON5, hInstance, NULL);

		showLList    = CreateWindow(L"button", L"Lighting", WS_CHILD|WS_VISIBLE, 
			Width - 170, 79, 70, 20, mainWindow, (HMENU)ID_BUTTON6, hInstance, NULL);

		showCList	 = CreateWindow(L"button", L"Cams", WS_CHILD|WS_VISIBLE, 
			Width - 100, 79, 70, 20, mainWindow, (HMENU)ID_BUTTON10, hInstance, NULL); 
	}

	UINT takeObjectFromList()
	{
		return ((UINT)SendMessage(objectsList, LB_GETCURSEL, 0, 0L) + 1);
	}

	UINT takeLightFromList()
	{
		return ((UINT)SendMessage(lightObjectsList, LB_GETCURSEL, 0, 0L) + 1);
	}

	UINT takeSubsetFromList()
	{
		return ((UINT)SendMessage(subsetsList, LB_GETCURSEL, 0, 0L));
	}

	UINT takeCamFromList()
	{
		return ((UINT)SendMessage(camObjectsList, LB_GETCURSEL, 0, 0L) + 1);
	}

	HWND getWindowHandle(int windowType)
	{
		switch(windowType)
		{
		case GET_D3DWINDOW:
			return directXWindow;
		case GET_OCCREATEOBJECT:
			return COButton;
		case GET_OBJECTLIST:
			return objectsList;
		case GET_LIGHTOBJECTLIST:
			return lightObjectsList;
		case GET_OBJECTNAMEEDITOR:
			return OSEdit1;
		case GET_MAINWINDOW:
			return mainWindow;
		case GET_NEWDIRLIGHTBUTTON:
			return CDLButton;
		case GET_NEWPOINTLIGHTBUTTON:
			return CPLButton;
		case GET_NEWSPOTLIGHTBUTTON:
			return CSLButton;
		case GET_CAMOBJECTLIST:
			return camObjectsList;
		case GET_NEWCAMOBJECTBUTTON:
			return CCButton;
		case GET_SUBSETSLIST:
			return subsetsList;
		}
	}
};


void RegWndClass(WNDPROC proc, HINSTANCE hInstance, LPCTSTR className)
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = proc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground =
		static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName  = 0;
	wc.lpszClassName = className;

	if(!RegisterClass(&wc))
		MessageBox(0, L"RegisterClass - Failed", 0, 0);
}