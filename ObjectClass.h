
enum interType{interObject, interVertex};

class object_class
{
private:
	IDirect3DDevice9* device;
	botStatusBar_Class* BSBar;
	rendStateTypes_class* rendStateTypes;
	DWORD pickedSubset;
	HWND subsetsList;

	DWORD ObjectID;
	char buffer[256];
	LPCTSTR objectName;
	ray_struct rayOfPicking;

	ID3DXMesh* mesh;
	worldMatrices_class worldMatrices;

	vertex* vertices;
	WORD* indices;
	DWORD* attributeBuffer;

	DWORD* adjacencyInfo;
	DWORD* optimizedAdjacencyInfo;
	D3DXATTRIBUTERANGE* attributeTable; 

	material_class* material[64];
	texture_class* texture[64];
	rendState_class* rendState[64];
	sphere_struct* vertexSphere[512];
	triangle* triangles[512];
	IDirect3DVertexBuffer9* vpb;
	particle* particles;
	DWORD numPickedVerts;

	DWORD numSubsetsAttributes;
	DWORD numSubsets;
	DWORD numCreatedFaces;
	DWORD numCreatedVerts;

	DWORD numMaterials;
	DWORD numTextures;
	DWORD numRendState;

public:
	void initObjectBase(IDirect3DDevice9* bDevice, DWORD numObject, HWND bSubsetsList, botStatusBar_Class* bBSBar,
		rendStateTypes_class* bRendStateTypes)
	{
		device = bDevice;
		BSBar = bBSBar;
		rendStateTypes = bRendStateTypes;
		ObjectID = numObject;
		subsetsList = bSubsetsList;
		objectName = L"Object";	
		numSubsets      = 0;
		pickedSubset    = -1;
		numMaterials	= 0;
		numTextures		= 0;
		numRendState    = 0;
		numCreatedVerts = 0;
		numCreatedFaces = 0;
		numPickedVerts  = 0;

		D3DXCreateMeshFVF(256, 512, D3DXMESH_MANAGED, vertex::FVF, device, &mesh);

		device->CreateVertexBuffer(
			512,						//������� �������� �� 65 ������!
			D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
			particle::FVF,
			D3DPOOL_DEFAULT,
			&vpb,
			0);
	
		this->baseObject();

		DWORD numVertices = mesh->GetNumVertices();
		DWORD numFaces = mesh->GetNumFaces();

		adjacencyInfo = new DWORD [numFaces * 3];
		mesh->GenerateAdjacency(0.0001f, adjacencyInfo);

		D3DXComputeNormals(mesh, adjacencyInfo);

		mesh->GetAttributeTable(0, &numSubsetsAttributes);
		attributeTable = new D3DXATTRIBUTERANGE [numSubsetsAttributes];
		mesh->GetAttributeTable(attributeTable, &numSubsetsAttributes);

		worldMatrices.fillMatrix(0, 0, 0, device);
		worldMatrices.worldMatrixRotateX(0.0f);		//��������� ����� ������� � 0 (��� ����������� ���������� �����������)
		worldMatrices.worldMatrixRotateY(0.0f);
		worldMatrices.worldMatrixRotateZ(0.0f);
	}

	void baseObject()
	{
		this->createNewFoundation();
		
		material[numMaterials] = new material_class;
		material[numMaterials]->initMaterialBase(device, numMaterials);

		texture[numTextures] = new texture_class;
		texture[numTextures]->initBaseForTexture(device);

		rendState[numRendState] = new rendState_class;
		rendState[numRendState]->initRendStateClass(device, rendStateTypes);
		rendState[numRendState]->addRendState(0, 2);
		rendState[numRendState]->addRendState(1, 0);
		rendState[numRendState]->addRendState(2, 0);
		rendState[numRendState]->addRendState(3, 0);

		SendMessage(subsetsList, LB_INSERTSTRING, numSubsets, (LPARAM)(LPCTSTR)L"Subset");
		pickedSubset = numSubsets;

		numSubsets	 += 1;
		numMaterials += 1;
		numTextures  += 1;
		numRendState += 1;
	}

