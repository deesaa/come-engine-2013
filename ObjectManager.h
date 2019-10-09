
class object_manager
{
private:
	IDirect3DDevice9* device;			//����������
	UINT numObject;						//������� ��������

	object_class* object[64];			//������ ����� �� �������

	IDirect3DVertexBuffer9* vb;			//����� ������
	IDirect3DIndexBuffer9* ib;			//����� ��������

public:
	object_manager(){}

	void initManager(IDirect3DDevice9* bDevice)
	{
		device = bDevice;				//���������� ����������
		numObject = 0;					//��������� ���������� ���-�� ��������
		this->createBuffers();			//�������� ������ ������ � ��������
	}

	//�������� ������ ������ � ��������
	void createBuffers()
	{
		device->CreateVertexBuffer(
			512 * sizeof(vertex),
			D3DUSAGE_WRITEONLY,
			vertex::FVF,
			D3DPOOL_MANAGED,
			&vb,
			0);

		device->CreateIndexBuffer(
			512 * sizeof(WORD),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&ib,
			0);
	}

	//�������� ��������� ������ �������
	UINT createNewObject(HWND objectList)
	{
		object[numObject] = new object_class;	//��������� ������ ��� �������
		object[numObject]->initObjectBase(device, numObject, vb, ib); //�������� ���� ������ �������
		SendMessage(objectList, LB_ADDSTRING, 0, (LPARAM)object[numObject]->getObjctName());
		numObject++; //��������� �������� ��������
		return numObject;
	}

	void moveObject(UINT objectNumber, float x, float y, float z)
	{
		objectNumber--;
		if(objectNumber < numObject)
		{
			object[objectNumber]->moveObject(x, y, z);
		}
	}

	void rotateObject(UINT objectNumber, int AXIS, float Angle)
	{
		objectNumber--;
		if(objectNumber < numObject)
		{
			switch(AXIS)
			{
			case ROTATION_AXIS_X:
				object[objectNumber]->rotateXObject(Angle);
				break;
			case ROTATION_AXIS_Y:
				object[objectNumber]->rotateYObject(Angle);
				break;
			case ROTATION_AXIS_Z:
				object[objectNumber]->rotateZObject(Angle);
				break;
			}
		}
	}

	//������������ ������
	void redrawObject(UINT objectNumber)
	{
		if (objectNumber < numObject)			//���� ������ ������ ������
			object[objectNumber]->redraw();		//�� �������������� ���
	}

	void redrawAllObjects()
	{
		for(int objectNumber = 0; objectNumber < numObject; objectNumber++)
			object[objectNumber]->redraw();
	}

	//����������
	~object_manager()
	{
		if(vb)	//���� �� ���� - �������
			vb->Release();
		if(ib)  //���� �� ���� - �������
			ib->Release();

		//���������� ��� ��������� �������, ������ ��
		for(;numObject != 0; numObject--)
		{
			delete object[numObject-1];
		}
	}
};