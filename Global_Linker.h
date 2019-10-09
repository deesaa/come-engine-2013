//Включение всяких технических библиотек, в которых лезть не надо
#include <windows.h>
#include <winbase.h>
#include <windowsx.h>
#include <CommDlg.h>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <math.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3dx9.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <winuser.h>
#include <d3d9.h>
#include <dinput.h>
#include <d3dx9.h>
#include <D3dx9tex.h>

#include "mmsystem.h"

#include <string>
#include <vector>

//Прототипы всяких функций
HRESULT CALLBACK MainProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OSWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OSDlgWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LSDlgWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CEDlgWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK RSEDlgWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void RegWndClass(WNDPROC proc, HINSTANCE hInstance, LPCTSTR className);
void setAllMatrices(D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix, IDirect3DDevice9* device);
void setViewMatrices(D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix, IDirect3DDevice9* device);
BOOL initPalette(HWND hwnd, D3DCOLORVALUE* mtrComponent);

class object_class;
class material_class;
class light_class;
class object_manager;
class temp_manager;
class tempObject_class;
class tempLight_class;
class tempCamera_class;
void saveFullObject(object_class* object, std::wstring file);
void saveFullMaterial(material_class* material, std::wstring file);
void saveFullLight(light_class* light, std::wstring file);
void saveProjectInfo(object_manager* manager, std::wstring file);
void saveAs(HINSTANCE bhInstace, HWND bWindow, object_class* object);
void saveProject(HWND mainWindow);
std::vector<float> getArrayFlValue(std::string* name, std::string* file);
std::vector<DWORD> getArrayDwValue(std::string* name, std::string* file, DWORD pos);
DWORD getDwValue(std::string* name, std::string* file, DWORD pos);
bool openObjects(temp_manager* manager, std::wstring fileName);
bool openObjectVertices(tempObject_class* object, std::string* file);
bool openObjectIndices(tempObject_class* object, std::string* file);
bool openObjectTriangles(tempObject_class* object, std::string* file);
bool openObjectAttributes(tempObject_class* object, std::string* file);
bool openObjectAdjacency(tempObject_class* object, std::string* file);
bool openProjectInfo(temp_manager* manager, std::string* file);
bool openProject(object_manager* manager);

//Собственно включение всех компонентов "движка"
#include "Constants.h"
#include "Structs.h"
#include "Temp_Classes.h"
#include "Camera_Class.h"
#include "Matrices_Class.h"
#include "DirectXInput_Class.h"
#include "DialogWndClass.h"
#include "Material_Class.h"
#include "Texture_Class.h"
#include "Light_Class.h"
#include "BotStatus_Bar.h"
#include "TopEditing_Bar.h"
#include "RendState_Database.h"
#include "RendState_Class.h"
#include "ObjectClass.h"
#include "ObjectManager.h"
#include "Window_Base.h"
#include "MainMenu_Class.h"
#include "DirectX_Base.h"
#include "Object_Settings.h"
#include "RendState_Editor.h"
#include "OC_Base.h"
#include "Object_Outwrite.h"