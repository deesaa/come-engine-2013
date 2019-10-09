//��������� ������� ����� ��������� ��������
struct COVertex	
{
	float x, y, z;
	static const DWORD FVF;

	COVertex(){}
	COVertex(float bX, float bY, float bZ)
	{
		x = bX; y = bY; z = bZ;
	}
};
const DWORD COVertex::FVF = D3DFVF_XYZ;

//��������� (����������) ������� ��������� ��������
void COFillBuffers(IDirect3DVertexBuffer9* vb, IDirect3DIndexBuffer9* ib, COVertex* vertices, WORD* indices);

class object_creator
{
private:
	IDirect3DDevice9* device;
	HWND windowHandle;
	HINSTANCE hInstace;
	fullMatrices_class matrices;		//������ ������ ������ ������ (����, ����, ��������������) ��� ����� ��������� ��������

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

public:
	object_creator(){}

	void initObjectCreator(IDirect3DDevice9* bDevice, HWND bWindowHandle, HINSTANCE bhInstance, object_manager* bManager)
	{
		device = bDevice;					//���������� ������������ ����������, ����, ����������
		windowHandle = bWindowHandle;
		hInstace = bhInstance;
		dX = dY = dZ = 0;
		AngleX = AngleY = 0.0f;
		clickX = clickY = 0;
		this->createBuffers();  //�������� ������ ������ � ��������					
		COFillBuffers(vb, ib, vertices, indices);  //���������� ������ ������ � �������� ����� ��������� ��������
		D3DXVECTOR3 position(0.0f, 0.0f, -20.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		matrices.fillMatrices(0, 0, 0, 0.25f, device, &position, &target, &up);  //���������� � ��������� ���� ���� ������
		d3dInput.createInput(hInstace);
		d3dInput.createKBMInput(windowHandle);
		KBDevice = d3dInput.getKBDevice();
		MDevice = d3dInput.getMDevice();

		manager = bManager;						//���������� ����������� ��������� ��������
		pickedObject = NULL;					//���������� ������� ��� ������������� �� ����� ����
		pickedLight = NULL;

		matrices.worldMatrixRotateX(0.0f);		//��������� ����� ������� � 0 (��� ����������� ���������� �����������)
		matrices.worldMatrixRotateY(0.0f);
		matrices.worldMatrixRotateZ(0.0f);
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
	{	pickedLight = NULL;
		pickedObject = bPickedObject;}

	void pickLight(UINT bPickedLight)
	{	pickedObject = NULL;
		pickedLight = bPickedLight;}

	UINT getPickedObject()
	{	return pickedObject;}

	UINT getPickedLight()
	{	return pickedLight;}

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

	void IfIntersectionPickObject()
	{
		DWORD bPickedObject = manager->checkIntersectionWithObjects(clickRay);

		if(bPickedObject != 0)
			pickedObject = bPickedObject;		
	}

	void applyKBMChanges()
	{
		if(FAILED(KBDevice->GetDeviceState(sizeof(KBBuffer), KBBuffer)))
			KBDevice->Acquire();

		if(FAILED(MDevice->GetDeviceState(sizeof(DIMOUSESTATE), &MBuffer)))
			MDevice->Acquire();
		
		if (KBBuffer[DIK_W] & 0x80)
		{
			AngleX = 0.001f;
			if(pickedObject != NULL)
				manager->rotateObject(pickedObject, ROTATION_AXIS_X, AngleX);
			if(pickedLight != NULL)
				manager->redirectLight(pickedLight, DIRECTION_UP);
		}

		if (KBBuffer[DIK_S] & 0x80)
		{
			AngleX = -0.001f;
			if(pickedObject != NULL)
				manager->rotateObject(pickedObject, ROTATION_AXIS_X, AngleX);
			if(pickedLight != NULL)
				manager->redirectLight(pickedLight, DIRECTION_DOWN);
		}

		if (KBBuffer[DIK_A] & 0x80)
		{
			AngleY = 0.001f;
			if(pickedObject != NULL)
				manager->rotateObject(pickedObject, ROTATION_AXIS_Y, AngleY);
			if(pickedLight != NULL)
				manager->redirectLight(pickedLight, DIRECTION_RIGHT);
		}

		if (KBBuffer[DIK_D] & 0x80)
		{
			AngleY = -0.001f;
			if(pickedObject != NULL)
				manager->rotateObject(pickedObject, ROTATION_AXIS_Y, AngleY);
			if(pickedLight != NULL)
				manager->redirectLight(pickedLight, DIRECTION_LEFT);
		}
			
		if (MBuffer.rgbButtons[LEFT_BUTTON] & 0x80)
		{
			dX = MBuffer.lX * 0.02f;
			dY = MBuffer.lY * 0.02f;

			if(pickedObject == NULL)
				matrices.worldMatrixMove(dX, dY, dZ);
			else
			{
				if(pickedObject != NULL)
					manager->moveObject(pickedObject, dX, dY, dZ);	
			}
		}
		
		if (MBuffer.rgbButtons[RIGHT_BUTTON] & 0x80)
		{
			dX = 0;
			dY = 0;
			matrices.worldMatrixMove(0, 0, 0);
			matrices.worldMatrixRotateX(0.0f);		//��������� ����� ������� � 0
			matrices.worldMatrixRotateY(0.0f);
			matrices.worldMatrixRotateZ(0.0f);
		}
	}

	//����������� ��������� ��������
	void redraw()	
	{
		matrices.resetWorldMatrices();

		device->SetStreamSource(0, vb, 0, sizeof(COVertex));
		device->SetIndices(ib);
		device->SetFVF(COVertex::FVF);
		device->DrawIndexedPrimitive(D3DPT_LINELIST,
			0, 0, 8, 0, 6);
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

//���������� ������ ������ � �������� ����� ��������� ��������
void COFillBuffers(IDirect3DVertexBuffer9* vb, IDirect3DIndexBuffer9* ib, COVertex* vertices, WORD* indices)
{
	vb->Lock(NULL, NULL, (void**)&vertices, NULL);
	vertices[0] = COVertex(-1, -1, -1);
	vertices[1] = COVertex(1, -1, -1);
	vertices[2] = COVertex(-1, 0, -1);
	vertices[3] = COVertex(1, 0, -1);
	vertices[4] = COVertex(-1, 1, -1);
	vertices[5] = COVertex(1, 1, -1);
	vertices[6] = COVertex(0, 1, -1);
	vertices[7] = COVertex(0, -1, -1);
	vb->Unlock();

	ib->Lock(0, 0, (void**)&indices, 0);
	indices[0]  = 0; indices[1]  = 1; 
	indices[2]  = 2; indices[3]  = 3; 
	indices[4]  = 4; indices[5]  = 5; 
	indices[6]  = 0; indices[7]  = 4; 
	indices[8]  = 7; indices[9]  = 6; 
	indices[10] = 1; indices[11] = 5; 
	ib->Unlock();
}