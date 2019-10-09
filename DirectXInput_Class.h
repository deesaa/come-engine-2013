class d3dInput_class
{
private:
	IDirectInput8* di;
	IDirectInputDevice8* diDeviceKB;
public:
	d3dInput_class(){}
	d3dInput_class(HWND windowHandle, HINSTANCE hInstance)
	{
		DirectInput8Create(hInstance,         
			DIRECTINPUT_VERSION, 
			IID_IDirectInput8,  
			(void**)&di, 
			NULL);

		di->CreateDevice(GUID_SysKeyboard,  
			&diDeviceKB,         
			NULL);

		diDeviceKB->SetDataFormat(&c_dfDIKeyboard);

		diDeviceKB->SetCooperativeLevel(windowHandle,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		diDeviceKB->Acquire();
	}

	void fillInputClass(HWND windowHandle, HINSTANCE hInstance)
	{
		DirectInput8Create(hInstance,         
			DIRECTINPUT_VERSION, 
			IID_IDirectInput8,  
			(void**)&di, 
			NULL);

		di->CreateDevice(GUID_SysKeyboard,  
			&diDeviceKB,         
			NULL);

		diDeviceKB->SetDataFormat(&c_dfDIKeyboard);

		diDeviceKB->SetCooperativeLevel(windowHandle,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		diDeviceKB->Acquire();
	}

	IDirectInputDevice8* getKBDevice()
	{
		return diDeviceKB;
	}
	~d3dInput_class()
	{
		diDeviceKB->Unacquire();
		diDeviceKB->Release();
	}
};