#define DIRECTION_UP	0
#define DIRECTION_DOWN	1
#define DIRECTION_RIGHT 2
#define DIRECTION_LEFT	3

#define DIRECTION_LIGHT 1
#define POINT_LIGHT		2
#define SPOT_LIGHT		2

class light_class
{
private:
	D3DLIGHT9 light;
	DWORD lightID;
	LPCTSTR lightName;
	short lightType;

	IDirect3DVertexBuffer9* vb;			//Ѕуфер вершин сетки
	IDirect3DIndexBuffer9* ib;			//Ѕуфер индексов
	COVertex* vertices;					//¬ершины сетки редактора объектов
	WORD* indices;						//»ндексы сетки редактора объектов
	material_class material;
	worldMatrices_class worldMatrices;
	D3DXVECTOR3 directionBuffer, positionBuffer;
	D3DXVECTOR4 direction, position;

	IDirect3DDevice9* device;
public:
	void initDirectionLightBase(IDirect3DDevice9* bDevice, UINT lightNumber)
	{
		device = bDevice;
		lightID = lightNumber;
		lightName = L"Direction Light";
		lightType = DIRECTION_LIGHT;
		ZeroMemory(&light, sizeof(light));

		light.Type			= D3DLIGHT_DIRECTIONAL;
		light.Diffuse		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light.Specular		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.6f;
		light.Ambient		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.4f;
		light.Direction		= directionBuffer = D3DXVECTOR3(-0.5f, 1.5f, 0.0f);

		device->SetLight(lightID, &light);
		device->LightEnable(lightID, true);

		this->createObjectOfLight();
	}

	void initPointLightBase(IDirect3DDevice9* bDevice, UINT lightNumber)
	{
		device = bDevice;
		lightID = lightNumber;
		lightName = L"Point Light";
		lightType = POINT_LIGHT;
		ZeroMemory(&light, sizeof(light));

		light.Type			= D3DLIGHT_POINT;
		light.Position		= positionBuffer = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		light.Diffuse		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light.Specular		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.6f;
		light.Ambient		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.4f;
		light.Attenuation0  = 0.0f;
		light.Attenuation1  = 1.0f;
		light.Attenuation2  = 0.0f;
		light.Range         = 100;

		device->SetLight(lightID, &light);
		device->LightEnable(lightID, true);

		this->createObjectOfLight();
	}

	void initSpotLightBase(IDirect3DDevice9* bDevice, UINT lightNumber)
	{
		device = bDevice;
		lightID = lightNumber;
		lightName = L"Spot Light";
		lightType = SPOT_LIGHT;
		ZeroMemory(&light, sizeof(light));

		light.Type			= D3DLIGHT_SPOT;
		light.Position		= positionBuffer  = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		light.Direction		= directionBuffer = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		light.Diffuse		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light.Specular		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.6f;
		light.Ambient		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.4f;
		light.Attenuation0  = 0.0f;
		light.Attenuation1  = 1.0f;
		light.Attenuation2  = 0.0f;
		light.Theta			= 0.4f;
		light.Phi			= 1.2f;
		light.Falloff		= 1.0f;
		light.Range         = 100;

		device->SetLight(lightID, &light);
		device->LightEnable(lightID, true);

		this->createObjectOfLight();
	}

	void loadLight(tempLight_class* tempLight)
	{
		light.Diffuse  = tempLight->light.Diffuse;
		light.Ambient  = tempLight->light.Ambient;
		light.Specular = tempLight->light.Specular;

		light.Attenuation0  = tempLight->light.Attenuation0;
		light.Attenuation1  = tempLight->light.Attenuation1;
		light.Attenuation2  = tempLight->light.Attenuation2;
		light.Theta			= tempLight->light.Theta;
		light.Phi			= tempLight->light.Phi;
		light.Falloff		= tempLight->light.Falloff;
		light.Range         = tempLight->light.Range;
	}

	void createObjectOfLight()
	{
		this->createAndFillBuffers();
		material.initAbsolutelyWhiteMaterial(device, NULL);
		worldMatrices.fillMatrix(0, 0, 0, device);
		worldMatrices.worldMatrixRotateX(0.0f);		//”становка углов наклона в 0
		worldMatrices.worldMatrixRotateY(0.0f);
		worldMatrices.worldMatrixRotateZ(0.0f);
	}
	void moveObject(float dX, float dY, float dZ, moveBy moveSpace)
	{	
		if(dX > 0)
			worldMatrices.worldMatrixMove(dX, dY, dZ, MOVE_RIGHT, moveSpace);
		if(dX < 0)
			worldMatrices.worldMatrixMove(dX, dY, dZ, MOVE_LEFT, moveSpace);
		if(dY > 0)
			worldMatrices.worldMatrixMove(dX, dY, dZ, MOVE_DOWN, moveSpace);
		if(dY < 0)
			worldMatrices.worldMatrixMove(dX, dY, dZ, MOVE_UP, moveSpace);
	}

