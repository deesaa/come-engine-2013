#include "Global_Linker.h"

LPCTSTR  className  = L"C-O.M.E. Engine"; 
LPCTSTR  windowName = L"C-O.M.E. Engine, Build 1.6.35";

MSG  msg;
IDirect3DDevice9* device;
HWND objectCreatorWindow, COButton;

char buffer[256];

object_manager* manager;		   //�������� ��������
object_creator* OC;				   //��������� � �������� ��������
window_class* windows;			   //Windows-���� (GUI)
mainMenu_class* mainMenu;
object_settings* objectSettings;   //���� �������� ��������
botStatusBar_Class* BStatBar;
topEditingBar_Class* TEditingBar;
rendStateTypes_class* rendStateTypes;
rendStateEditor_class* rendStateEditor;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	windows = new window_class;	
	windows->initWindows(windowName, className, hInstance, MainProc);	//�������� �������� GUI

	directx9_class device1(windows->getWindowHandle(GET_D3DWINDOW), hInstance);		//����������� ����� ������ DirectX � ����

	device				= device1.getDevice();				//��������� ���������� ����������
	objectCreatorWindow = windows->getWindowHandle(GET_D3DWINDOW);		//��������� ����������� ����, � ������� ���� ����� DirectX
	COButton			= windows->getWindowHandle(GET_OCCREATEOBJECT);		//��������� ����������� ������, ��������� ����� ������
	
	mainMenu = new mainMenu_class;
	mainMenu->initMainMenu(windows->getWindowHandle(GET_MAINWINDOW));

	objectSettings = new object_settings;
	objectSettings->initSettings(windows->getWindowHandle(GET_MAINWINDOW), hInstance);
	objectSettings->initObjectSettings();
	objectSettings->initLightSettings();

	int Style;
	Style = GetWindowLongPtr(windows->getWindowHandle(GET_LIGHTOBJECTLIST), GWL_STYLE);
	Style = Style | WS_CAPTION;
	Style = Style | WS_SIZEBOX;
	SetWindowLongPtr(windows->getWindowHandle(GET_LIGHTOBJECTLIST), GWL_STYLE, Style);


	BStatBar = new botStatusBar_Class;
	BStatBar->initBStatBar(windows->getWindowHandle(GET_MAINWINDOW), hInstance);

	TEditingBar = new topEditingBar_Class;
	TEditingBar->initTEBar(windows->getWindowHandle(GET_MAINWINDOW), hInstance);

	rendStateTypes = new rendStateTypes_class;
	rendStateTypes->initRendStateStructs();

	rendStateEditor = new rendStateEditor_class;
	rendStateEditor->initRendStateEditor(windows->getWindowHandle(GET_MAINWINDOW), hInstance, rendStateTypes);

	manager = new object_manager;	//�������� ������ �� �������� ��������
	manager->initManager(device, hInstance, windows->getWindowHandle(GET_MAINWINDOW), 
		BStatBar, TEditingBar, rendStateTypes, windows);	//�������������� �������� ��������

	OC = new object_creator;		//�������� ������ �� �������� ��������
	OC->initObjectCreator(device, windows->getWindowHandle(GET_MAINWINDOW), hInstance, manager, objectSettings, BStatBar, rendStateEditor);	//�������������� �������� ��������

	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	device->SetRenderState(D3DRS_SPECULARENABLE, true);
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	static float lastTime = (float)timeGetTime();
	float currTime, timeDelta;
	manager->setPointerToTimeDelta(&timeDelta);

	manager->createNewCam(windows->getWindowHandle(GET_CAMOBJECTLIST));

	/*HANDLE FileHandle;
	OFSTRUCT fileInfo;
	fileInfo.szPathName;
	DWORD num;
	FileHandle = (HANDLE)OpenFile("Settings.ini", &fileInfo, OF_READ| OF_SHARE_DENY_READ);
	char Buf[128];
	ZeroMemory(Buf, sizeof(Buf));
	//if(FileHandle)
	//{
		ReadFile(FileHandle, Buf, sizeof(Buf), &num, NULL);
		CloseHandle(FileHandle);
	//} */

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
	delete mainMenu;
	delete windows;
	delete objectSettings;
	delete BStatBar;
	delete TEditingBar;
	delete rendStateEditor;
	delete rendStateTypes;
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
				ShowWindow(windows->getWindowHandle(GET_NEWSPOTLIGHTBUTTON), SW_HIDE);
				ShowWindow(windows->getWindowHandle(GET_NEWCAMOBJECTBUTTON), SW_HIDE);
				BStatBar->hideBar();
				TEditingBar->hideBar();
			}
			else
			{
				ShowWindow(objectCreatorWindow, SW_NORMAL); //��� - ��������
				ShowWindow(COButton, SW_NORMAL);
				ShowWindow(windows->getWindowHandle(GET_NEWDIRLIGHTBUTTON), SW_NORMAL);
				ShowWindow(windows->getWindowHandle(GET_NEWPOINTLIGHTBUTTON), SW_NORMAL);
				ShowWindow(windows->getWindowHandle(GET_NEWSPOTLIGHTBUTTON), SW_NORMAL);
				ShowWindow(windows->getWindowHandle(GET_NEWCAMOBJECTBUTTON), SW_NORMAL);
				BStatBar->showBar();
				TEditingBar->showBar();
			}
			return 0;
		case ID_TEBBUTTON1:
			OC->xAxis();
			return 0;
		case ID_TEBBUTTON2:
			OC->yAxis();
			return 0;
		case ID_TEBBUTTON3:
			OC->zAxis();
			return 0;
		case ID_BUTTON2:
			//��������� ������ ��������� ���������� � ����������� � ���� ��������
			OC->pickObject(manager->createNewObject(windows->getWindowHandle(GET_OBJECTLIST), windows->getWindowHandle(GET_SUBSETSLIST), false));
	
			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_HIDE);	
			ShowWindow(windows->getWindowHandle(GET_CAMOBJECTLIST), SW_HIDE);	
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_NORMAL);
			ShowWindow(windows->getWindowHandle(GET_SUBSETSLIST), SW_NORMAL);
			rendStateEditor->showRSSettingsButton();

			objectSettings->showOSButton(SW_NORMAL);
			if(OC->getMaterialClass())
				objectSettings->fillObjectSettings(OC->getMaterialClass());

			rendStateEditor->fillRSEditor(OC->getRendState());

			return 0;

		case ID_BUTTON4:	
			OC->pickLight(manager->createNewDirectionLight(windows->getWindowHandle(GET_LIGHTOBJECTLIST)));
			
			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_NORMAL);							
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_CAMOBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_SUBSETSLIST), SW_HIDE);
			rendStateEditor->hideRSSettingsButton();

			objectSettings->showLSButton(SW_NORMAL);
			objectSettings->fillLightSettings(OC->getLightStruct());
			return 0;

		case ID_BUTTON7:
			OC->pickLight(manager->createNewPointLight(windows->getWindowHandle(GET_LIGHTOBJECTLIST)));

			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_NORMAL);							
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_CAMOBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_SUBSETSLIST), SW_HIDE);
			rendStateEditor->hideRSSettingsButton();

			objectSettings->showLSButton(SW_NORMAL);
			objectSettings->fillLightSettings(OC->getLightStruct());
			return 0;

		case ID_BUTTON8:
			OC->pickLight(manager->createNewSpotLight(windows->getWindowHandle(GET_LIGHTOBJECTLIST)));

			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_NORMAL);							
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_CAMOBJECTLIST), SW_HIDE);	
			ShowWindow(windows->getWindowHandle(GET_SUBSETSLIST), SW_HIDE);
			rendStateEditor->hideRSSettingsButton();

			objectSettings->showLSButton(SW_NORMAL);
			objectSettings->fillLightSettings(OC->getLightStruct());
			return 0;

		case ID_BUTTON9:
			OC->pickCam(manager->createNewCam(windows->getWindowHandle(GET_CAMOBJECTLIST)));

			ShowWindow(windows->getWindowHandle(GET_CAMOBJECTLIST), SW_NORMAL);							
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_SUBSETSLIST), SW_HIDE);
			rendStateEditor->hideRSSettingsButton();
			objectSettings->showOSButton(SW_HIDE);
			objectSettings->showLSButton(SW_HIDE);
			return 0;

		case ID_LISTBOX1:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				//������, ��������� �� ����� ��������, ��������� ���������� 
				OC->pickObject(windows->takeObjectFromList());
				objectSettings->showOSButton(SW_NORMAL);
				rendStateEditor->showRSSettingsButton();
				if(OC->getMaterialClass())
					objectSettings->fillObjectSettings(OC->getMaterialClass());
				rendStateEditor->fillRSEditor(OC->getRendState());
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
				objectSettings->showOSButton(SW_HIDE);
				objectSettings->showLSButton(SW_NORMAL);
				rendStateEditor->hideRSSettingsButton();
				objectSettings->fillLightSettings(OC->getLightStruct());
				return 0;
			}
			return 0;

		case ID_LISTBOX3:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				//������ �����, ��������� �� ����� �������� �����, ��������� ���������� 
				OC->pickCam(windows->takeCamFromList());
				objectSettings->showOSButton(SW_HIDE);
				objectSettings->showLSButton(SW_HIDE);
				manager->resetCam(OC->getPickedCam());
				return 0;
			}
			return 0;

		case ID_LISTBOX4: 
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				manager->pickSubset(OC->getPickedObject(), windows->takeSubsetFromList());
				return 0;
			}
			return 0;
		
		case ID_BUTTON3:
			MessageBox(windows->getWindowHandle(GET_D3DWINDOW), L"Error because I said so!", L"Staaaph", MB_ICONSTOP);
			//� ���������� � ������ ������ ������� ���������� ��������� /*�� ��������*/
			//OC->renameObject(windows->getWindowHandle(GET_OBJECTLIST), windows->getWindowHandle(GET_OBJECTNAMEEDITOR));
			SetFocus(windows->getWindowHandle(GET_D3DWINDOW));
			return 0;
		case ID_BUTTON5:
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_NORMAL);
			ShowWindow(windows->getWindowHandle(GET_SUBSETSLIST), SW_NORMAL);
			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_HIDE);	
			ShowWindow(windows->getWindowHandle(GET_CAMOBJECTLIST), SW_HIDE);		
			return 0;
		case ID_BUTTON6:	
			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_NORMAL);							
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_CAMOBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_SUBSETSLIST), SW_HIDE);
			return 0;
		case ID_BUTTON10:
			ShowWindow(windows->getWindowHandle(GET_CAMOBJECTLIST), SW_NORMAL);							
			ShowWindow(windows->getWindowHandle(GET_OBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_LIGHTOBJECTLIST), SW_HIDE);
			ShowWindow(windows->getWindowHandle(GET_SUBSETSLIST), SW_HIDE);
			return 0;
		case ID_OSBUTTON1:
			objectSettings->showOSettingsWnd();
			return 0;
		case ID_OSBUTTON2:
			objectSettings->showLSettingsWnd();
			return 0;
		case ID_ESSBUTTON1:
			rendStateEditor->showRSSettingsWnd();
			return 0;
		case VM_WNDLOCAT:
			return 0;
		case FM_FILESAVE:
			saveProject(windows->getWindowHandle(GET_MAINWINDOW), manager);
			return 0;
		case FM_FILEOPEN:
			openProject(manager, OC);
			return 0;
		case ID_EDIT1:
			return 0;
		case ID_BSBSTATIC2:
			return 0;
		case ID_BSBSTATIC4:
			return 0;
		case ID_BSBSTATIC6:
			return 0;
		case ID_BSBSTATIC8:
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
			rendStateEditor->showRSSettingsButton();
			if(OC->getMaterialClass())
				objectSettings->fillObjectSettings(OC->getMaterialClass());
			rendStateEditor->fillRSEditor(OC->getRendState());
		}
		if(OC->pickIntersectedVertex())
		{
			objectSettings->showOSButton(SW_NORMAL);
			rendStateEditor->showRSSettingsButton();
			if(OC->getMaterialClass())
				objectSettings->fillObjectSettings(OC->getMaterialClass());
			rendStateEditor->fillRSEditor(OC->getRendState());
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

LRESULT CALLBACK RSEDlgWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_ESSBUTTON2:
			rendStateEditor->setChanges();
			return 0;	
		case ID_ESSBUTTON3:
			rendStateEditor->swapMode();
			return 0;
		case  ID_ESSBUTTON4:
			rendStateEditor->loadShader();
			return 0;
		case IDCANCEL:
			EndDialog(hwnd, TRUE);
			return 0;
		}
		return 0;

	case WM_HELP:
		rendStateEditor->showErrorBuffer();
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
