#include "Global_Linker.h"

LPCTSTR  className  = L"C.O.M.E. Engine"; 
LPCTSTR  windowName = L"C.O.M.E. Engine, Build 1.006.16";

MSG  msg;
IDirect3DDevice9* device;
HWND objectCreatorWindow, COButton;

char buffer[256];

object_manager* manager;		   //�������� ��������
object_creator* OC;				   //��������� � �������� ��������
window_class* windows;			   //Windows-���� (GUI)
object_settings* objectSettings;   //���� �������� ��������

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	windows = new window_class;	
	windows->initWindows(windowName, className, hInstance, MainProc);	//�������� �������� GUI

	directx9_class device1(windows->getWindowHandle(GET_D3DWINDOW), hInstance);		//����������� ����� ������ DirectX � ����

	device				= device1.getDevice();				//��������� ���������� ����������
	objectCreatorWindow = windows->getWindowHandle(GET_D3DWINDOW);		//��������� ����������� ����, � ������� ���� ����� DirectX
	COButton			= windows->getWindowHandle(GET_OCCREATEOBJECT);		//��������� ����������� ������, ��������� ����� ������

	manager = new object_manager;	//�������� ������ �� �������� ��������
	manager->initManager(device);	//�������������� �������� ��������
	
	OC = new object_creator;		//�������� ������ �� �������� ��������
	OC->initObjectCreator(device, objectCreatorWindow, hInstance, manager);	//�������������� �������� ��������

	objectSettings = new object_settings;
	objectSettings->initSettings(windows->getWindowHandle(GET_MAINWINDOW), hInstance);
	objectSettings->initObjectSettings();
	objectSettings->initLightSettings();

	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	device->SetRenderState(D3DRS_SPECULARENABLE, true);
	
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
				0x00000000, 1.0f, 0);
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
			if(IsWindowVisible(objectCreatorWindow))		//����� �� ����?
			{
				ShowWindow(objectCreatorWindow, SW_HIDE);	//��  - ��������
				ShowWindow(COButton, SW_HIDE);
				ShowWindow(windows->getWindowHandle(GET_NEWDIRLIGHTBUTTON), SW_HIDE);
				ShowWindow(windows->getWindowHandle(GET_NEWPOINTLIGHTBUTTON), SW_HIDE);
			}
			else
			{
				ShowWindow(objectCreatorWindow, SW_NORMAL); //��� - ��������
				ShowWindow(COButton, SW_NORMAL);
				ShowWindow(windows->getWindowHandle(GET_NEWDIRLIGHTBUTTON), SW_NORMAL);
				ShowWindow(windows->getWindowHandle(GET_NEWPOINTLIGHTBUTTON), SW_NORMAL);
			}
			return 0;
		case ID_BUTTON2:
			//��������� ������ ��������� ���������� � ����������� � ���� ��������
			OC->pickObject(manager->createNewObject(windows->getWindowHandle(GET_OBJECTLIST)));
	
			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_HIDE);							
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_NORMAL);

			objectSettings->showOSButton(SW_NORMAL);
			objectSettings->fillObjectSettings(OC->getMaterialClass());
			return 0;

		case ID_BUTTON4:	
			OC->pickLight(manager->createNewDirectionLight(windows->getWindowHandle(GET_LIGHTOBJECTLIST)));
			
			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_NORMAL);							
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_HIDE);

			objectSettings->showLSButton(SW_NORMAL);
			objectSettings->fillLightSettings(OC->getLightStruct());
			return 0;

		case ID_LISTBOX1:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				//������, ��������� �� ����� ��������, ��������� ���������� 
				OC->pickObject(windows->takeObjectFromList());
				objectSettings->showOSButton(SW_NORMAL);
				objectSettings->fillObjectSettings(OC->getMaterialClass());
				return 0;
			default:
				return 0;
			}
			return 0;
		case ID_LISTBOX2:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				//������ �����, ��������� �� ����� �������� �����, ��������� ���������� 
				OC->pickLight(windows->takeLightFromList());
				manager->setOnlyPickedLight(OC->getPickedLight());	//� � ����������� ��������������� ������ ��
				objectSettings->showLSButton(SW_NORMAL);
				objectSettings->fillLightSettings(OC->getLightStruct());
				return 0;
			default:
				return 0;
			}
			return 0;
		case ID_BUTTON3:
			MessageBox(windows->getWindowHandle(GET_D3DWINDOW), L"THIS SHIT DOESN'T WORK!", L"STAAAPH", MB_ICONSTOP);
			//� ���������� � ������ ������ ������� ���������� ��������� /*�� ��������*/
			//OC->renameObject(windows->getWindowHandle(GET_OBJECTLIST), windows->getWindowHandle(GET_OBJECTNAMEEDITOR));
			SetFocus(windows->getWindowHandle(GET_D3DWINDOW));
			return 0;
		case ID_BUTTON5:
			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_HIDE);							
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_NORMAL);
			return 0;
		case ID_BUTTON6:	
			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_NORMAL);							
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_HIDE);
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
		wsprintf (textBuffer, L"pX: %d pY: %d", pOc.x, pOc.y);
		SetWindowText (windows->getWindowHandle(GET_MAINWINDOW), textBuffer);
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
/*				         � ALL COPYRIGHTS RESERVED	                    */
/************************************************************************/
/************************************************************************/
/*				         � ��� ��������� ��������                       */
/************************************************************************/
/************************************************************************/
/*				         � ��� ����� ���������   						*/
/************************************************************************/
/************************************************************************/
/*				         � VSE PRAVA SOHRANENI                          */
/************************************************************************/
