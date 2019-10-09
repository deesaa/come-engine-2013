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

	object_manager* manager;			//���������� ��������� ��������
	UINT pickedObject;					//��������� � ������ ������ ������(�� ���� ������������� ����������)

public:
	object_creator(){}

	void initObjectCreator(IDirect3DDevice9* bDevice, HWND bWindowHandle, HINSTANCE bhInstance, object_manager* bManager)
	{
		device = bDevice;					//���������� ������������ ����������, ����, ����������
		windowHandle = bWindowHandle;
		hInstace = bhInstance;
		dX = 0; dY = 0;	dZ = 0;
		AngleX = 0.0f; AngleY = 0.0f;
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

	void TakeCreatedObject(UINT bPickedObject)
	{
		pickedObject = bPickedObject;
	}

	void applyKBChanges()
	{
		
		if(FAILED(KBDevice->GetDeviceState(sizeof(KBBuffer), KBBuffer)))
			KBDevice->Acquire();

		if(FAILED(MDevice->GetDeviceState(sizeof(DIMOUSESTATE), &MBuffer)))
			MDevice->Acquire();
		
		if (KBBuffer[DIK_W] & 0x80)
		{
			AngleX += 0.001f;
			if(AngleX >= 6.28f)
				AngleX = 0.0f;
			matrices.worldMatrixRotateX(AngleX);
		}

		if (KBBuffer[DIK_S] & 0x80)
		{
			AngleX -= 0.001f;
			if(AngleX >= 6.28f)
				AngleX = 0.0f;
			matrices.worldMatrixRotateX(AngleX);
		}

		if (KBBuffer[DIK_A] & 0x80)
		{
			AngleY += 0.001f;
			if(AngleY >= 6.28f)
				AngleY = 0.0f;
			matrices.worldMatrixRotateY(AngleY);
		}

		if (KBBuffer[DIK_D] & 0x80)
		{
			AngleY -= 0.001f;
			if(AngleY >= 6.28f)
				AngleY = 0.0f;
			matrices.worldMatrixRotateY(AngleY);
		}
			
		if (MBuffer.rgbButtons[LEFT_BUTTON] & 0x80)
		{
			dX += MBuffer.lX * 0.02f;
			dY -= MBuffer.lY * 0.02f;
			matrices.worldMatrixMove(dX, dY, 0);
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
		d3dInput.Release();
		ib->Release();
		vb->Release();
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