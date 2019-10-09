

enum picked{Nothing, Object, Light, Cam, Vertex};

class object_creator
{
private:
	IDirect3DDevice9* device;
	HWND windowHandle;
	HINSTANCE hInstace;

	IDirect3DVertexBuffer9* vb;			//����� ������ �����
	IDirect3DIndexBuffer9* ib;			//����� �������� �����

	COVertex* vertices;					//������� ����� ��������� ��������
	WORD* indices;						//������� ����� ��������� ��������

	d3dInput_class d3dInput;			//������ ������ ��������� �����(����������, ����)
	IDirectInputDevice8* KBDevice;		//����������
	IDirectInputDevice8* MDevice;		//����
	char KBBuffer[256];					//����� ��� �������� ��������� ����������
	DIMOUSESTATE MBuffer;				//����� ��� �������� ��������� ����

	float dX, dY, dZ;					//����������� �������� ���������� � ������������ ���� (��� ������������ ��������)
	float AngleX, AngleY, AngleZ;		//���� ��� �������� ��������
	POINT clickedPoint; 
	float clickX, clickY;
	ray_struct clickRay;

	object_manager* manager;			//���������� ��������� ��������
	DWORD pickedObject;					//��������� � ������ ������ ������(�� ���� ������������� ����������)
	DWORD pickedLight;
	DWORD pickedCam;
	DWORD pickedVertex;

	picked pickType;

	bool NewTriButtonLocked;
public:
	object_creator(){}

	void initObjectCreator(IDirect3DDevice9* bDevice, HWND bWindowHandle, HINSTANCE bhInstance, object_manager* bManager)
	{
		device = bDevice;					//���������� ������������ ����������, ����, ����������
		windowHandle = bWindowHandle;
		hInstace = bhInstance;
		dX = dY = dZ = 0.0f;
		AngleX = AngleY = 0.0f;
		clickX = clickY = 0.0f;
		this->createBuffers();  //�������� ������ ������ � ��������	
		d3dInput.createInput(hInstace);
		d3dInput.createKBMInput(windowHandle);
		KBDevice = d3dInput.getKBDevice();
		MDevice = d3dInput.getMDevice();

		NewTriButtonLocked = FALSE;

		manager = bManager;						//���������� ����������� ��������� ��������
		pickedObject = NULL;					
		pickedLight = NULL;
		pickedCam = NULL;
		pickedVertex = NULL;
		pickType = Nothing;
	}

	//�������� ������ ������ � ��������	
	void createBuffers()
	{
		device->CreateVertexBuffer(
			8 * sizeof(COVertex),
			D3DUSAGE_WRITEONLY,
			COVertex::FVF,
			D3DPOOL_MANAGED,
			&vb,
			0);

		device->CreateIndexBuffer(
			36 * sizeof(WORD),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&ib,
			0);
	}

	void pickObject(UINT bPickedObject)
	{	pickType = Object;
		pickedObject = bPickedObject;}

	void pickLight(UINT bPickedLight)
	{	pickType = Light;
		pickedLight = bPickedLight;}

	void pickCam(UINT bPickedCam)
	{	pickType = Cam;
		pickedCam = bPickedCam;}

	void pickVertex(UINT bPickedVertex)
	{	pickType = Vertex;
		pickedVertex = bPickedVertex;}

	UINT getPickedObject()
	{	return pickedObject;}

	UINT getPickedLight()
	{	return pickedLight;}

	UINT getPickedCam()
	{	return pickedCam;}

	UINT getPickedVertex()
	{	return pickedVertex;}

	void renameObject(HWND objectsList, HWND nameEditor)
	{
		manager->renameObject(pickedObject, objectsList, nameEditor);
		SetFocus(windowHandle);
	}

	void createRayOfClick(POINT cOp)
	{	
		clickX = (float)cOp.x;
		clickY = (float)cOp.y;
		D3DXMATRIX proj;
		device->GetTransform(D3DTS_PROJECTION, &proj);
		clickX = ((( 2.0f*clickX) / DirectXWidth)  - 1.0f) / proj(0, 0);
		clickY = (((-2.0f*clickY) / DirectXHeight) + 1.0f) / proj(1, 1);

		clickRay.origin	   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		clickRay.direction = D3DXVECTOR3(clickX, clickY, 1.0f);
	}

