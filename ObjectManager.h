
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
	UINT createNewObject()
	{
		object[numObject] = new object_class;	//��������� ������ ��� �������
		object[numObject]->initObjectBase(device, numObject, vb, ib); //�������� ���� ������ �������
		numObject++; //��������� �������� ��������
		return numObject;
	}

	void moveObject(UINT objectNumber, float x, float y, float z)
	{
		if(objectNumber < numObject)
		{
			object[objectNumber]->moveObject(x, y, z);
		}
	}

	//������������ ������
	void redrawObject(UINT objectNumber)
	{
		if (objectNumber < numObject)			//���� ������ ������ ������
			object[objectNumber]->redraw();		//�� �������������� ���
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