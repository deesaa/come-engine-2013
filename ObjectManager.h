
class object_manager
{
private:
	IDirect3DDevice9* device;			//Устройство
	UINT numObject;						//Счетчик объектов

	object_class* object[64];			//Массив сылок на объекты

	IDirect3DVertexBuffer9* vb;			//Буфер вершин
	IDirect3DIndexBuffer9* ib;			//Буфер индексов

public:
	object_manager(){}

	void initManager(IDirect3DDevice9* bDevice)
	{
		device = bDevice;				//Сохранение устройства
		numObject = 0;					//Установка начального кол-ва объектов
		this->createBuffers();			//Создание буфера вершин и индексов
	}

	//Создание буфера вершин и индексов
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

	//Создание абсолютно нового объекта
	UINT createNewObject(HWND objectList)
	{
		object[numObject] = new object_class;	//Выделение памяти для объекта
		object[numObject]->initObjectBase(device, numObject, vb, ib); //Создание базы нового объекта
		SendMessage(objectList, LB_ADDSTRING, 0, (LPARAM)object[numObject]->getObjctName());
		numObject++; //Инкремент счетчика объектов
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

	//Перерисовать объект
	void redrawObject(UINT objectNumber)
	{
		if (objectNumber < numObject)			//Если данный объект создан
			object[objectNumber]->redraw();		//то перерисовываем его
	}

	void redrawAllObjects()
	{
		for(int objectNumber = 0; objectNumber < numObject; objectNumber++)
			object[objectNumber]->redraw();
	}

	//Деструктор
	~object_manager()
	{
		if(vb)	//Если не пуст - удаляем
			vb->Release();
		if(ib)  //Если не пуст - удаляем
			ib->Release();

		//Перебираем все созданные объекты, удаляя их
		for(;numObject != 0; numObject--)
		{
			delete object[numObject-1];
		}
	}
};