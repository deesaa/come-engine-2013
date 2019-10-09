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
//Статические окна 
#define ID_STATIC1  2001
//Окна списков
#define ID_LISTBOX1 3001
#define ID_LISTBOX2 3002
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

class window_class
{
private:
	HWND mainWindow, OCButton, COButton, CDLButton, CPLButton, CSLButton, directXWindow;   //Окна, относящиеся к созданию объектов
	HWND objectsList, lightObjectsList, showOList, showLList;   //Окна, относящиеся к спискам
	HWND OSButton1, OSEdit1;   //Окна, относящиеся к редактированию объектов
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
			12, 100, 190, 35, mainWindow, (HMENU)ID_BUTTON1, hInstance, NULL);

		//Кнопка, создающая новый объект
		COButton = CreateWindow(L"button", L"Create Object", WS_CHILD|BS_CENTER, 
			61, 135, 100, 20, mainWindow, (HMENU)ID_BUTTON2, hInstance, NULL);

		CDLButton = CreateWindow(L"button", L"Create Directional Light", WS_CHILD|BS_CENTER, 
			26, 155, 170, 20, mainWindow, (HMENU)ID_BUTTON4, hInstance, NULL);

		CPLButton = CreateWindow(L"button", L"Create Point Light", WS_CHILD|BS_CENTER, 
			26, 175, 170, 20, mainWindow, (HMENU)ID_BUTTON7, hInstance, NULL);

		CSLButton = CreateWindow(L"button", L"Create Spot Light", WS_CHILD|BS_CENTER, 
			26, 195, 170, 20, mainWindow, (HMENU)ID_BUTTON8, hInstance, NULL);

		//Окно, подключаемое к DirectX
		directXWindow = CreateWindow(L"static", NULL, WS_CHILD|WS_BORDER, 
			217, 100, DirectXWidth, DirectXHeight, mainWindow, (HMENU)ID_STATIC1, hInstance, NULL);

		//Глобальный перечень всех созданных объектов
		objectsList = CreateWindow(L"listbox", NULL, WS_CHILD|LBS_STANDARD|WS_VISIBLE, 
			1360, 100, 200, 705, mainWindow, (HMENU)ID_LISTBOX1, hInstance, NULL);

		lightObjectsList = CreateWindow(L"listbox", NULL, WS_CHILD|LBS_STANDARD|WS_VISIBLE, 
			1360, 100, 200, 705, mainWindow, (HMENU)ID_LISTBOX2, hInstance, NULL);

		//Текстовый редактор для редактирования названия объекта
		OSEdit1 = CreateWindow(L"edit", NULL, WS_CHILD|WS_BORDER|WS_VISIBLE|ES_LEFT|ES_AUTOHSCROLL, 
			35, 403, 150, 20, mainWindow, (HMENU)ID_EDIT1, hInstance, NULL);

		//Кнопка, применяющая строку в редакторе к названию объекта
		OSButton1 = CreateWindow(L"button", L"Set New Name", WS_CHILD|WS_VISIBLE, 
			46, 424, 127, 20, mainWindow, (HMENU)ID_BUTTON3, hInstance, NULL);

		showOList    = CreateWindow(L"button", L"Objects", WS_CHILD|WS_VISIBLE, 
			1360, 79, 70, 20, mainWindow, (HMENU)ID_BUTTON5, hInstance, NULL);

		showLList    = CreateWindow(L"button", L"Lighting", WS_CHILD|WS_VISIBLE, 
			1430, 79, 70, 20, mainWindow, (HMENU)ID_BUTTON6, hInstance, NULL);
	}

	UINT takeObjectFromList()
	{
		return ((UINT)SendMessage(objectsList, LB_GETCURSEL, 0, 0L) + 1);
	}

	UINT takeLightFromList()
	{
		return ((UINT)SendMessage(lightObjectsList, LB_GETCURSEL, 0, 0L) + 1);
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