	void createNewFoundation()
	{
		mesh->LockVertexBuffer(0, (void**)&vertices);
		vertices[numCreatedVerts  ] = vertex( -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		vertices[numCreatedVerts+1] = vertex(  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);	
		vertices[numCreatedVerts+2] = vertex( -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		mesh->UnlockVertexBuffer();

		mesh->LockIndexBuffer(0, (void**)&indices);
		indices[numCreatedVerts  ] = numCreatedVerts;
		indices[numCreatedVerts+1] = numCreatedVerts+1; 
		indices[numCreatedVerts+2] = numCreatedVerts+2;
		mesh->UnlockIndexBuffer();

		mesh->LockAttributeBuffer(0, &attributeBuffer); 
		attributeBuffer[numCreatedFaces] = numSubsets; 
		mesh->UnlockAttributeBuffer();

		vpb->Lock(NULL, NULL, (void**)&particles, NULL);
		for(short i(0); i != 3;)
		{
			vertexSphere[numCreatedVerts+i] = new sphere_struct;
			*vertexSphere[numCreatedVerts+i] = sphere_struct(vertices[numCreatedVerts+i].pos, numCreatedVerts+i, numCreatedFaces*3+i, 
				0.3f, FALSE, numCreatedVerts+i, Father);

			particles[numCreatedVerts+i] = particle(vertices[numCreatedVerts+i].pos, 0.5f);
			i++;
		}
		vpb->Unlock();

		triangles[numCreatedFaces] = new triangle;
		*triangles[numCreatedFaces] = triangle(Full, numCreatedVerts, numCreatedVerts+1, numCreatedVerts+2, 
			numCreatedFaces, numSubsets, numMaterials, numCreatedVerts, numCreatedVerts+1, numCreatedVerts+2);

		numCreatedVerts += 3;
		numCreatedFaces += 1;
	}

	void createNewTriangle()
	{
		bool found[3] = {FALSE, FALSE, FALSE};
		DWORD vertsID[3];
		
		this->findOneRelevantVert(found, vertsID, 3);

		if(found[0] == TRUE && found[1] == TRUE && found[2] == TRUE)
		{
			mesh->LockVertexBuffer(0, (void**)&vertices);
			vertices[numCreatedVerts  ] = vertices[vertsID[0]];
			vertices[numCreatedVerts+1] = vertices[vertsID[1]];
			vertices[numCreatedVerts+2] = vertices[vertsID[2]];
			mesh->UnlockVertexBuffer();

			mesh->LockIndexBuffer(0, (void**)&indices);
			indices[numCreatedVerts  ] = numCreatedVerts;
			indices[numCreatedVerts+1] = numCreatedVerts+1; 
			indices[numCreatedVerts+2] = numCreatedVerts+2;
			mesh->UnlockIndexBuffer();

			mesh->LockAttributeBuffer(0, &attributeBuffer); 
			attributeBuffer[numCreatedFaces] = numSubsets-1; 
			mesh->UnlockAttributeBuffer();

			vpb->Lock(NULL, NULL, (void**)&particles, NULL);
			for(short i(0); i != 3;)
			{
				vertexSphere[numCreatedVerts+i] = new sphere_struct;
				*vertexSphere[numCreatedVerts+i] = sphere_struct(vertices[numCreatedVerts+i].pos, numCreatedVerts+i, numCreatedFaces*3+i, 
					0.3f, FALSE, numCreatedVerts+i, Child);

				vertexSphere[vertsID[i]]->linkVert(numCreatedVerts+i);

				particles[numCreatedVerts+i] = particle(vertices[numCreatedVerts+i].pos, 0.5f);
				i++;
			}
			vpb->Unlock();

			triangles[numCreatedFaces] = new triangle;
			*triangles[numCreatedFaces] = triangle(Full, numCreatedVerts, numCreatedVerts+1, numCreatedVerts+2, 
				numCreatedFaces, numSubsets-1, numMaterials-1, vertsID[0], vertsID[1], vertsID[2]);

			mesh->GenerateAdjacency(0.001f, adjacencyInfo);
			D3DXComputeNormals(mesh, adjacencyInfo);

			numCreatedVerts += 3;
			numCreatedFaces += 1;
		}
		else
		{
			if(found[0] == TRUE && found[1] == TRUE)
			{
				mesh->LockVertexBuffer(0, (void**)&vertices);
				vertices[numCreatedVerts]   = vertex( 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f); //New Vertex
				vertices[numCreatedVerts+1] = vertices[vertsID[1]];
				vertices[numCreatedVerts+2] = vertices[vertsID[0]];
				mesh->UnlockVertexBuffer();

				mesh->LockIndexBuffer(0, (void**)&indices);
				indices[numCreatedVerts  ] = numCreatedVerts;
				indices[numCreatedVerts+1] = numCreatedVerts+1; 
				indices[numCreatedVerts+2] = numCreatedVerts+2;
				mesh->UnlockIndexBuffer();

				mesh->LockAttributeBuffer(0, &attributeBuffer); 
				attributeBuffer[numCreatedFaces] = numSubsets-1; 
				mesh->UnlockAttributeBuffer();

				vertexSphere[numCreatedVerts] = new sphere_struct;
				*vertexSphere[numCreatedVerts] = sphere_struct(vertices[numCreatedVerts].pos, numCreatedVerts, numCreatedFaces*3, 
					0.3f, FALSE, numCreatedVerts, Father);

				vertexSphere[numCreatedVerts+1] = new sphere_struct;
				*vertexSphere[numCreatedVerts+1] = sphere_struct(vertices[numCreatedVerts+1].pos, numCreatedVerts+1, numCreatedFaces*3+1, 
					0.3f, FALSE, numCreatedVerts+1, Child);

				vertexSphere[numCreatedVerts+2] = new sphere_struct;
				*vertexSphere[numCreatedVerts+2] = sphere_struct(vertices[numCreatedVerts+2].pos, numCreatedVerts+2, numCreatedFaces*3+2, 
					0.3f, FALSE, numCreatedVerts+2, Child);

				vertexSphere[vertsID[1]]->linkVert(numCreatedVerts+1);
				vertexSphere[vertsID[0]]->linkVert(numCreatedVerts+2);

				vpb->Lock(NULL, NULL, (void**)&particles, NULL);
				for(short i(0); i != 3;)
				{
					particles[numCreatedVerts+i] = particle(vertices[numCreatedVerts+i].pos, 0.5f);
					i++;
				}
				vpb->Unlock();

				triangles[numCreatedFaces] = new triangle;
				*triangles[numCreatedFaces] = triangle(Full, numCreatedVerts, numCreatedVerts+1, numCreatedVerts+2, 
					numCreatedFaces, numSubsets-1, numMaterials-1, numCreatedVerts, vertsID[1], vertsID[0]);

				mesh->GenerateAdjacency(0.001f, adjacencyInfo);
				D3DXComputeNormals(mesh, adjacencyInfo);

				numCreatedVerts += 3;
				numCreatedFaces += 1;
			}
		}
	}

	void LoadTexture4Verts()
	{
		DWORD vertsID[4];
		DWORD textureVerts[4];
		bool found[4];
		this->findOneRelevantVert(found, textureVerts, 4);
		if(found[0] == TRUE && found[1] == TRUE && found[2] == TRUE && found[3] == TRUE)
		{
			texture[pickedSubset]->loadTexture4Verts(textureVerts, vertices, vertexSphere);
		}
	}

	void swapIndices()
	{
		bool found[72];
		DWORD vertsID[72];

		this->findOneRelevantVert(found, vertsID, 3);

		DWORD relevantTriangle[24];
		DWORD numRelevantTris = this->findRelevantTriangle(vertsID, relevantTriangle, 3);

		if(numPickedVerts >= 3)
		{
			DWORD indexBuffer;										
				
			mesh->LockIndexBuffer(0, (void**)&indices);
			for(DWORD counter(0); counter != numRelevantTris;)
			{
				indexBuffer = indices[relevantTriangle[counter]*3+1];					//������ ������� ������ � ������ ������
				indices[relevantTriangle[counter]*3+1] = indices[relevantTriangle[counter]*3+2];
				indices[relevantTriangle[counter]*3+2] = indexBuffer;
				counter += 1;
			}
			mesh->UnlockIndexBuffer();

			mesh->GenerateAdjacency(0.001f, adjacencyInfo);
			D3DXComputeNormals(mesh, adjacencyInfo);
		}
	}

	void newSubset()
	{
		bool found[72];
		DWORD vertsID[72];

		this->findOneRelevantVert(found, vertsID, 3);

		if(found[0] == TRUE && found[1] == TRUE && found[2] == TRUE)   //��� ������� �������
		{
			DWORD relevantTriangle[24];
			DWORD numRelevantTris = this->findRelevantTriangle(vertsID, relevantTriangle, 3);

			if(numPickedVerts >= 3)
			{
				mesh->LockAttributeBuffer(0, &attributeBuffer); 
				for(DWORD counter(0); counter != numRelevantTris;)
				{
					attributeBuffer[relevantTriangle[counter]] = numSubsets; 

					triangles[relevantTriangle[counter]]->subsetID   = numSubsets;
					triangles[relevantTriangle[counter]]->materialID = numMaterials;
					counter += 1;
				}
				mesh->UnlockAttributeBuffer();

				material[numSubsets] = new material_class;
				material[numSubsets]->initAbsolutelyWhiteMaterial(device, numMaterials);

				texture[numSubsets] = new texture_class;
				texture[numSubsets]->initBaseForTexture(device);

				rendState[numRendState] = new rendState_class;
				rendState[numRendState]->initRendStateClass(device, rendStateTypes);
				rendState[numRendState]->addRendState(0, 2);
				rendState[numRendState]->addRendState(1, 0);
				rendState[numRendState]->addRendState(2, 0);
				rendState[numRendState]->addRendState(3, 0);

				SendMessage(subsetsList, LB_INSERTSTRING, numSubsets, (LPARAM)(LPCTSTR)L"Subset");
			
				pickedSubset = numSubsets;
				numSubsets	 += 1;
				numMaterials += 1;
				numTextures  += 1;
				numRendState += 1;
			}
		}
	}

	void rewriteSubsetsList()
	{
		SendMessage(subsetsList, LB_RESETCONTENT, 0, 0);

		for(DWORD counter(0); counter != numSubsets;)
		{
			SendMessage(subsetsList, LB_INSERTSTRING, counter, (LPARAM)(LPCTSTR)L"Subset");
			counter++;
		}
	}

	void moveObject(float dX, float dY, float dZ)
	{	
		if(dX > 0)
			worldMatrices.worldMatrixMove(dX, dY, dZ, MOVE_RIGHT);
		if(dX < 0)
			worldMatrices.worldMatrixMove(dX, dY, dZ, MOVE_LEFT);
		if(dY > 0)
			worldMatrices.worldMatrixMove(dX, dY, dZ, MOVE_DOWN);
		if(dY < 0)
			worldMatrices.worldMatrixMove(dX, dY, dZ, MOVE_UP);
	}

	void moveVertex(DWORD vertexNumber, float dX, float dY, float dZ)
	{
		vertexNumber--;
	
		if(vertexSphere[vertexNumber]->vertType == Father)
		{
			mesh->LockVertexBuffer(0, (void**)&vertices);
			if(dX > 0)
				worldMatrices.moveVert(&vertices[vertexNumber].pos, dX, dY, dZ, MOVE_RIGHT);
			if(dX < 0)
				worldMatrices.moveVert(&vertices[vertexNumber].pos, dX, dY, dZ, MOVE_LEFT);
			if(dY > 0)
				worldMatrices.moveVert(&vertices[vertexNumber].pos, dX, dY, dZ, MOVE_DOWN);
			if(dY < 0)
				worldMatrices.moveVert(&vertices[vertexNumber].pos, dX, dY, dZ, MOVE_UP);

			particles[vertexNumber].pos = vertexSphere[vertexNumber]->center = vertices[vertexNumber].pos;

			for(DWORD counter(0); counter != vertexSphere[vertexNumber]->numLinkedVerts;)
			{
				particles[vertexSphere[vertexNumber]->linkedVertices[counter]].pos = 
				vertexSphere[vertexSphere[vertexNumber]->linkedVertices[counter]]->center = 
				vertices[vertexSphere[vertexNumber]->linkedVertices[counter]].pos =
				vertexSphere[vertexNumber]->center;
				counter++;
			}
			BSBar->writeVertPos(&vertices[vertexNumber].pos);
			mesh->UnlockVertexBuffer();	
		}
	}

	void cutVertices()
	{
		bool found;
		DWORD vertsID;

		this->findOneRelevantVert(&found, &vertsID, 1);
		if(found == TRUE && vertexSphere[vertsID]->numLinkedVerts != 0)
		{
			for(DWORD counter(0); counter != vertexSphere[vertsID]->numLinkedVerts;)
			{
				vertexSphere[vertexSphere[vertsID]->linkedVertices[counter]]->vertType = Father;

				vertexSphere[vertsID]->linkedVertices[counter] = 0;
				counter += 1;
			}

			DWORD relevantTris[24];
			DWORD numFoundTris = this->findRelevantTriangle(&vertsID, relevantTris, 1);
			for(DWORD counter_2(0); counter_2 != numFoundTris;)
			{
				for(DWORD counter_3(0); counter_3 != 3;)
				{
					if(triangles[relevantTris[counter_2]]->fathers[counter_3] == vertsID)
					{
						triangles[relevantTris[counter_2]]->fathers[counter_3] = triangles[relevantTris[counter_2]]->verticesID[counter_3];
					}
					counter_3 += 1;
				}
				counter_2 += 1;
			}
			vertexSphere[vertsID]->numLinkedVerts = 0;
		}
	}
	void uniteVertices()
	{
		bool found[2];
		DWORD vertsID[2];	

		this->findOneRelevantVert(found, vertsID, 2);
		if(found[0] == TRUE && found[1] == TRUE)
		{
			vertexSphere[vertsID[0]]->linkVert(vertsID[1]);		//� ������� vertsID[0] ������������ ������� vertsID[1]
			vertexSphere[vertsID[1]]->vertType = Child;			//�������� ������� vertsID[1]
			vertexSphere[vertsID[1]]->isPicked = FALSE;

			particles[vertsID[1]].pos = vertexSphere[vertsID[1]]->center = vertices[vertsID[1]].pos;

			for(DWORD counter(0); counter != vertexSphere[vertsID[1]]->numLinkedVerts;)
			{
				vertexSphere[vertsID[0]]->linkVert(vertexSphere[vertsID[1]]->linkedVertices[counter]);
				vertexSphere[vertsID[1]]->linkedVertices[counter] = 0;
				vertexSphere[vertexSphere[vertsID[1]]->linkedVertices[counter]]->isPicked = FALSE;

				particles[vertexSphere[vertsID[1]]->linkedVertices[counter]].pos = 
				vertexSphere[vertexSphere[vertsID[1]]->linkedVertices[counter]]->center = 
				vertices[vertexSphere[vertsID[1]]->linkedVertices[counter]].pos =
				vertexSphere[vertsID[1]]->center;

				counter += 1;
			}
			DWORD relevantTris_1[24];
			DWORD numFoundTris_1 = this->findRelevantTriangle(&vertsID[0], relevantTris_1, 1);
			DWORD relevantTris_2[24];
			DWORD numFoundTris_2 = this->findRelevantTriangle(&vertsID[1], relevantTris_2, 1);
			for(DWORD counter(0); counter != numFoundTris_2;)
			{
				for(DWORD counter_2(0); counter_2 != 3;)
				{
					if(triangles[relevantTris_2[counter]]->fathers[counter_2] == vertsID[1])
					{
						/**********/
						for(DWORD counter_3(0); counter_3 != numFoundTris_1;)
						{
							for(DWORD counter_4(0); counter_4 != 3;)
							{
								if(triangles[relevantTris_1[counter_3]]->fathers[counter_4] == vertsID[0])
								{
									triangles[relevantTris_2[counter]]->fathers[counter_2] = 
									triangles[relevantTris_1[counter_3]]->fathers[counter_4];
								}
								counter_4 += 1;
							}
							counter_3 += 1;
						}
						/**********/
					}
					counter_2 += 1;
				}
				counter += 1;
			}
			//����� ��� fathers �������������, ���������� vertsID[1], ���������� � ���������������� father ������������, ����������� vertsID[0]  
			
			


			vertexSphere[vertsID[1]]->numLinkedVerts = 0;
		}
	}
	void rotateXObject(float Angle)
	{	worldMatrices.worldMatrixRotateX(Angle);}

	void rotateYObject(float Angle)
	{	worldMatrices.worldMatrixRotateY(Angle);}

	void rotateZObject(float Angle)
	{	worldMatrices.worldMatrixRotateZ(Angle);}

	void pickSubset(DWORD subsetNumber)
	{
		pickedSubset = subsetNumber;
	}
	DWORD checkIntersection(ray_struct clickRay, interType intersectedType)
	{
		BOOL  hit;	DWORD pFaceIndex; FLOAT pU; FLOAT pV; FLOAT pDist;
		D3DXMATRIX worldMatrix = worldMatrices.getFinallyWorldMatrix();
		
		D3DXMatrixInverse(&worldMatrix, NULL, &worldMatrix);
		D3DXVec3TransformCoord(&clickRay.origin, &clickRay.origin, &worldMatrix);
		D3DXVec3TransformNormal(&clickRay.direction, &clickRay.direction, &worldMatrix);
		D3DXVec3Normalize(&clickRay.direction, &clickRay.direction);

		if(intersectedType == interVertex)
		{
			for(int counter = 0; counter != numCreatedVerts;)
			{
				D3DXVECTOR3 v = clickRay.origin - vertexSphere[counter]->center;

				float b = 2.0f * D3DXVec3Dot(&clickRay.direction, &v);
				float c = D3DXVec3Dot(&v, &v) - (vertexSphere[counter]->radius * vertexSphere[counter]->radius);

				float discriminant = (b * b) - (4.0f * c);

				if(discriminant > 0.0f)
				{
					discriminant = sqrtf(discriminant);

					float s0 = (-b + discriminant) / 2.0f;
					float s1 = (-b - discriminant) / 2.0f;

					if(s0 >= 0.0f || s1 >= 0.0f)
					{
						if(vertexSphere[counter]->isPicked == FALSE)
						{
							numPickedVerts++;
							BSBar->writeNumPickedVerts(numPickedVerts);
							vertexSphere[counter]->isPicked = TRUE;
						}
						else
						{
							numPickedVerts--;
							BSBar->writeNumPickedVerts(numPickedVerts);
							vertexSphere[counter]->isPicked = FALSE;
						}
						
						if(numPickedVerts == 3)
						{
							bool found[3] = {FALSE, FALSE, FALSE};
							DWORD vertsID[3];

							this->findOneRelevantVert(found, vertsID, 3);
	
							if(found[0] == TRUE && found[1] == TRUE && found[2] == TRUE)   //��� ������� �������
							{
								DWORD relevantTriangle[24];
								DWORD numRelevantTris = this->findRelevantTriangle(vertsID, relevantTriangle, 3);

								if(numPickedVerts == 3)
								{
									for(DWORD counter(0); counter != numRelevantTris;)
									{
										pickedSubset = triangles[relevantTriangle[counter]]->subsetID;
										counter += 1;
									}
								}
							}
						}
						if(vertexSphere[counter]->vertType == Father)
							return vertexSphere[counter]->vertexID+1;
					}
				}
				counter++;
			}
			return 0;
		}

		if(intersectedType == interObject)
		{
			D3DXIntersect(mesh, &clickRay.origin, &clickRay.direction,
				&hit, &pFaceIndex, &pU, &pV, &pDist, NULL, NULL);
	
			if(hit == TRUE)
				return ObjectID + 1;
			else
				return 0;
		}
	}

	void unpickAllVerts()
	{
		for(DWORD counter(0); counter != numCreatedVerts;)
		{
			vertexSphere[counter]->isPicked = FALSE;
			counter += 1;
		}
		numPickedVerts = 0;
	}
	D3DMATERIAL9* getMaterial()
	{
		return material[pickedSubset]->getMaterial();
	}

	rendState_class* getRendState()
	{
		return rendState[pickedSubset];
	}

	void renameObject(HWND objectsList, HWND nameEditor, UINT objectNumber)
	{
		SendMessage(nameEditor, EM_LIMITTEXT, (WPARAM)80, NULL);
		SendMessage(nameEditor, EM_GETLINE, 0, (LPARAM)buffer);
		
		objectName = (LPCTSTR)buffer;
		
		SendMessage(objectsList, LB_DELETESTRING, (WPARAM)objectNumber, NULL);
		SendMessage(objectsList, LB_INSERTSTRING, (WPARAM)objectNumber, (LPARAM)objectName);
	}

	void findOneRelevantVert(bool found[], DWORD vertsID[], DWORD numNeededVerts)
	{
		int counter(0);
		for(short i(0); i != numNeededVerts;)
		{
			for(;counter != numCreatedVerts;)
			{
				if(vertexSphere[counter]->isPicked)
				{
					vertsID[i] = vertexSphere[counter]->startVertex;
					found[i] = TRUE;
					counter++;
					break;
				}
				counter++;
			}
			i++;
		}
	}
	DWORD findRelevantTriangle(DWORD vertsID[], DWORD relevantTriangle[], DWORD numNeededRelevant)
	{
		DWORD relevantVerts(0);
		DWORD numRelevantTris(0);
		for(DWORD counter(0); counter != numCreatedFaces;)
		{
			for(short counter_2(0); counter_2 != 3;)
			{
				if(triangles[counter]->fathers[0] == vertsID[counter_2])
					relevantVerts += 1;
				if(triangles[counter]->fathers[1] == vertsID[counter_2])
					relevantVerts += 1;
				if(triangles[counter]->fathers[2] == vertsID[counter_2])
					relevantVerts += 1;

				if(relevantVerts == numNeededRelevant)
				{	relevantTriangle[numRelevantTris] = counter;
					numRelevantTris += 1;
					break;}
				counter_2 += 1;
			}
			relevantVerts = 0;
			counter += 1;
		}
		return numRelevantTris;
	}
	LPCTSTR getObjectName()
	{	return objectName;}
	DWORD getObjectID()
	{	return ObjectID;}

	ID3DXMesh* getMehs()
	{	return mesh;}
	vertex* getVertices()
	{	return vertices;}
	WORD* getIndices()
	{	return indices;}
	DWORD getNumVerts()
	{	return numCreatedVerts;}
	DWORD getNumFaces()
	{	return numCreatedFaces;}
	DWORD* getAttributes()
	{	return attributeBuffer;}
	DWORD* getAdjacencyInfo()
	{	return adjacencyInfo;}
	triangle** getTriangles()
	{
		return triangles;
	}
	void saveObjectAs(HINSTANCE bhInstace, HWND bWindow)
	{
		saveAs(bhInstace, bWindow, this);
	}
	void saveObject()
	{
		//saveFullObject(this);
	}
	void redraw()
	{
		worldMatrices.resetMatrices();
	
		device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
		//device->SetRenderState(D3DRS_POINTSIZE, flToDw(2.5f));
		//device->SetRenderState(D3DRS_POINTSIZE_MIN, flToDw(0.2f));
		device->SetRenderState(D3DRS_POINTSIZE_MAX, flToDw(5.0f));
		device->SetRenderState(D3DRS_LIGHTING, FALSE);

		device->SetFVF(particle::FVF);
		device->SetStreamSource(0, vpb, 0, sizeof(particle));

		for(DWORD vert = 0; vert != numCreatedVerts; vert++)
		{
			if(vertexSphere[vert]->isPicked == TRUE)
			{
				device->DrawPrimitive(D3DPT_POINTLIST, vertexSphere[vert]->startVertex, 1);
			}
		}

		for(DWORD subset(0); subset != numSubsets; subset++)
		{
			if(numMaterials)
				material[subset]->resetMaterial();
			if(numTextures)
				texture[subset]->resetTexture();
			if(numRendState)
				rendState[subset]->setRendState();

			mesh->DrawSubset(subset);
		}
	}

	DWORD getNumMaterials()
	{
		return numMaterials;
	}

	material_class* getMaterialClass(DWORD materialNumber)
	{
		return material[materialNumber];
	}

	~object_class()
	{	
		//saveFullObject(this);

		for(;numMaterials != 0;)
		{
			numMaterials--;
			//saveFullMaterial(material[numMaterials]->getThis());
			delete material[numMaterials];
			material[numMaterials] = NULL;
		}

		for(;numRendState != 0;)
		{
			numRendState -= 1;
			delete rendState[numRendState];
			rendState[numRendState] = NULL;
		}

		for(;numTextures != 0;)
		{
			numTextures--;
			delete texture[numTextures];
			texture[numTextures] = NULL;
		}

		for(;numCreatedFaces != 0;)
		{
			numCreatedFaces--;
			delete triangles[numCreatedFaces];
			triangles[numCreatedFaces] = NULL;
		}

		for(;numCreatedVerts != 0;)
		{
			numCreatedVerts--;
			delete vertexSphere[numCreatedVerts];
			vertexSphere[numCreatedVerts] = NULL;
		}

		vpb->Release();

		delete adjacencyInfo;
		adjacencyInfo			 = NULL;
		delete optimizedAdjacencyInfo;
		optimizedAdjacencyInfo   = NULL;
		delete attributeTable;
		attributeTable			 = NULL;

		mesh->Release();
	}
};