	bool pickIntersectedObject()
	{
		DWORD bPickedObject = manager->checkIntersectionWithObjects(clickRay, interObject);

		if(bPickedObject != 0)
		{
			pickedObject = bPickedObject;
			pickType = Object;
			return true;
		}
		return false;
	}

	bool pickIntersectedVertex()
	{
		DWORD bPickedVertex = manager->checkIntersectionWithObjects(clickRay, interVertex);

		if(bPickedVertex != 0)
		{
			pickedVertex = bPickedVertex;
			pickType = Vertex;
			return true;
		}
		return false;
	}

	D3DMATERIAL9* getMaterialClass()
	{
		return manager->getMaterialClass(pickedObject);
	}

	D3DLIGHT9* getLightStruct()
	{
		return manager->getLightStruct(pickedLight);
	}

	bool checkPickType(picked bPickType)
	{
		if(bPickType == pickType)
			return TRUE;
		return FALSE;
	}

	void applyKBMChanges()
	{
		if(FAILED(KBDevice->GetDeviceState(sizeof(KBBuffer), KBBuffer)))
			KBDevice->Acquire();

		if(FAILED(MDevice->GetDeviceState(sizeof(DIMOUSESTATE), &MBuffer)))
			MDevice->Acquire();

		if (MBuffer.rgbButtons[LEFT_BUTTON] & 0x80)
		{
			dX = MBuffer.lX * 0.02f;
			dY = MBuffer.lY * 0.02f;

			if(pickType == Object)
				manager->moveObject(pickedObject, dX, dY, dZ);	
			if(pickType == Light)
				manager->moveLight(pickedLight, dX, dY, dZ);
			if (pickType == Cam)
				manager->rotateCam(pickedCam, dX, dY);	
			if(pickType == Vertex)
				manager->moveVertex(pickedObject, pickedVertex, dX, dY, dZ);
		}
		
		if (KBBuffer[DIK_W] & 0x8000f)
		{
			AngleX = 0.001f;
			if(pickType == Object)
				manager->rotateObject(pickedObject, ROTATION_AXIS_X, AngleX);
			if(pickType == Light)
				manager->rotateLight(pickedLight, ROTATION_AXIS_X, AngleX);
			if(pickType == Cam)
				manager->moveCam(pickedCam, MOVE_FORVARD);
		}

		if (KBBuffer[DIK_S] & 0x8000f)
		{
			AngleX = -0.001f;
			if(pickType == Object)
				manager->rotateObject(pickedObject, ROTATION_AXIS_X, AngleX);
			if(pickType == Light)
				manager->rotateLight(pickedLight, ROTATION_AXIS_X, AngleX);
			if(pickType == Cam)
				manager->moveCam(pickedCam, MOVE_BACK);
		}

		if (KBBuffer[DIK_A] & 0x8000f)
		{
			AngleY = 0.001f;
			if(pickType == Object)
				manager->rotateObject(pickedObject, ROTATION_AXIS_Y, AngleY);
			if(pickType == Light)
				manager->rotateLight(pickedLight, ROTATION_AXIS_Y, AngleY);
			if(pickType == Cam)
				manager->moveCam(pickedCam, MOVE_LEFT);
		}

		if (KBBuffer[DIK_D] & 0x8000f)
		{
			AngleY = -0.001f;
			if(pickType == Object)
				manager->rotateObject(pickedObject, ROTATION_AXIS_Y, AngleY);
			if(pickType == Light)
				manager->rotateLight(pickedLight, ROTATION_AXIS_Y, AngleY);
			if(pickType == Cam)
				manager->moveCam(pickedCam, MOVE_RIGHT);
		}
		
		if (KBBuffer[DIK_F] & 0x8000f)
		{
			if(NewTriButtonLocked == FALSE)
			{
				if(pickType == Object)
					manager->createNewTriangle(pickedObject);
			}
			NewTriButtonLocked = TRUE;
		}
		else
			NewTriButtonLocked = FALSE;
	}

	~object_creator() 
	{
		d3dInput.Release();		//������ ������� � �������� ���������(����������, ����)
		ib->Release();			//�������� ������ ��������
		ib = NULL;
		vb->Release();			//�������� ������ ������
		vb = NULL;
	}
};
