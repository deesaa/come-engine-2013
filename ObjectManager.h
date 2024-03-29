
class object_manager
{
private:
	IDirect3DDevice9* device;			//����������
	botStatusBar_Class* BSBar;
	topEditingBar_Class* TEBar;
	rendStateTypes_class* rendStateTypes;
	window_class* windowsBase;
	HINSTANCE hInstance;
	HWND window;

	DWORD numObject;					//������� ��������
	DWORD numLight;						//������� �������� �����
	DWORD numCams;
	DWORD numGlobal;
	bool i;

	float* timeDelta;

	object_class* object[512];				//������ ����� �� �������
	light_class*  light[128];				//������ ����� �� ������� �����
	camera_class* camera[64];

public:
	object_manager(){}

	void initManager(IDirect3DDevice9* bDevice, HINSTANCE bhInstance, HWND bWindow, botStatusBar_Class* bBSBar,
		topEditingBar_Class* bTEBar, rendStateTypes_class* bRendStateTypes, window_class* bWindowsBase)
	{
		device = bDevice;						//���������� ����������
		BSBar = bBSBar;
		TEBar = bTEBar;
		rendStateTypes = bRendStateTypes;
		windowsBase = bWindowsBase;
		hInstance = bhInstance;
		window = bWindow;
		numObject = 0;					//��������� ���������� ���-�� ��������
		numLight = 0;
		numGlobal = 0;
		numCams = 0;
		i = true;
	}

	//�������� ��������� ������ �������
	UINT createNewObject(HWND objectList, HWND subsetsList, bool isVoid)
	{
		object[numObject] = new object_class;	//��������� ������ ��� �������
		object[numObject]->initObjectBase(device, numObject, subsetsList, BSBar, rendStateTypes, isVoid); //�������� ���� ������ �������
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
		for(int i = 0; i != numLight; i++)   //���������� ���� ���������� �����
			light[i]->disableLight();
		numLight++;
		numGlobal++;
		return numLight;
	}

	UINT createNewPointLight(HWND lightObjectList)
	{
		light[numLight] = new light_class;
		light[numLight]->initPointLightBase(device, numLight);
		SendMessage(lightObjectList, LB_INSERTSTRING, numLight, (LPARAM)light[numLight]->getLightName());
		for(int i = 0; i != numLight; i++)   //���������� ���� ���������� �����
			light[i]->disableLight();
		numLight++;
		numGlobal++;
		return numLight;
	}

	UINT createNewSpotLight(HWND lightObjectList)
	{
		light[numLight] = new light_class;
		light[numLight]->initSpotLightBase(device, numLight);
		SendMessage(lightObjectList, LB_INSERTSTRING, numLight, (LPARAM)light[numLight]->getLightName());
		for(int i = 0; i != numLight; i++)   //���������� ���� ���������� �����
			light[i]->disableLight();
		numLight++;
		numGlobal++;
		return numLight;
	}

	UINT createNewCam(HWND camObjectList)
	{
		camera[numCams] = new camera_class;
		camera[numCams]->initCamBase(device);
		SendMessage(camObjectList, LB_INSERTSTRING, numCams, (LPARAM)camera[numCams]->getCamName());
		numCams++;
		numGlobal++;
		return numCams;
	}

	DWORD checkIntersectionWithObjects(ray_struct clickRay, interType intersectedType)
	{
		DWORD intersectedObject = 0;
		D3DXMATRIX viewMatrix;
		device->GetTransform(D3DTS_VIEW, &viewMatrix);
		D3DXMatrixInverse(&viewMatrix, NULL, &viewMatrix);
		D3DXVec3TransformCoord(&clickRay.origin, &clickRay.origin, &viewMatrix);
		D3DXVec3TransformNormal(&clickRay.direction, &clickRay.direction, &viewMatrix);
		D3DXVec3Normalize(&clickRay.direction, &clickRay.direction);

		for(DWORD numObjectC = 0; numObjectC != numObject; numObjectC++)
		{	
			intersectedObject = object[numObjectC]->checkIntersection(clickRay, intersectedType);
			if(intersectedObject != NULL)
				return intersectedObject;
		}
		return 0;
	}
	
	void unpickAllVerts(DWORD objectNumber)
	{
		objectNumber--;
		object[objectNumber]->unpickAllVerts();
	}

