
enum interType{interObject, interVertex};

struct vertex	
{
	D3DXVECTOR3 pos;
	float nX, nY, nZ;
	static const DWORD FVF;

	vertex(){}
	vertex(float bX, float bY, float bZ, float bnX, float bnY, float bnZ)
	{
		pos = D3DXVECTOR3(bX, bY, bZ);
		nX = bnX; nY = bnY; nZ = bnZ;
	}
};
const DWORD vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

struct particle
{
	D3DXVECTOR3 pos;
	float size;
	static const DWORD FVF;

	particle(){}
	particle(D3DXVECTOR3 bPos, float bSize)
	{
		pos = bPos;
		size = bSize;
	}
};
const DWORD particle::FVF = D3DFVF_XYZ | D3DFVF_PSIZE;

class object_class
{
private:
	IDirect3DDevice9* device;
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
	sphere_struct* vertexSphere[512];
	std::vector<triangle> triangless;
	triangle* triangles[512];
	IDirect3DVertexBuffer9* vpb;
	particle* particles;
	DWORD numPickedVerts;

	DWORD numSubsetsAttributes;
	DWORD numSubsets;
	DWORD numCreatedFaces;
	DWORD numCreatedVerts;

	DWORD numMaterials;

public:
	void initObjectBase(IDirect3DDevice9* bDevice, DWORD numObject, HWND bSubsetsList)
	{
		device = bDevice;
		ObjectID = numObject;
		subsetsList = bSubsetsList;
		objectName = L"Object";	
		numSubsets      = 0;
		pickedSubset    = -1;
		numMaterials	= 0;
		numCreatedVerts = 0;
		numCreatedFaces = 0;
		numPickedVerts  = 0;

		D3DXCreateMeshFVF(256, 512, D3DXMESH_MANAGED, vertex::FVF, device, &mesh);

		device->CreateVertexBuffer(
			512,
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
		worldMatrices.worldMatrixRotateX(0.0f);		//”становка углов наклона в 0 (дл€ правильного начального отображени€)
		worldMatrices.worldMatrixRotateY(0.0f);
		worldMatrices.worldMatrixRotateZ(0.0f);
	}

	void baseObject()
	{
		this->createNewFoundation();
		
		material[numMaterials] = new material_class;
		material[numMaterials]->initMaterialBase(device);

		SendMessage(subsetsList, LB_INSERTSTRING, numSubsets, (LPARAM)(LPCTSTR)L"Subset");
		pickedSubset = numSubsets;

		numSubsets	 += 1;
		numMaterials += 1;
	}

	void createNewFoundation()
	{
		mesh->LockVertexBuffer(0, (void**)&vertices);
		vertices[numCreatedVerts  ] = vertex( 0.0f, 1.0f,  0.0f, 0.0f, 0.0f, 0.0f);
		vertices[numCreatedVerts+1] = vertex( 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);	
		vertices[numCreatedVerts+2] = vertex(-1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
		mesh->UnlockVertexBuffer();

		mesh->LockIndexBuffer(0, (void**)&indices);
		indices[numCreatedVerts  ] = numCreatedVerts;
		indices[numCreatedVerts+1] = numCreatedVerts+1; 
		indices[numCreatedVerts+2] = numCreatedVerts+2;
		mesh->UnlockIndexBuffer();

		mesh->LockAttributeBuffer(0, &attributeBuffer); 
		attributeBuffer[numCreatedFaces] = numSubsets; 
		mesh->UnlockAttributeBuffer();

		vpb->Lock(numCreatedVerts, 3, (void**)&particles, NULL);
		for(short i(0); i != 3;)
		{
			vertexSphere[numCreatedVerts+i] = new sphere_struct;
			*vertexSphere[numCreatedVerts+i] = sphere_struct(vertices[numCreatedVerts+i].pos, numCreatedVerts+i, numCreatedFaces*3+i, 
				0.3f, FALSE, numCreatedVerts+i);

			particles[numCreatedVerts+i] = particle(vertices[numCreatedVerts+i].pos, 0.5f);
			i++;
		}
		vpb->Unlock();

		triangles[numCreatedFaces] = new triangle;
		*triangles[numCreatedFaces] = triangle(Full, numCreatedVerts, numCreatedVerts+1, numCreatedVerts+2, 
			numCreatedFaces, numSubsets, numMaterials);

		numCreatedVerts += 3;
		numCreatedFaces += 1;
	}

	void createNewTriangle()
	{
		bool found[3] = {FALSE, FALSE, FALSE};
		DWORD vertsID[3];
		
		 this->findRelevantVertices(found, vertsID);

		if(found[0] == TRUE && found[1] == TRUE && found[2] == TRUE)
		{
			mesh->LockIndexBuffer(0, (void**)&indices);
			indices[numCreatedFaces*3  ] = vertsID[0];
			indices[numCreatedFaces*3+1] = vertsID[1];
			indices[numCreatedFaces*3+2] = vertsID[2];
			mesh->UnlockIndexBuffer();

			mesh->GenerateAdjacency(0.0001f, adjacencyInfo);

			mesh->LockAttributeBuffer(0, &attributeBuffer); 
			attributeBuffer[numCreatedFaces] = numSubsets-1; 
			mesh->UnlockAttributeBuffer();

			triangles[numCreatedFaces] = new triangle;
			*triangles[numCreatedFaces] = triangle(Ind, vertsID[0], vertsID[1], vertsID[2], 
				numCreatedFaces, numSubsets-1, numMaterials-1);

			numCreatedFaces += 1;
		}
		else
		{
			if(found[0] == TRUE && found[1] == TRUE)
			{
				mesh->LockVertexBuffer(0, (void**)&vertices);
				vertices[numCreatedVerts] = vertex(-3.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f); //New Vertex
				mesh->UnlockVertexBuffer();

				mesh->LockIndexBuffer(0, (void**)&indices);
				indices[numCreatedFaces*3  ] = numCreatedVerts;
				indices[numCreatedFaces*3+1] = vertsID[0];
				indices[numCreatedFaces*3+2] = vertsID[1];
				mesh->UnlockIndexBuffer();

				mesh->GenerateAdjacency(0.0001f, adjacencyInfo);

				mesh->LockAttributeBuffer(0, &attributeBuffer); 
				attributeBuffer[numCreatedFaces] = numSubsets-1; 
				mesh->UnlockAttributeBuffer();

				vertexSphere[numCreatedVerts] = new sphere_struct;
				*vertexSphere[numCreatedVerts] = sphere_struct(vertices[numCreatedVerts].pos, numCreatedVerts, numCreatedFaces*3, 
					0.3f, FALSE, numCreatedVerts);

				triangles[numCreatedFaces] = new triangle;
				*triangles[numCreatedFaces] = triangle(Vert, numCreatedVerts, vertsID[0], vertsID[1], 
					numCreatedFaces, numSubsets-1, numMaterials-1);

				particles[numCreatedVerts] = particle(vertices[numCreatedVerts].pos, 0.5f);

				numCreatedVerts += 1;
				numCreatedFaces += 1;
			}
		}
		D3DXComputeNormals(mesh, adjacencyInfo);
	}

	void swapIndices()
	{
		int counter(0);
		bool found[3] = {FALSE, FALSE, FALSE};
		DWORD vertsID[3];

		this->findRelevantVertices(found, vertsID);

		if(found[0] == TRUE && found[1] == TRUE && found[2] == TRUE)   //“ри вершины найдены
		{
			DWORD relevantTriangle(-1);

			this->findRelevantTriangles(vertsID, &relevantTriangle);

			if(relevantTriangle != -1 && numPickedVerts == 3)
			{
				DWORD indexBuffer;										

				mesh->LockIndexBuffer(0, (void**)&indices);
				indexBuffer = indices[relevantTriangle*3+2];					//ћен€ем местами второй и третий индекс
				indices[relevantTriangle*3+2] = indices[relevantTriangle*3+1];
				indices[relevantTriangle*3+1] = indexBuffer;
				mesh->UnlockIndexBuffer();
			}
		}
	}

	void newSubset()
	{
		int counter(0);
		bool found[3] = {FALSE, FALSE, FALSE};
		DWORD vertsID[3];

		this->findRelevantVertices(found, vertsID);

		if(found[0] == TRUE && found[1] == TRUE && found[2] == TRUE)   //“ри вершины найдены
		{
			DWORD relevantTriangle(-1);
			this->findRelevantTriangles(vertsID, &relevantTriangle);

			if(relevantTriangle != -1 && numPickedVerts == 3)
			{
				mesh->LockAttributeBuffer(0, &attributeBuffer); 
				attributeBuffer[relevantTriangle] = numSubsets; 
				mesh->UnlockAttributeBuffer();

				material[numMaterials] = new material_class;
				material[numMaterials]->initAbsolutelyWhiteMaterial(device);

				triangles[relevantTriangle]->subsetID   = numSubsets;
				triangles[relevantTriangle]->materialID = numMaterials;

				SendMessage(subsetsList, LB_INSERTSTRING, numSubsets, (LPARAM)(LPCTSTR)L"Subset");
			
				pickedSubset = numSubsets;
				numSubsets += 1;
				numMaterials++;
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
		mesh->UnlockVertexBuffer();	
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
							vertexSphere[counter]->isPicked = TRUE;
						}
						else
						{
							numPickedVerts--;
							vertexSphere[counter]->isPicked = FALSE;
						}
						
						if(numPickedVerts == 3)
						{
							int counter(0);
							bool found[3] = {FALSE, FALSE, FALSE};
							DWORD vertsID[3];

							this->findRelevantVertices(found, vertsID);
	
							if(found[0] == TRUE && found[1] == TRUE && found[2] == TRUE)   //“ри вершины найдены
							{
								DWORD relevantTriangle(-1);

								this->findRelevantTriangles(vertsID, &relevantTriangle);

								if(relevantTriangle != -1 && numPickedVerts == 3)
								{
									pickedSubset = triangles[relevantTriangle]->subsetID;
								}
							}
						}
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

	D3DMATERIAL9* getMaterial()
	{
		return material[pickedSubset]->getMaterial();
	}

	void renameObject(HWND objectsList, HWND nameEditor, UINT objectNumber)
	{
		SendMessage(nameEditor, EM_LIMITTEXT, (WPARAM)80, NULL);
		SendMessage(nameEditor, EM_GETLINE, 0, (LPARAM)buffer);
		
		objectName = (LPCTSTR)buffer;
		
		SendMessage(objectsList, LB_DELETESTRING, (WPARAM)objectNumber, NULL);
		SendMessage(objectsList, LB_INSERTSTRING, (WPARAM)objectNumber, (LPARAM)objectName);
	}

	void findRelevantVertices(bool found[], DWORD vertsID[])
	{
		int counter(0);
		for(short i(0); i != 3;)
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

	void findRelevantTriangles(DWORD vertsID[], DWORD* relevantTriangle)
	{
		short relevantVertices(0);
		for(DWORD tri(0); tri != numCreatedFaces;)	//ѕровер€ем отношение выбранных вершин к какому-то одному полигону					
		{
			for(short i(0); i != 3;)								
			{
				if(triangles[tri]->verticesID[i] == vertsID[0])
					relevantVertices++;
				if(triangles[tri]->verticesID[i] == vertsID[1])
					relevantVertices++;
				if(triangles[tri]->verticesID[i] == vertsID[2])
					relevantVertices++;

				if(relevantVertices == 3)
				{	*relevantTriangle = tri;
				break;}
				i++;
			}
			relevantVertices = 0;
			tri++;
		}
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
	void saveObjectAs(HINSTANCE bhInstace, HWND bWindow)
	{
		saveAs(bhInstace, bWindow, this);
	}
	void saveObject()
	{
		saveFullObject(this);
	}
	void redraw()
	{
		worldMatrices.resetMatrices();
	
		device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
		device->SetRenderState(D3DRS_POINTSIZE, flToDw(2.5f));
		device->SetRenderState(D3DRS_POINTSIZE_MIN, flToDw(0.2f));
		device->SetRenderState(D3DRS_POINTSIZE_MAX, flToDw(5.0f));

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
			material[subset]->resetMaterial();
			mesh->DrawSubset(subset);
		}
	}

	~object_class()
	{	
		saveFullObject(this);

		for(;numMaterials != 0;)
		{
			numMaterials--;
			delete material[numMaterials];
			material[numMaterials] = NULL;
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