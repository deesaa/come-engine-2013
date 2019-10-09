#include "Global_Linker.h"

LPCTSTR  className  = L"C.O.M.E. Engine"; 
LPCTSTR  windowName = L"C.O.M.E. Engine, Build 1.006";

MSG  msg;
IDirect3DDevice9* device;
HWND objectCreatorWindow;

char buffer[256];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	window_class windows(windowName, className, hInstance, MainProc);			//Создания базового GUI

	directx9_class device1(windows.getWindowHandle(), hInstance);				//Подключение точки вывода DirectX к окну

	device = device1.getDevice();							//Вывод выше сделанных объектов в глобальную видимость 
	objectCreatorWindow = windows.getWindowHandle();		//Получение дескриптора окна, в которое идет вывод DirectX

	object_creator OC(device, objectCreatorWindow, hInstance);
	
	object_manager manager(device);
	manager.createNewObject();
	
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

			OC.applyKBChanges();
			OC.redraw();
			manager.redrawObject(0);
				
			device->EndScene();
			device->Present(0, 0, 0, 0); 
		}
	}
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
				ShowWindow(objectCreatorWindow, SW_HIDE);	//Да  - спрятать
			else
				ShowWindow(objectCreatorWindow, SW_NORMAL); //Нет - показать
			return 0;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
