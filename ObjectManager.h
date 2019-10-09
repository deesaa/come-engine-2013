
class object_manager
{
private:
	IDirect3DDevice9* device;			//Устройство
	UINT numObject;						//Счетчик объектов
	UINT numLight;						//Счетчик объектов света
	UINT numGlobal;

	object_class* object[64];			//Массив сылок на объекты
	light_class* light[64];				//Массив сылок на объекты света

	IDirect3DVertexBuffer9* vb;			//Буфер вершин
	IDirect3DIndexBuffer9* ib;			//Буфер индексов

public:
	object_manager(){}

	void initManager(IDirect3DDevice9* bDevice)
	{
		device = bDevice;				//Сохранение устройства
		numObject = 0;					//Установка начального кол-ва объектов
		numLight = 0;
		numGlobal = 0;
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
		SendMessage(objectList, LB_INSERTSTRING, numObject, (LPARAM)object[numObject]->getObjectName());
		numObject++; 
		numGlobal++;
		return numObject;
	}

	UINT createNewDirectionLight(HWND lightObjectList)
	{
		light[numLight] = new light_class;
		light[numLight]->initDirectionLightBase(device, numLight);
		SendMessage(lightObjectList, LB_INSERTSTRING, numLight, (LPARAM)light[numLight]->getLightName());
		for(int i = 0; i != numLight; i++)   //Отключение всех источников света
			light[i]->disableLight();
		numLight++;
		numGlobal++;
		return numLight;
	}

	void renameObject(UINT objectNumber, HWND objectsList, HWND nameEditor)
	{
		objectNumber--;
		if(objectNumber < numObject)
			object[objectNumber]->renameObject(objectsList, nameEditor, objectNumber);
	}

	void moveObject(UINT objectNumber, float x, float y, float z)
	{
		objectNumber--;
		if(objectNumber < numObject)
			object[objectNumber]->moveObject(x, y, z);
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

	void redirectLight(UINT lightNumber, int DIRECT_TYPE)
	{
		lightNumber--;
		light[lightNumber]->redirectLight(DIRECT_TYPE);
	}

	//Перерисовать объект
	void redrawObject(UINT objectNumber)
	{
		objectNumber--;
		if (objectNumber <= numObject)			//Если данный объект создан
			object[objectNumber]->redraw();		//то перерисовываем его
	}

	void setOnlyPickedLight(UINT lightNumber)
	{
		
		for(UINT numLightC = 0; numLightC != numLight; numLightC++)
			light[numLightC]->disableLight();
		if(lightNumber != NULL)
		{
			lightNumber--;
			light[lightNumber]->enableLight();
		}
	}

	void redrawAllObjects()
	{
		for(UINT objectNumber = 0; objectNumber < numObject; objectNumber++)
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
			delete object[numObject];
		
		for(;numLight != 0; numLight--)
			delete light[numLight];
	}
};