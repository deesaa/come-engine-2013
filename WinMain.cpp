#include "Global_Linker.h"

LPCTSTR  className  = L"C-O.M.E. Engine"; 
LPCTSTR  windowName = L"C-O.M.E. Engine, Build 1.6.23";

MSG  msg;
IDirect3DDevice9* device;
HWND objectCreatorWindow, COButton;

char buffer[256];

object_manager* manager;		   //Менеджер объектов
object_creator* OC;				   //Создатель и редактор объектов
window_class* windows;			   //Windows-окна (GUI)
object_settings* objectSettings;   //Окна настроек объектов

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

	objectSettings = new object_settings;
	objectSettings->initSettings(windows->getWindowHandle(GET_MAINWINDOW), hInstance);
	objectSettings->initObjectSettings();
	objectSettings->initLightSettings();

	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	device->SetRenderState(D3DRS_SPECULARENABLE, true);
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


	static float lastTime = (float)timeGetTime();
	float currTime, timeDelta;
	manager->setPointerToTimeDelta(&timeDelta);

	manager->createNewCam(windows->getWindowHandle(GET_CAMOBJECTLIST));

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
			currTime = (float)timeGetTime();
			timeDelta = (currTime - lastTime) * 0.001f;
	
			device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
				0x00000000, 1.0f, 0);

			device->BeginScene();

			OC->applyKBMChanges();
			
			manager->redrawAllObjects();
			manager->redrawObjectOfLight(OC->getPickedLight());
				
			device->EndScene();
			device->Present(0, 0, 0, 0); 

			lastTime = currTime;
		}
	}

	delete manager;
	delete OC;
	delete windows;
	delete objectSettings;
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
				ShowWindow(windows->getWindowHandle(GET_NEWDIRLIGHTBUTTON), SW_HIDE);
				ShowWindow(windows->getWindowHandle(GET_NEWPOINTLIGHTBUTTON), SW_HIDE);
				ShowWindow(windows->getWindowHandle(GET_NEWSPOTLIGHTBUTTON), SW_HIDE);
				ShowWindow(windows->getWindowHandle(GET_NEWCAMOBJECTBUTTON), SW_HIDE);
			}
			else
			{
				ShowWindow(objectCreatorWindow, SW_NORMAL); //Нет - показать
				ShowWindow(COButton, SW_NORMAL);
				ShowWindow(windows->getWindowHandle(GET_NEWDIRLIGHTBUTTON), SW_NORMAL);
				ShowWindow(windows->getWindowHandle(GET_NEWPOINTLIGHTBUTTON), SW_NORMAL);
				ShowWindow(windows->getWindowHandle(GET_NEWSPOTLIGHTBUTTON), SW_NORMAL);
				ShowWindow(windows->getWindowHandle(GET_NEWCAMOBJECTBUTTON), SW_NORMAL);
			}
			return 0;
		case ID_BUTTON2:
			//Созданный объект принимает управление и добавляется в лист объектов
			OC->pickObject(manager->createNewObject(windows->getWindowHandle(GET_OBJECTLIST)));
	
			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_HIDE);	
			ShowWindow(windows->getWindowHandle(GET_CAMOBJECTLIST), SW_HIDE);	
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_NORMAL);

			objectSettings->showOSButton(SW_NORMAL);
			objectSettings->fillObjectSettings(OC->getMaterialClass());
			return 0;

		case ID_BUTTON4:	
			OC->pickLight(manager->createNewDirectionLight(windows->getWindowHandle(GET_LIGHTOBJECTLIST)));
			
			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_NORMAL);							
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_CAMOBJECTLIST), SW_HIDE);	

			objectSettings->showLSButton(SW_NORMAL);
			objectSettings->fillLightSettings(OC->getLightStruct());
			return 0;

		case ID_BUTTON7:
			OC->pickLight(manager->createNewPointLight(windows->getWindowHandle(GET_LIGHTOBJECTLIST)));

			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_NORMAL);							
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_CAMOBJECTLIST), SW_HIDE);	

			objectSettings->showLSButton(SW_NORMAL);
			objectSettings->fillLightSettings(OC->getLightStruct());
			return 0;

		case ID_BUTTON8:
			OC->pickLight(manager->createNewSpotLight(windows->getWindowHandle(GET_LIGHTOBJECTLIST)));

			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_NORMAL);							
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_CAMOBJECTLIST), SW_HIDE);	

			objectSettings->showLSButton(SW_NORMAL);
			objectSettings->fillLightSettings(OC->getLightStruct());
			return 0;

		case ID_BUTTON9:
			OC->pickCam(manager->createNewCam(windows->getWindowHandle(GET_CAMOBJECTLIST)));

			ShowWindow(windows->getWindowHandle(GET_CAMOBJECTLIST), SW_NORMAL);							
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_HIDE);
			return 0;

		case ID_LISTBOX1:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				//Объект, выбранный из листа объектов, принимает управление 
				OC->pickObject(windows->takeObjectFromList());
				objectSettings->showOSButton(SW_NORMAL);
				objectSettings->fillObjectSettings(OC->getMaterialClass());
				return 0;
			}
			return 0;
		case ID_LISTBOX2:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				//Объект света, выбранный из листа объектов света, принимает управление 
				OC->pickLight(windows->takeLightFromList());
				manager->setOnlyPickedLight(OC->getPickedLight());	//И в действующие устанавливается только он
				objectSettings->showLSButton(SW_NORMAL);
				objectSettings->fillLightSettings(OC->getLightStruct());
				return 0;
			}
			return 0;

		case ID_LISTBOX3:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				//Объект света, выбранный из листа объектов света, принимает управление 
				OC->pickCam(windows->takeCamFromList());
				manager->resetCam(OC->getPickedCam());
				return 0;
			}
			return 0;

		case ID_BUTTON3:
			MessageBox(windows->getWindowHandle(GET_D3DWINDOW), L"Error because I said so!", L"Staaaph", MB_ICONSTOP);
			//У выбранного в данный момент объекта изменяется навзвание /*НЕ РАБОТАЕТ*/
			//OC->renameObject(windows->getWindowHandle(GET_OBJECTLIST), windows->getWindowHandle(GET_OBJECTNAMEEDITOR));
			SetFocus(windows->getWindowHandle(GET_D3DWINDOW));
			return 0;
		case ID_BUTTON5:
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_NORMAL);
			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_HIDE);	
			ShowWindow(windows->getWindowHandle(GET_CAMOBJECTLIST), SW_HIDE);		
			return 0;
		case ID_BUTTON6:	
			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_NORMAL);							
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_CAMOBJECTLIST), SW_HIDE);
			return 0;
		case ID_BUTTON10:
			ShowWindow(windows->getWindowHandle(GET_CAMOBJECTLIST), SW_NORMAL);							
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_HIDE);
			return 0;
		case ID_OSBUTTON1:
			objectSettings->showOSettingsWnd();
			return 0;
		case ID_OSBUTTON2:
			objectSettings->showLSettingsWnd();
			return 0;
		case ID_EDIT1:
			return 0;
		}
	case WM_LBUTTONDOWN:
		POINT pOc;
		TCHAR textBuffer[32];
		GetCursorPos(&pOc);
		ScreenToClient(windows->getWindowHandle(GET_D3DWINDOW), &pOc);
		OC->createRayOfClick(pOc);

		if(OC->pickIntersectedObject())
		{
			objectSettings->showOSButton(SW_NORMAL);
			objectSettings->fillObjectSettings(OC->getMaterialClass());
		}
		if(OC->pickIntersectedVertex())
		{
			objectSettings->showOSButton(SW_NORMAL);
			objectSettings->fillObjectSettings(OC->getMaterialClass());
		}

		if(OC->checkPickType(Cam))
		{
			RECT rect;
			rect.top	= (Height*0.5f)-30;
			rect.left	= (Width*0.5f)-30;
			rect.bottom = (Height*0.5f)+30;
			rect.right	= (Width*0.5f)+30;
			ClipCursor(&rect);
			ShowCursor(FALSE);
		}
		return 0;
	case WM_LBUTTONUP:
		if(OC->checkPickType(Cam))
		{
			RECT rect;
			rect.top	= 0;
			rect.left	= 0;
			rect.bottom = Height;
			rect.right	= Width;
			ClipCursor(&rect);
			ShowCursor(TRUE);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

LRESULT CALLBACK OSDlgWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_EOBUTTON1:
			objectSettings->applyEOChanges();
			return 0;
		case ID_EOCEDIT1:
			objectSettings->editODiffuseColor();
			return 0;
		case ID_EOCEDIT2:
			objectSettings->editOAmbientColor();
			return 0;
		case ID_EOCEDIT3:
			objectSettings->editOSpecularColor();
			return 0;
		case ID_EOCEDIT4:
			objectSettings->editOEmissiveColor();
			return 0;
		case IDCANCEL:
			EndDialog(hwnd, TRUE);
			return 0;
		}
	}
	return 0;
}

