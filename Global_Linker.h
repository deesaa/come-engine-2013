//Включение всяких технических библиотек, в которых лезть не надо
#include <windows.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3dx9.lib")

#include <windows.h>
#include <d3d9.h>
#include <dinput.h>
#include <d3dx9.h>
#include <D3dx9tex.h>

#include <string>

//Прототипы всяких функций
HRESULT CALLBACK MainProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OSWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OSDlgWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void RegWndClass(WNDPROC proc, HINSTANCE hInstance, LPCTSTR className);
void setMatrices(D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix, IDirect3DDevice9* device);

//Собственно включение всех компонентов "движка"
#include "Constants.h"
#include "Structs.h"
#include "Matrices_Class.h"
#include "DirectXInput_Class.h"
#include "Material_Class.h"
#include "Light_Class.h"
#include "ObjectClass.h"
#include "ObjectManager.h"
#include "DialogWndClass.h"
#include "Window_Base.h"
#include "DirectX_Base.h"
#include "OC_Base.h"
#include "Object_Settings.h"