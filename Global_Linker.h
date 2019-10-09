//Включение всяких технических библиотек, в которых лезть не надо
#include <windows.h>
#include <winbase.h>
#include <CommDlg.h>
#include <fstream>

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
LRESULT CALLBACK BStatBarProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void RegWndClass(WNDPROC proc, HINSTANCE hInstance, LPCTSTR className);
void setAllMatrices(D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix, IDirect3DDevice9* device);
void setViewMatrices(D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix, IDirect3DDevice9* device);
BOOL initPalette(HWND hwnd, D3DCOLORVALUE* mtrComponent);

class object_class;
class material_class;
class light_class;
void saveFullObject(object_class* object);
void saveFullMaterial(material_class* material);
void saveFullLight(light_class* light);
void saveAs(HINSTANCE bhInstace, HWND bWindow, object_class* object);

//Собственно включение всех компонентов "движка"
#include "Constants.h"
#include "Structs.h"
#include "Camera_Class.h"
#include "Matrices_Class.h"
#include "DirectXInput_Class.h"
#include "Material_Class.h"
#include "Texture_Class.h"
#include "Light_Class.h"
#include "BotStatus_Bar.h"
#include "ObjectClass.h"
#include "ObjectManager.h"
#include "DialogWndClass.h"
#include "Window_Base.h"
#include "DirectX_Base.h"
#include "Object_Settings.h"
#include "OC_Base.h"
#include "Object_Outwrite.h"