LRESULT CALLBACK LSDlgWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_ELBUTTON1:
			objectSettings->applyELChanges();
			manager->resetLight(OC->getPickedLight());
			return 0;
		case ID_ELCEDIT1:
			objectSettings->editLDiffuseColor();
			manager->resetLight(OC->getPickedLight());
			return 0;
		case ID_ELCEDIT2:
			objectSettings->editLAmbientColor();
			manager->resetLight(OC->getPickedLight());
			return 0;
		case ID_ELCEDIT3:
			objectSettings->editLSpecularColor();
			manager->resetLight(OC->getPickedLight());
			return 0;
		case IDCANCEL:
			EndDialog(hwnd, TRUE);
			return 0;
		}
	}
	return 0;
}

LRESULT CALLBACK CEDlgWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hwnd, TRUE);
			return 0;
		}
		return 0;
	}
	return 0;
}

/************************************************************************/
/*				         © ALL COPYRIGHTS RESERVED	                    */
/************************************************************************/
/************************************************************************/
/*				         © АЛЛ КАПИРАЙТС РЕСЕРВЕД                       */
/************************************************************************/
/************************************************************************/
/*				         © ВСЕ ПРАВА СОХРАНЕНЫ   						*/
/************************************************************************/
/************************************************************************/
/*				         © VSE PRAVA SOHRANENI                          */
/************************************************************************/