	void renameObject(DWORD objectNumber, HWND objectsList, HWND nameEditor)
	{
		objectNumber--;
		if(objectNumber < numObject)
			object[objectNumber]->renameObject(objectsList, nameEditor, objectNumber);
	}

	void moveObject(DWORD objectNumber, float x, float y, float z, moveBy moveSpace)
	{
		objectNumber--;
		if(objectNumber < numObject)
			object[objectNumber]->moveObject(x, y, z, moveSpace);
	}

	void rotateObject(DWORD objectNumber, int AXIS, float Angle)
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

	void rewriteSubsetsList(DWORD objectNumber)
	{
		objectNumber--;
		object[objectNumber]->rewriteSubsetsList();
	}

	void pickSubset(DWORD objectNumber, DWORD subsetNumber)
	{
		objectNumber--;
		object[objectNumber]->pickSubset(subsetNumber);
	}

	void moveLight(DWORD lightNumber, float x, float y, float z, moveBy moveSpace)
	{
		lightNumber--;
		if(lightNumber < numLight)
			light[lightNumber]->moveObject(x, y, z, moveSpace);
	}

	void rotateLight(DWORD lightNumber, int AXIS, float Angle)
	{
		lightNumber--;
		if(lightNumber < numLight)
		{
			switch(AXIS)
			{
			case ROTATION_AXIS_X:
				light[lightNumber]->rotateXObject(Angle);
				break;
			case ROTATION_AXIS_Y:
				light[lightNumber]->rotateYObject(Angle);
				break;
			case ROTATION_AXIS_Z:
				light[lightNumber]->rotateZObject(Angle);
				break;
			}
		}
	}

	void moveCam(DWORD camNumber, short moveType)
	{
		camNumber--;
		if(camNumber < numCams)
			camera[camNumber]->move(moveType);
	}

	void moveVertex(DWORD objectNumber, DWORD vertexNumber, float x, float y, float z)
	{
		objectNumber--;
		object[objectNumber]->moveVertex(vertexNumber, x, y, z);
	}

	void createNewTriangle(DWORD objectNumber)
	{
		objectNumber--;
		object[objectNumber]->createNewTriangle();
	}

	void swapIndices(DWORD objectNumber)
	{
		objectNumber--;
		object[objectNumber]->swapIndices();
	}

	void newSubset(DWORD objectNumber)
	{
		objectNumber--;
		object[objectNumber]->newSubset();
	}

	void rotateCam(DWORD camNumber, float dX, float dY)
	{
		camNumber--;
		if(camNumber < numCams)
			camera[camNumber]->rotate(dX, dY);		
	}

	void resetCam(DWORD camNumber)
	{
		camNumber--;
		if(camNumber < numCams)
			 camera[camNumber]->resetMatrices();
	}

	void loadTexture(DWORD objectNumber)
	{
		objectNumber--;
		object[objectNumber]->LoadTexture4Verts();
	}

	void cutVertices(DWORD objectNumber)
	{
		objectNumber--;
		object[objectNumber]->cutVertices();
	}

	void uniteVertices(DWORD objectNumber)
	{
		objectNumber--;
		object[objectNumber]->uniteVertices();
	}

	D3DMATERIAL9* getMaterial(DWORD objectNumber)
	{
		objectNumber--;
		return object[objectNumber]->getMaterial();
	}

	rendState_class* getRendState(DWORD objectNumber)
	{
		objectNumber--;
		return object[objectNumber]->getRendState();
	}

	D3DLIGHT9* getLightStruct(DWORD lightNumber)
	{
		lightNumber--;
		return light[lightNumber]->getLight();
	}

	void setPointerToTimeDelta(float* bTimeDelta)
	{	timeDelta = bTimeDelta;}

	void resetLight(DWORD bPickedLight)
	{
		bPickedLight--;
		light[bPickedLight]->setLight();	
	}

	void saveAs(DWORD objectNumber)
	{
		objectNumber--;
		object[objectNumber]->saveObjectAs(hInstance, window);
	}

	void saveFullObject(DWORD objectNumber)
	{
		objectNumber--;
		object[objectNumber]->saveObject();
	}

	DWORD getNumObjects()
	{
		return numObject;
	}

	object_class* getObject(DWORD objectNumber)
	{
		return object[objectNumber];
	}