	void rotateXObject(float Angle)
	{	worldMatrices.worldMatrixRotateX(Angle);}

	void rotateYObject(float Angle)
	{	worldMatrices.worldMatrixRotateY(Angle);}

	void rotateZObject(float Angle)
	{	worldMatrices.worldMatrixRotateZ(Angle);}

	void redrawObject()
	{
		switch(light.Type)
		{
		case D3DLIGHT_SPOT:
			worldMatrices.resetMatrices();
			material.resetMaterial();

			D3DXVec3Transform(&direction, &directionBuffer, &worldMatrices.getFinallyRotateMatrix());
			light.Direction = (D3DXVECTOR3)direction;

			D3DXVec3Transform(&position, &positionBuffer, &worldMatrices.getWorldMatrix());
			light.Position  = (D3DXVECTOR3)position;

			device->SetLight(lightID, &light);
			device->LightEnable(lightID, TRUE);
			device->SetStreamSource(0, vb, 0, sizeof(COVertex));
			device->SetIndices(ib);
			device->SetFVF(COVertex::FVF);
			device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				0, 0, 14, 0, 18);
			device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			break;
		case D3DLIGHT_POINT:
			worldMatrices.resetMatrices();
			material.resetMaterial();

			D3DXVec3Transform(&position, &positionBuffer, &worldMatrices.getWorldMatrix());
			light.Position  = (D3DXVECTOR3)position;

			device->SetLight(lightID, &light);
			device->LightEnable(lightID, TRUE);
			device->SetStreamSource(0, vb, 0, sizeof(COVertex));
			device->SetIndices(ib);
			device->SetFVF(COVertex::FVF);
			device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				0, 0, 14, 0, 18);
			device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			break;
		case D3DLIGHT_DIRECTIONAL:
			worldMatrices.resetMatrices();
			material.resetMaterial();

			D3DXVec3Transform(&direction, &directionBuffer, &worldMatrices.getFinallyRotateMatrix());
			light.Direction = (D3DXVECTOR3)direction;

