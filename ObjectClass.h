
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

	std::vector<material_class> vecMaterials;
	std::vector<texture_class> vecTextures;
	std::vector<rendState_class> vecRendState;
	rendState_class* rendState[64];
	std::vector<sphere_struct> vecVertexSphere;
	std::vector<triangle> vecTriangles;
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
		rendStateTypes_class* bRendStateTypes, bool isVoid)
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
			2048,						//Хватает примерно на 65 вершин!
			D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
			particle::FVF,
			D3DPOOL_DEFAULT,
			&vpb,
			0);
	
		if(isVoid == false)
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
		worldMatrices.worldMatrixRotateX(0.0f);		//Установка углов наклона в 0 (для правильного начального отображения)
		worldMatrices.worldMatrixRotateY(0.0f);
		worldMatrices.worldMatrixRotateZ(0.0f);
	}

	void baseObject()
	{
		this->createNewFoundation();
		
		vecMaterials.push_back(material_class());
		vecMaterials.at(numMaterials).initMaterialBase(device, numMaterials);

		vecTextures.push_back(texture_class()); 
		vecTextures.at(numTextures).initBaseForTexture(device);

		rendState[numRendState] = new rendState_class;
		rendState[numRendState]->initRendStateClass(device, rendStateTypes);

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
			vecVertexSphere.push_back(sphere_struct(vertices[numCreatedVerts+i].pos, numCreatedVerts+i, numCreatedFaces*3+i, 
				0.3f, FALSE, numCreatedVerts+i, Father));

			particles[numCreatedVerts+i] = particle(vertices[numCreatedVerts+i].pos, 0.5f);
			i++;
		}
		vpb->Unlock();

		vecTriangles.push_back(triangle(Full, numCreatedVerts, numCreatedVerts+1, numCreatedVerts+2, 
			numCreatedFaces, numSubsets, numMaterials, numCreatedVerts, numCreatedVerts+1, numCreatedVerts+2));

		numCreatedVerts += 3;
		numCreatedFaces += 1;
	}

	void createNewTriangle()
	{
		bool found[3] = {FALSE, FALSE, FALSE};
		DWORD vertsID[3];
		
		this->findPickedVerts(found, vertsID, 3);

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
				vecVertexSphere.push_back(sphere_struct(vertices[numCreatedVerts+i].pos, numCreatedVerts+i, numCreatedFaces*3+i, 
					0.3f, FALSE, numCreatedVerts+i, Child));
				vecVertexSphere.at(vertsID[i]).linkVert(numCreatedVerts+i);

				particles[numCreatedVerts+i] = particle(vertices[numCreatedVerts+i].pos, 0.5f);
				i++;
			}
			vpb->Unlock();

			vecTriangles.push_back(triangle(Full, numCreatedVerts, numCreatedVerts+1, numCreatedVerts+2, 
				numCreatedFaces, numSubsets-1, numMaterials-1, vertsID[0], vertsID[1], vertsID[2]));

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

				vecVertexSphere.push_back(sphere_struct(vertices[numCreatedVerts].pos, numCreatedVerts, numCreatedFaces*3, 
					0.3f, FALSE, numCreatedVerts, Father));

				vecVertexSphere.push_back(sphere_struct(vertices[numCreatedVerts+1].pos, numCreatedVerts+1, numCreatedFaces*3+1, 
					0.3f, FALSE, numCreatedVerts+1, Child));

				vecVertexSphere.push_back(sphere_struct(vertices[numCreatedVerts+2].pos, numCreatedVerts+2, numCreatedFaces*3+2, 
					0.3f, FALSE, numCreatedVerts+2, Child));

				vecVertexSphere.at(vertsID[1]).linkVert(numCreatedVerts+1);
				vecVertexSphere.at(vertsID[0]).linkVert(numCreatedVerts+2);

				vpb->Lock(NULL, NULL, (void**)&particles, NULL);
				for(short i(0); i != 3;)
				{
					particles[numCreatedVerts+i] = particle(vertices[numCreatedVerts+i].pos, 0.5f);
					i++;
				}
				vpb->Unlock();

				vecTriangles.push_back(triangle(Full, numCreatedVerts, numCreatedVerts+1, numCreatedVerts+2, 
					numCreatedFaces, numSubsets-1, numMaterials-1, numCreatedVerts, vertsID[1], vertsID[0]));

				mesh->GenerateAdjacency(0.001f, adjacencyInfo);
				D3DXComputeNormals(mesh, adjacencyInfo);

				numCreatedVerts += 3;
				numCreatedFaces += 1;
			}
		}
	}

	void LoadTexture4Verts()				// Переделать!
	{
		DWORD vertsID[4];
		DWORD textureVerts[4];
		bool found[4];
		this->findPickedVerts(found, textureVerts, 4);
		if(found[0] == TRUE && found[1] == TRUE && found[2] == TRUE && found[3] == TRUE)
		{
			//texture[pickedSubset]->loadTexture4Verts(textureVerts, vertices, vertexSphere);
		}
	}

	void swapIndices()
	{
		bool found[72];
		DWORD vertsID[72];

		this->findPickedVerts(found, vertsID, 3);

		DWORD relevantTriangle[24];
		DWORD numRelevantTris = this->findRelevantTriangles(vertsID, relevantTriangle, 3);

		if(numPickedVerts >= 3)
		{
			DWORD indexBuffer;										
				
			mesh->LockIndexBuffer(0, (void**)&indices);
			for(DWORD counter(0); counter != numRelevantTris;)
			{
				indexBuffer = indices[relevantTriangle[counter]*3+1];					//Меняем местами второй и третий индекс
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

		this->findPickedVerts(found, vertsID, 3);

		if(found[0] == TRUE && found[1] == TRUE && found[2] == TRUE)   //Три вершины найдены
		{
			DWORD relevantTriangle[24];
			DWORD numRelevantTris = this->findRelevantTriangles(vertsID, relevantTriangle, 3);

			if(numPickedVerts >= 3)
			{
				mesh->LockAttributeBuffer(0, &attributeBuffer); 
				for(DWORD counter(0); counter != numRelevantTris;)
				{
					attributeBuffer[relevantTriangle[counter]] = numSubsets; 

					vecTriangles.at(relevantTriangle[counter]).subsetID   = numSubsets;
					vecTriangles.at(relevantTriangle[counter]).materialID = numMaterials;
					counter += 1;
				}
				mesh->UnlockAttributeBuffer();

				vecMaterials.push_back(material_class());
				vecMaterials.at(numSubsets).initAbsolutelyWhiteMaterial(device, numMaterials);

				vecTextures.push_back(texture_class());
				vecTextures.at(numSubsets).initBaseForTexture(device);

				rendState[numRendState] = new rendState_class;
				rendState[numRendState]->initRendStateClass(device, rendStateTypes);

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

	void moveVertex(DWORD vertexNumber, float dX, float dY, float dZ)
	{
		vertexNumber--;
	
		if(vecVertexSphere.at(vertexNumber).vertType == Father)
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

			particles[vertexNumber].pos = vecVertexSphere.at(vertexNumber).center = vertices[vertexNumber].pos;

			for(DWORD counter(0); counter != vecVertexSphere.at(vertexNumber).numLinkedVerts;)
			{
				particles[vecVertexSphere.at(vertexNumber).linkedVertices[counter]].pos = 
				vecVertexSphere.at(vecVertexSphere.at(vertexNumber).linkedVertices[counter]).center = 
				vertices[vecVertexSphere.at(vertexNumber).linkedVertices[counter]].pos =
				vecVertexSphere.at(vertexNumber).center;
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

		this->findPickedVerts(&found, &vertsID, 1);
		if(found == TRUE && vecVertexSphere.at(vertsID).numLinkedVerts != 0)
		{
			for(DWORD counter(0); counter != vecVertexSphere.at(vertsID).numLinkedVerts;)
			{
				vecVertexSphere.at(vecVertexSphere.at(vertsID).linkedVertices[counter]).vertType = Father;

				vecVertexSphere.at(vertsID).linkedVertices[counter] = 0;
				counter += 1;
			}

			DWORD relevantTris[24];
			DWORD numFoundTris = this->findRelevantTriangles(&vertsID, relevantTris, 1);
			for(DWORD counter_2(0); counter_2 != numFoundTris;)
			{
				for(DWORD counter_3(0); counter_3 != 3;)
				{
					if(vecTriangles.at(relevantTris[counter_2]).fathers[counter_3] == vertsID)
					{
						vecTriangles.at(relevantTris[counter_2]).fathers[counter_3] = vecTriangles.at(relevantTris[counter_2]).verticesID[counter_3];
					}
					counter_3 += 1;
				}
				counter_2 += 1;
			}
			vecVertexSphere.at(vertsID).numLinkedVerts = 0;
		}
	}
	void uniteVertices()
	{
		bool found[2];
		DWORD vertsID[2];	

		this->findPickedVerts(found, vertsID, 2);
		if(found[0] == TRUE && found[1] == TRUE)
		{
			vecVertexSphere.at(vertsID[0]).linkVert(vertsID[1]);		//К вершине vertsID[0] привинчиваем вершину vertsID[1]
			vecVertexSphere.at(vertsID[1]).vertType = Child;			//Скрываем вершину vertsID[1]
			vecVertexSphere.at(vertsID[1]).isPicked = FALSE;

			particles[vertsID[1]].pos = vecVertexSphere.at(vertsID[1]).center = vertices[vertsID[1]].pos;

			for(DWORD counter(0); counter != vecVertexSphere.at(vertsID[1]).numLinkedVerts;)
			{
				vecVertexSphere.at(vertsID[0]).linkVert(vecVertexSphere.at(vertsID[1]).linkedVertices[counter]);
				vecVertexSphere.at(vertsID[1]).linkedVertices[counter] = 0;
				vecVertexSphere.at(vecVertexSphere.at(vertsID[1]).linkedVertices[counter]).isPicked = FALSE;

				particles[vecVertexSphere[vertsID[1]].linkedVertices[counter]].pos = 
				vecVertexSphere.at(vecVertexSphere.at(vertsID[1]).linkedVertices[counter]).center = 
				vertices[vecVertexSphere[vertsID[1]].linkedVertices[counter]].pos =
				vecVertexSphere.at(vertsID[1]).center;

				counter += 1;
			}
			DWORD relevantTris_1[24];
			DWORD numFoundTris_1 = this->findRelevantTriangles(&vertsID[0], relevantTris_1, 1);
			DWORD relevantTris_2[24];
			DWORD numFoundTris_2 = this->findRelevantTriangles(&vertsID[1], relevantTris_2, 1);
			for(DWORD counter(0); counter != numFoundTris_2;)
			{
				for(DWORD counter_2(0); counter_2 != 3;)
				{
					if(vecTriangles.at(relevantTris_2[counter]).fathers[counter_2] == vertsID[1])
					{
						/**********/
						for(DWORD counter_3(0); counter_3 != numFoundTris_1;)
						{
							for(DWORD counter_4(0); counter_4 != 3;)
							{
								if(vecTriangles.at(relevantTris_1[counter_3]).fathers[counter_4] == vertsID[0])
								{
									vecTriangles.at(relevantTris_2[counter]).fathers[counter_2] = 
									vecTriangles.at(relevantTris_1[counter_3]).fathers[counter_4];
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
			//Нужно все fathers треугольников, содержащих vertsID[1], прировнять к соответствующему father треугольника, содержащего vertsID[0]  
			
			


			vecVertexSphere.at(vertsID[1]).numLinkedVerts = 0;
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
				D3DXVECTOR3 v = clickRay.origin - vecVertexSphere.at(counter).center;

				float b = 2.0f * D3DXVec3Dot(&clickRay.direction, &v);
				float c = D3DXVec3Dot(&v, &v) - (vecVertexSphere.at(counter).radius * vecVertexSphere.at(counter).radius);

				float discriminant = (b * b) - (4.0f * c);

				if(discriminant > 0.0f)
				{
					discriminant = sqrtf(discriminant);

					float s0 = (-b + discriminant) / 2.0f;
					float s1 = (-b - discriminant) / 2.0f;

					if(s0 >= 0.0f || s1 >= 0.0f)
					{
						if(vecVertexSphere.at(counter).isPicked == FALSE)
						{
							numPickedVerts++;
							BSBar->writeNumPickedVerts(numPickedVerts);
							vecVertexSphere.at(counter).isPicked = TRUE;
						}
						else
						{
							numPickedVerts--;
							BSBar->writeNumPickedVerts(numPickedVerts);
							vecVertexSphere.at(counter).isPicked = FALSE;
						}
						
						if(numPickedVerts == 3)
						{
							bool found[3] = {FALSE, FALSE, FALSE};
							DWORD vertsID[3];

							this->findPickedVerts(found, vertsID, 3);
	
							if(found[0] == TRUE && found[1] == TRUE && found[2] == TRUE)   //Три вершины найдены
							{
								DWORD relevantTriangle[24];
								DWORD numRelevantTris = this->findRelevantTriangles(vertsID, relevantTriangle, 3);

								if(numPickedVerts == 3)
								{
									for(DWORD counter(0); counter != numRelevantTris;)
									{
										pickedSubset = vecTriangles.at(relevantTriangle[counter]).subsetID;
										counter += 1;
									}
								}
							}
						}
						if(vecVertexSphere.at(counter).vertType == Father)
							return vecVertexSphere.at(counter).vertexID+1;
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
			vecVertexSphere.at(counter).isPicked = FALSE;
			counter += 1;
		}
		numPickedVerts = 0;
	}
	D3DMATERIAL9* getMaterial()
	{
		return vecMaterials.at(pickedSubset).getMaterial();
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

	void findPickedVerts(bool found[], DWORD vertsID[], DWORD numNeeded)
	{
		int counter(0);
		for(short i(0); i != numNeeded;)
		{
			for(;counter != numCreatedVerts;)
			{
				if(vecVertexSphere.at(counter).isPicked)
				{
					vertsID[i] = vecVertexSphere.at(counter).startVertex;
					found[i] = TRUE;
					counter++;
					break;
				}
				counter++;
			}
			i++;
		}
	}

	DWORD findRelevantTriangles(DWORD vertsID[], DWORD relevantTriangle[], DWORD numNeeded)
	{
		DWORD relevantVerts(0);
		DWORD numRelevantTris(0);
		for(DWORD counter(0); counter != numCreatedFaces;)
		{
			for(short counter_2(0); counter_2 != 3;)
			{
				if(vecTriangles.at(counter).fathers[0] == vertsID[counter_2])
					relevantVerts += 1;
				if(vecTriangles.at(counter).fathers[1] == vertsID[counter_2])
					relevantVerts += 1;
				if(vecTriangles.at(counter).fathers[2] == vertsID[counter_2])
					relevantVerts += 1;

				if(relevantVerts == numNeeded)
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

	void loadObject(tempObject_class* tempObject)
	{
		vpb->Lock(NULL, NULL, (void**)&particles, NULL);
		mesh->LockVertexBuffer(0, (void**)&vertices);
		for(DWORD counter(0); counter != tempObject->numVerts;)
		{
			vertices[counter] = *tempObject->vertices[counter];

			vecVertexSphere.push_back(sphere_struct(vertices[counter].pos, counter, counter, 
				0.3f, FALSE, counter, tempObject->vertexSphere[counter]->vertType));
		
			for(DWORD counter_2(0); counter_2 != tempObject->vertexSphere[counter]->numLinkedVerts;)
			{
				vecVertexSphere.at(counter).linkVert(tempObject->vertexSphere[counter]->linkedVertices[counter_2]);
				counter_2 += 1;
			}

			particles[counter] = particle(vertices[counter].pos, 0.5f);

			numCreatedVerts += 1;
			counter += 1;
		}
		mesh->UnlockVertexBuffer();
		vpb->Unlock();

		mesh->LockIndexBuffer(0, (void**)&indices);
		for(DWORD counter(0); counter != tempObject->numIndices;)
		{
			indices[counter] = *tempObject->indices[counter];
			counter += 1;
		}
		mesh->UnlockIndexBuffer();


		mesh->LockAttributeBuffer(0, &attributeBuffer);
		for(DWORD counter(0); counter != tempObject->numAttributes;)
		{
			attributeBuffer[counter] = *tempObject->attributeBuffer[counter];
			counter += 1;
		}
		mesh->UnlockAttributeBuffer();

		for(DWORD counter(0); counter != tempObject->numFaces;)
		{		
			vecTriangles.push_back(triangle(Full, 
				tempObject->triangles[counter]->verticesID[0], 
				tempObject->triangles[counter]->verticesID[1], 
				tempObject->triangles[counter]->verticesID[2], 
				counter, tempObject->triangles[counter]->subsetID, 
				tempObject->triangles[counter]->subsetID, 
				tempObject->triangles[counter]->fathers[0], 
				tempObject->triangles[counter]->fathers[1], 
				tempObject->triangles[counter]->fathers[2]));
			
			numCreatedFaces += 1;
			counter += 1;
		}

		for(DWORD counter(0); counter != tempObject->numMaterials;)
		{

			vecMaterials.push_back(material_class());
			vecMaterials.at(counter).initMaterialBase(device, counter);
			vecMaterials.at(counter).getMaterial()->Ambient = tempObject->materials[counter]->Ambient;
			vecMaterials.at(counter).getMaterial()->Diffuse = tempObject->materials[counter]->Diffuse;
			vecMaterials.at(counter).getMaterial()->Specular = tempObject->materials[counter]->Specular;
			vecMaterials.at(counter).getMaterial()->Emissive = tempObject->materials[counter]->Emissive;
			vecMaterials.at(counter).getMaterial()->Power = tempObject->materials[counter]->Power;

			vecTextures.push_back(texture_class());
			vecTextures.at(counter).initBaseForTexture(device);

			rendState[counter] = new rendState_class;
			rendState[counter]->initRendStateClass(device, rendStateTypes);

			numMaterials += 1;
			numRendState += 1;
			numTextures += 1;
			numSubsets += 1;
			counter += 1;
		}
		pickedSubset = 0;
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
	std::vector<triangle>* getTriangles()
	{
		return &vecTriangles;
	}

	sphere_struct* getVertexSphere(DWORD vertexNumber)
	{
		return &vecVertexSphere.at(vertexNumber);
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
			if(vecVertexSphere.at(vert).isPicked == TRUE)
			{
				device->DrawPrimitive(D3DPT_POINTLIST, vecVertexSphere.at(vert).startVertex, 1);
			}
		}

		for(DWORD subset(0); subset != numSubsets; subset++)
		{
			if(numMaterials)
				vecMaterials.at(subset).resetMaterial();
			if(numTextures)
				vecTextures.at(subset).resetTexture();
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
		return &vecMaterials.at(materialNumber);
	}

	~object_class()
	{	
		//saveFullObject(this);

		for(;numRendState != 0;)
		{
			numRendState -= 1;
			delete rendState[numRendState];
			rendState[numRendState] = NULL;
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