	DWORD getNumLight()
	{
		return numLight;
	}

	light_class* getLight(DWORD lightNumber)
	{
		return light[lightNumber];
	}

	DWORD getNumMaterials(DWORD objectNumber)
	{
		return object[objectNumber]->getNumMaterials();
	}

	material_class* getMaterialClass(DWORD objectNumber, DWORD materialNumber)
	{
		return object[objectNumber]->getMaterialClass(materialNumber);
	}

	void setNumObjects(DWORD bNumObjects)
	{
		numObject = bNumObjects; 
	}

	void setNumLight(DWORD bNumLight)
	{
		numLight = bNumLight;
	}

	void setNumCams(DWORD bNumCams)
	{
		numCams = bNumCams;
	}

	void setNumGlobal()
	{
		numGlobal = numObject+numLight+numCams;
	}

	DWORD getNumCams()
	{
		return numCams;
	}

	//������������ ������
	void redrawObject(DWORD objectNumber)
	{
		objectNumber--;
		if (objectNumber <= numObject)			//���� ������ ������ ������
			object[objectNumber]->redraw();		//�� �������������� ���
	}

	void setOnlyPickedLight(DWORD lightNumber)
	{
		
		for(DWORD numLightC = 0; numLightC != numLight; numLightC++)
			light[numLightC]->disableLight();
		if(lightNumber != NULL)
		{
			lightNumber--;
			light[lightNumber]->enableLight();
		}
	}

	void redrawObjectOfLight(DWORD lightNumber)
	{
		if(lightNumber != NULL)
		{
			lightNumber--;
			light[lightNumber]->redrawObject();
		}
	}
	void redrawAllObjects()
	{
		for(DWORD objectNumber = 0; objectNumber < numObject; objectNumber++)
			object[objectNumber]->redraw();
	}

	DWORD loadManager(temp_manager* tempManager)
	{
		DWORD objectNumber;
		SendMessage(windowsBase->getWindowHandle(GET_OBJECTLIST), LB_RESETCONTENT, 0, 0L);
		SendMessage(windowsBase->getWindowHandle(GET_LIGHTOBJECTLIST), LB_RESETCONTENT, 0, 0L);
		SendMessage(windowsBase->getWindowHandle(GET_SUBSETSLIST), LB_RESETCONTENT, 0, 0L);
		SendMessage(windowsBase->getWindowHandle(GET_CAMOBJECTLIST), LB_RESETCONTENT, 0, 0L);
		this->~object_manager();

		for(DWORD counter(0); counter != tempManager->numFactObjects;)
		{
			objectNumber = this->createNewObject(windowsBase->getWindowHandle(GET_OBJECTLIST), windowsBase->getWindowHandle(GET_SUBSETSLIST), true);
			object[counter]->loadObject(tempManager->object[counter]);
			counter += 1;
		}

		for(DWORD counter(0); counter != tempManager->numFactLight;)
		{
			if(tempManager->light[counter]->light.Type == D3DLIGHT_POINT)
				this->createNewPointLight(windowsBase->getWindowHandle(GET_LIGHTOBJECTLIST));

			if(tempManager->light[counter]->light.Type == D3DLIGHT_DIRECTIONAL)
				this->createNewDirectionLight(windowsBase->getWindowHandle(GET_LIGHTOBJECTLIST));

			if(tempManager->light[counter]->light.Type == D3DLIGHT_SPOT)
				this->createNewSpotLight(windowsBase->getWindowHandle(GET_LIGHTOBJECTLIST));

			light[counter]->loadLight(tempManager->light[counter]);
			counter += 1;
		}
		return objectNumber;
	}

	~object_manager()
	{
		//���������� ��� ��������� �������, ������ ��
		for(;numObject != 0;)
		{
			numObject--;
			if(object[numObject] != NULL)
			{
				delete object[numObject];
				object[numObject] = NULL;
			}
		}
		
		for(;numLight != 0;)
		{
			numLight--;
			if(light[numLight] != NULL)
			{
				delete light[numLight];
				light[numLight] = NULL;
			}
		}

		for(;numCams != 0;)
		{
			numCams--;
			if(camera[numCams] != NULL)
			{
				delete camera[numCams];
				camera[numCams] = NULL;
			}
		}
		numGlobal = 0;
	}
};
