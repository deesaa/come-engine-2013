/**Идентификаторы для второстепенных окон**/
//Кнопки
#define ID_BUTTON1  1001
#define ID_BUTTON2  1002
#define ID_BUTTON3  1003
//Статические окна 
#define ID_STATIC1  2001
//Окна списков
#define ID_LISTBOX1 3001
//Окна текстовых редакторов
#define ID_EDIT1    4001

/**Идентификаторы возвращаемых окон*/
#define GET_D3DWINDOW			1
#define GET_OCCREATEOBJECT		2
#define GET_OBJECTLIST			3
#define GET_OBJECTNAMEEDITOR	4

class window_class
{
private:
	HWND mainWindow, OCButton, COButton, directXWindow;   //Окна, относящиеся к созданию объекта
	HWND objectsList;   //Список, содержащий созданные объекты
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
		OCButton   = CreateWindow(L"button", L"Object Creator", WS_CHILD|WS_VISIBLE|BS_CENTER, 
			12, 100, 190, 35, mainWindow, (HMENU)ID_BUTTON1, hInstance, NULL);

		//Кнопка, создающая новый объект
		COButton   = CreateWindow(L"button", L"Create Object", WS_CHILD|BS_CENTER, 
			102, 135, 100, 17, mainWindow, (HMENU)ID_BUTTON2, hInstance, NULL);

		//Окно, подключаемое к DirectX
		directXWindow = CreateWindow(L"static", NULL, WS_CHILD|WS_BORDER, 
			217, 100, DirectXWidth, DirectXHeight, mainWindow, (HMENU)ID_STATIC1, hInstance, NULL);

		//Глобальный перечень всех созданных объектов
		objectsList = CreateWindow(L"listbox", NULL, WS_CHILD|LBS_STANDARD|WS_VISIBLE, 
			1360, 100, 200, 705, mainWindow, (HMENU)ID_LISTBOX1, hInstance, NULL);

		//Текстовый редактор для редактирования названия объекта
		OSEdit1 = CreateWindow(L"edit", NULL, WS_CHILD|WS_BORDER|WS_VISIBLE|ES_LEFT|ES_AUTOHSCROLL, 
			35, 403, 150, 20, mainWindow, (HMENU)ID_EDIT1, hInstance, NULL);

		//Кнопка, применяющая строку в редакторе к названию объекта
		OSButton1 = CreateWindow(L"button", L"Set New Name", WS_CHILD|WS_VISIBLE, 
			46, 424, 127, 20, mainWindow, (HMENU)ID_BUTTON3, hInstance, NULL);
	}

	UINT takeObjectFromList()
	{
		return ((UINT)SendMessage(objectsList, LB_GETCURSEL, 0, 0L) + 1);
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
		case GET_OBJECTNAMEEDITOR:
			return OSEdit1;
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
	{
		MessageBox(0, L"RegisterClass - Failed", 0, 0);
	}
}