			device->SetLight(lightID, &light);
			device->LightEnable(lightID, TRUE);
			device->SetStreamSource(0, vb, 0, sizeof(COVertex));
			device->SetIndices(ib);
			device->SetFVF(COVertex::FVF);
			device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				0, 0, 14, 0, 18);
			device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			break;
		}
	}
	void createAndFillBuffers()
	{
		device->CreateVertexBuffer(
			16 * sizeof(COVertex),
			NULL,
			COVertex::FVF,
			D3DPOOL_MANAGED,
			&vb,
			0);
		device->CreateIndexBuffer(
			64 * sizeof(WORD),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&ib,
			0);
		switch(light.Type)
		{
		case D3DLIGHT_SPOT:
			this->spotLight();
			break;
		case D3DLIGHT_POINT:
			this->pointLight();
			break;
		case D3DLIGHT_DIRECTIONAL:
			this->directionalLight();
			break;
		}
	}
	void spotLight()
	{
		vb->Lock(NULL, NULL, (void**)&vertices, D3DLOCK_DISCARD);
		vertices[0]  = COVertex(-0.2f, -0.2f, -0.2f);
		vertices[1]  = COVertex(-0.2f,  0.2f, -0.2f);
		vertices[2]  = COVertex( 0.2f,  0.2f, -0.2f);
		vertices[3]  = COVertex( 0.2f, -0.2f, -0.2f);
		vertices[4]  = COVertex(-0.2f, -0.2f,  0.2f);
		vertices[5]  = COVertex(-0.2f,  0.2f,  0.2f);
		vertices[6]  = COVertex( 0.2f,  0.2f,  0.2f);
		vertices[7]  = COVertex( 0.2f, -0.2f,  0.2f);

		vertices[8]  = COVertex( 0.0f,  0.0f,  0.0f);
		vertices[9]  = COVertex( 0.1f,  1.5f, -0.2f);
		vertices[10] = COVertex(-0.1f,  1.5f, -0.2f);
		vertices[11] = COVertex( 0.1f,  1.5f,  0.2f);
		vertices[12] = COVertex(-0.1f,  1.5f,  0.2f);
		vb->Unlock();

		ib->Lock(0, 0, (void**)&indices, D3DLOCK_DISCARD);
		// передн€€ грань
		indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;
		indices[3]  = 0; indices[4]  = 2; indices[5]  = 3;
		// задн€€ грань
		indices[6]  = 4; indices[7]  = 6; indices[8]  = 5;
		indices[9]  = 4; indices[10] = 7; indices[11] = 6;
		// лева€ грань
		indices[12] = 4; indices[13] = 5; indices[14] = 1;
		indices[15] = 4; indices[16] = 1; indices[17] = 0;
		// права€ грань
		indices[18] = 3; indices[19] = 2; indices[20] = 6;
		indices[21] = 3; indices[22] = 6; indices[23] = 7;
		// верх
		indices[24] = 1; indices[25] = 5; indices[26] = 6;
		indices[27] = 1; indices[28] = 6; indices[29] = 2;
		// низ
		indices[30] = 4; indices[31] = 0; indices[32] = 3;
		indices[33] = 4; indices[34] = 3; indices[35] = 7;
		//луч
		indices[36] = 8; indices[37] = 10; indices[38] = 9;
		indices[39] = 8; indices[40] = 11; indices[41] = 12;
		indices[42] = 8; indices[43] = 9;  indices[44] = 11;
		indices[45] = 8; indices[46] = 12; indices[47] = 10;
		indices[48] = 9; indices[49] = 12; indices[50] = 11;
		indices[51] = 9; indices[52] = 10; indices[53] = 12;
		ib->Unlock();
	}
	void pointLight()
	{
		vb->Lock(NULL, NULL, (void**)&vertices, D3DLOCK_DISCARD);
		vertices[0]  = COVertex(-0.2f, -0.2f, -0.2f);
		vertices[1]  = COVertex(-0.2f,  0.2f, -0.2f);
		vertices[2]  = COVertex( 0.2f,  0.2f, -0.2f);
		vertices[3]  = COVertex( 0.2f, -0.2f, -0.2f);
		vertices[4]  = COVertex(-0.2f, -0.2f,  0.2f);
		vertices[5]  = COVertex(-0.2f,  0.2f,  0.2f);
		vertices[6]  = COVertex( 0.2f,  0.2f,  0.2f);
		vertices[7]  = COVertex( 0.2f, -0.2f,  0.2f);
		vb->Unlock();

		ib->Lock(0, 0, (void**)&indices, D3DLOCK_DISCARD);
		// передн€€ грань
		indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;
		indices[3]  = 0; indices[4]  = 2; indices[5]  = 3;
		// задн€€ грань
		indices[6]  = 4; indices[7]  = 6; indices[8]  = 5;
		indices[9]  = 4; indices[10] = 7; indices[11] = 6;
		// лева€ грань
		indices[12] = 4; indices[13] = 5; indices[14] = 1;
		indices[15] = 4; indices[16] = 1; indices[17] = 0;
		// права€ грань
		indices[18] = 3; indices[19] = 2; indices[20] = 6;
		indices[21] = 3; indices[22] = 6; indices[23] = 7;
		// верх
		indices[24] = 1; indices[25] = 5; indices[26] = 6;
		indices[27] = 1; indices[28] = 6; indices[29] = 2;
		// низ
		indices[30] = 4; indices[31] = 0; indices[32] = 3;
		indices[33] = 4; indices[34] = 3; indices[35] = 7;
		ib->Unlock();
	}
	void directionalLight()
	{
		vb->Lock(NULL, NULL, (void**)&vertices, D3DLOCK_DISCARD);
		vertices[0]  = COVertex( 0.0f,  0.0f,  0.0f);
		vertices[1]  = COVertex( 0.1f,  1.5f, -0.2f);
		vertices[2] = COVertex(-0.1f,  1.5f, -0.2f);
		vertices[3] = COVertex( 0.1f,  1.5f,  0.2f);
		vertices[4] = COVertex(-0.1f,  1.5f,  0.2f);
		vb->Unlock();
		
		ib->Lock(0, 0, (void**)&indices, D3DLOCK_DISCARD);
		indices[0]  = 0; indices[1]  = 2; indices[2]  = 1;
		indices[3]  = 0; indices[4]  = 3; indices[5]  = 4;
		indices[6]  = 0; indices[7]  = 1; indices[8]  = 3;
		indices[9]  = 0; indices[10] = 4; indices[11] = 2;
		indices[12] = 1; indices[13] = 4; indices[14] = 3;
		indices[15] = 1; indices[16] = 2; indices[17] = 4;
		ib->Unlock();
	}
	void setLight()
	{
		device->SetLight(lightID, &light);
	}

	LPCTSTR getLightName()
	{	return lightName;}
	DWORD getLightID()
	{
		return lightID;
	}
	void enableLight()
	{	device->LightEnable(lightID, true);}
	void disableLight()
	{	device->LightEnable(lightID, false);}
	D3DLIGHT9* getLight()
	{	return &light;}
	light_class* getThis()
	{
		return this;
	}
	~light_class()
	{
		if(light.Type == D3DLIGHT_SPOT)
		{
			vb->Release();
			ib->Release();
		}
	}
};