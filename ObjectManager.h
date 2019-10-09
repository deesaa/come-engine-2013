
class object_manager
{
private:
	IDirect3DDevice9* device;			//����������
	DWORD numObject;					//������� ��������
	DWORD numLight;						//������� �������� �����
	DWORD numCam;
	DWORD numGlobal;

	float* timeDelta;

	object_class* object[512];				//������ ����� �� �������
	light_class*  light[128];				//������ ����� �� ������� �����
	camera_class* camera[64];

public:
	object_manager(){}

	void initManager(IDirect3DDevice9* bDevice)
	{
		device = bDevice;						//���������� ����������
		numObject = 0;					//��������� ���������� ���-�� ��������
		numLight = 0;
		numGlobal = 0;
		numCam = 0;
	}

	//�������� ��������� ������ �������
	UINT createNewObject(HWND objectList)
	{
		object[numObject] = new object_class;	//��������� ������ ��� �������
		object[numObject]->initObjectBase(device, numObject); //�������� ���� ������ �������
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
		camera[numCam] = new camera_class;
		camera[numCam]->initCamBase(device);
		SendMessage(camObjectList, LB_INSERTSTRING, numCam, (LPARAM)camera[numCam]->getCamName());
		numCam++;
		numGlobal++;
		return numCam;
	}

	DWORD checkIntersectionWithObjects(ray_struct clickRay)
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
			intersectedObject = object[numObjectC]->checkIntersection(clickRay);
			if(intersectedObject != NULL)
				return intersectedObject;
		}
		return 0;
	}

	void renameObject(DWORD objectNumber, HWND objectsList, HWND nameEditor)
	{
		objectNumber--;
		if(objectNumber < numObject)
			object[objectNumber]->renameObject(objectsList, nameEditor, objectNumber);
	}

	void moveObject(DWORD objectNumber, float x, float y, float z)
	{
		objectNumber--;
		if(objectNumber < numObject)
			object[objectNumber]->moveObject(x, y, z);
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

	void moveLight(DWORD lightNumber, float x, float y, float z)
	{
		lightNumber--;
		if(lightNumber < numLight)
			light[lightNumber]->moveObject(x, y, z);
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
		if(camNumber < numCam)
			camera[camNumber]->move(moveType);
	}

	void rotateCam(DWORD camNumber, float dX, float dY)
	{
		camNumber--;
		if(camNumber < numCam)
			camera[camNumber]->rotate(dX, dY);		
	}

	void resetCam(DWORD camNumber)
	{
		camNumber--;
		if(camNumber < numCam)
			camera[camNumber]->resetMatrices();
	}

	D3DMATERIAL9* getMaterialClass(DWORD objectNumber)
	{
		objectNumber--;
		return object[objectNumber]->getMaterial();
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

	//����������
	~object_manager()
	{
		//���������� ��� ��������� �������, ������ ��
		for(;numObject != 0;)
		{
			numObject--;
			delete object[numObject];
			object[numObject] = NULL;
		}
		
		for(;numLight != 0;)
		{
			numLight--;
			delete light[numLight];
			light[numLight] = NULL;
		}

		for(;numCam != 0;)
		{
			numCam--;
			delete camera[numCam];
			camera[numCam] = NULL;
		}
	}
};