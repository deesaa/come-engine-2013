
class object_manager
{
private:
	IDirect3DDevice9* device;			//����������
	DWORD numObject;						//������� ��������
	DWORD numLight;						//������� �������� �����
	DWORD numGlobal;

	object_class* object[64];			//������ ����� �� �������
	light_class* light[64];				//������ ����� �� ������� �����

public:
	object_manager(){}

	void initManager(IDirect3DDevice9* bDevice)
	{
		device = bDevice;						//���������� ����������
		numObject = 0;					//��������� ���������� ���-�� ��������
		numLight = 0;
		numGlobal = 0;
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

	void redirectLight(DWORD lightNumber, int DIRECT_TYPE)
	{
		lightNumber--;
		light[lightNumber]->redirectLight(DIRECT_TYPE);
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
		}
		
		for(;numLight != 0;)
		{
			numLight--;
			delete light[numLight];
		}
	}
};