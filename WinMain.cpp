#include "Global_Linker.h"

LPCTSTR  className  = L"C.O.M.E. Engine"; 
LPCTSTR  windowName = L"C.O.M.E. Engine, Build 1.006";

MSG  msg;
IDirect3DDevice9* device;
HWND objectCreatorWindow, COButton;

char buffer[256];

object_manager* manager;	//Менеджер объектов
object_creator* OC;			//Создатель и редактор объектов
window_class* windows;		//Windows-окна (GUI)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	windows = new window_class;	
	windows->initWindows(windowName, className, hInstance, MainProc);	//Создания базового GUI

	directx9_class device1(windows->getWindowHandle(GET_D3DWINDOW), hInstance);		//Подключение точки вывода DirectX к окну

	device				= device1.getDevice();				//Получение устройства видеокарты
	objectCreatorWindow = windows->getWindowHandle(GET_D3DWINDOW);		//Получение дескриптора окна, в которое идет вывод DirectX
	COButton			= windows->getWindowHandle(GET_OCCREATEOBJECT);		//Получение дескриптора кнопки, создающей новый объект

	manager = new object_manager;	//Выделяем память на менеджер объектов
	manager->initManager(device);	//Инициализируем менеджер объектов
	
	OC = new object_creator;		//Выделяем память на редактор объектов
	OC->initObjectCreator(device, objectCreatorWindow, hInstance, manager);	//Инициализируем редактор объектов

	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	
	while(true)
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
				0x0000fff0, 1.0f, 0);
			device->BeginScene();

			OC->applyKBMChanges();
			OC->redraw();
			manager->redrawAllObjects();
				
			device->EndScene();
			device->Present(0, 0, 0, 0); 
		}
	}
	delete manager;
	delete OC;
	return msg.wParam;
}

HRESULT CALLBACK MainProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_BUTTON1:
			if(IsWindowVisible(objectCreatorWindow))		//Видно ли окно?
			{
				ShowWindow(objectCreatorWindow, SW_HIDE);	//Да  - спрятать
				ShowWindow(COButton, SW_HIDE);
			}
			else
			{
				ShowWindow(objectCreatorWindow, SW_NORMAL); //Нет - показать
				ShowWindow(COButton, SW_NORMAL);
			}
			return 0;
		case ID_BUTTON2:
			//Созданный объект принимает управление и добавляется в лист объектов
			OC->pickObject(manager->createNewObject(windows->getWindowHandle(GET_OBJECTLIST)));
			return 0;
		case ID_LISTBOX1:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				//Объект, выбранный из листа объектов, принимает управление 
				OC->pickObject(windows->takeObjectFromList());
				return 0;
			default:
				return 0;
			}
			return 0;
		case ID_BUTTON3:
			MessageBox(windows->getWindowHandle(GET_D3DWINDOW), L"THIS SHIT DOESN'T WORK!", L"STAAAPH", MB_ICONSTOP);
			//У выбранного в данный момент объекта изменяется навзвание /*НЕ РАБОТАЕТ*/
			//OC->renameObject(windows->getWindowHandle(GET_OBJECTLIST), windows->getWindowHandle(GET_OBJECTNAMEEDITOR));
			return 0;
		case ID_EDIT1:
			return 0;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}


/************************************************************************/
/*				         © ALL COPYRIGHTS RESERVED	                    */
/************************************************************************/
/************************************************************************/
/*				         © АЛЛ КАПИРАЙТС РЕСЕРВЕД                       */
/************************************************************************/
