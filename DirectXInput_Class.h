class d3dInput_class
{
private:
	IDirectInput8* di;					//Главный объект DirectXInput
	IDirectInputDevice8* diDeviceKB;	
	IDirectInputDevice8* diDeviceM;

public:
	//Конструктор присваивает всем сылкам нулевое значение
	d3dInput_class()
	{
		di = NULL;
		diDeviceKB = NULL;
		diDeviceM = NULL;
	}

	//Создание главного объекта DirectXInput
	void createInput(HINSTANCE hInstance)
	{
		DirectInput8Create(hInstance,         
			DIRECTINPUT_VERSION, 
			IID_IDirectInput8,  
			(void**)&di, 
			NULL);
	}

	//Создание устройств: клавиотуры и мыши
	void createKBMInput(HWND windowHandle)
	{
		di->CreateDevice(GUID_SysKeyboard, &diDeviceKB, NULL);
		diDeviceKB->SetDataFormat(&c_dfDIKeyboard);
		diDeviceKB->SetCooperativeLevel(windowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

		di->CreateDevice(GUID_SysMouse, &diDeviceM, NULL);
		diDeviceM->SetDataFormat(&c_dfDIMouse);
		diDeviceM->SetCooperativeLevel(windowHandle, DISCL_FOREGROUND | DISCL_EXCLUSIVE);	
	}
	
	//Получить клавиотуру
	IDirectInputDevice8* getKBDevice()		
	{
		return diDeviceKB;
	}

	//Получить мышь
	IDirectInputDevice8* getMDevice()
	{
		return diDeviceM;
	}

	//Отключить от программы клавиотуру/мышь
	void Release()
	{
		if(diDeviceKB != NULL)
			diDeviceKB->Unacquire();	//Потерять доступ
		if(diDeviceKB != NULL)
			diDeviceKB->Release();		//Выгрузить
		diDeviceKB = NULL;

		if(diDeviceM != NULL)
			diDeviceM->Unacquire();		//Потерять доступ
		if(diDeviceM != NULL)
			diDeviceM->Release();		//Выгрузить
		diDeviceM = NULL;
	}

	//Деструктор присваивает всем сылкам нулевое значение
	~d3dInput_class()				
	{
		di = NULL;
		diDeviceKB = NULL;
		diDeviceM = NULL;
	}
};