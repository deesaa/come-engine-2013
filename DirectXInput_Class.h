class d3dInput_class
{
private:
	IDirectInput8* di;					//������� ������ DirectXInput
	IDirectInputDevice8* diDeviceKB;	
	IDirectInputDevice8* diDeviceM;

public:
	//����������� ����������� ���� ������ ������� ��������
	d3dInput_class()
	{
		di = NULL;
		diDeviceKB = NULL;
		diDeviceM = NULL;
	}

	//�������� �������� ������� DirectXInput
	void createInput(HINSTANCE hInstance)
	{
		DirectInput8Create(hInstance,         
			DIRECTINPUT_VERSION, 
			IID_IDirectInput8,  
			(void**)&di, 
			NULL);
	}

	//�������� ���������: ���������� � ����
	void createKBMInput(HWND windowHandle)
	{
		di->CreateDevice(GUID_SysKeyboard, &diDeviceKB, NULL);
		diDeviceKB->SetDataFormat(&c_dfDIKeyboard);
		diDeviceKB->SetCooperativeLevel(windowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

		di->CreateDevice(GUID_SysMouse, &diDeviceM, NULL);
		diDeviceM->SetDataFormat(&c_dfDIMouse);
		diDeviceM->SetCooperativeLevel(windowHandle, DISCL_FOREGROUND | DISCL_EXCLUSIVE);	
	}
	
	//�������� ����������
	IDirectInputDevice8* getKBDevice()		
	{
		return diDeviceKB;
	}

	//�������� ����
	IDirectInputDevice8* getMDevice()
	{
		return diDeviceM;
	}

	//��������� �� ��������� ����������/����
	void Release()
	{
		if(diDeviceKB != NULL)
			diDeviceKB->Unacquire();	//�������� ������
		if(diDeviceKB != NULL)
			diDeviceKB->Release();		//���������
		diDeviceKB = NULL;

		if(diDeviceM != NULL)
			diDeviceM->Unacquire();		//�������� ������
		if(diDeviceM != NULL)
			diDeviceM->Release();		//���������
		diDeviceM = NULL;
	}

	//���������� ����������� ���� ������ ������� ��������
	~d3dInput_class()				
	{
		di = NULL;
		diDeviceKB = NULL;
		diDeviceM = NULL;
	}
};