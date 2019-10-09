
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
	float x, y, z;
	float        size;
	static const DWORD FVF;

	particle(){}
	particle(float bX, float bY, float bZ, float bSize)
	{
		x = bX;   y = bY;   z = bZ;
		size = bSize;
	}
};
const DWORD particle::FVF = D3DFVF_XYZ | D3DFVF_PSIZE;

class object_class
{
private:
	IDirect3DDevice9* device;
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
	triangle* triangles[512];
	IDirect3DVertexBuffer9* vpb;
	particle* particles;
	DWORD numPickedVerts;

	DWORD numSubsetsAttributes;
	DWORD numSubsets;
	DWORD numCreatedFaces;
	DWORD numCreatedVerts;

public:
	void initObjectBase(IDirect3DDevice9* bDevice, DWORD numObject)
	{
		device = bDevice;
		ObjectID = numObject;
		objectName = L"Object";	
		numSubsets      = 0;
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

		optimizedAdjacencyInfo = new DWORD [numFaces * 3];
		mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_COMPACT|D3DXMESHOPT_IGNOREVERTS,
			adjacencyInfo, optimizedAdjacencyInfo, 0, 0);


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
		
		material[numSubsets] = new material_class;
		material[numSubsets]->initMaterialBase(device);
		numSubsets += 1;
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
		attributeBuffer[numCreatedFaces] = 0; 
		mesh->UnlockAttributeBuffer();

		vpb->Lock(numCreatedVerts, 3, (void**)&particles, NULL);
		for(short i(0); i != 3;)
		{
			vertexSphere[numCreatedVerts+i] = new sphere_struct;
			vertexSphere[numCreatedVerts+i]->center.x	  = vertices[numCreatedVerts+i].pos.x;
			vertexSphere[numCreatedVerts+i]->center.y	  = vertices[numCreatedVerts+i].pos.y;
			vertexSphere[numCreatedVerts+i]->center.z	  = vertices[numCreatedVerts+i].pos.z;
			vertexSphere[numCreatedVerts+i]->vertexID	  = numCreatedVerts+i;
			vertexSphere[numCreatedVerts+i]->indexID      = numCreatedFaces*3+i;
			vertexSphere[numCreatedVerts+i]->radius		  = 0.5f;
			vertexSphere[numCreatedVerts+i]->isPicked	  = FALSE;
			vertexSphere[numCreatedVerts+i]->startVertex = numCreatedVerts+i;
 
			particles[numCreatedVerts+i] = particle(vertices[numCreatedVerts+i].pos.x,
													vertices[numCreatedVerts+i].pos.y,
													vertices[numCreatedVerts+i].pos.z,
													0.5f);
			i++;
		}
		vpb->Unlock();

		triangles[numCreatedFaces] = new triangle;
		triangles[numCreatedFaces]->triType = Full;
		triangles[numCreatedFaces]->verticesID[0] = numCreatedVerts;
		triangles[numCreatedFaces]->verticesID[1] = numCreatedVerts+1;
		triangles[numCreatedFaces]->verticesID[2] = numCreatedVerts+2;

		numCreatedVerts += 3;
		numCreatedFaces += 1;
	}

	void createNewTriangle()
	{
		int counter = 0;
		bool firstFound(FALSE), secondFound(FALSE);
		DWORD firstVertID, secondVertID;

		for(;counter != numCreatedVerts;)
		{
			if(vertexSphere[counter]->isPicked)
			{
				firstVertID = vertexSphere[counter]->vertexID;
				firstFound = TRUE;
				counter++;
				break;
			}
			counter++;
		}

		for(;counter != numCreatedVerts;)
		{
			if(vertexSphere[counter]->isPicked)
			{
				secondVertID = vertexSphere[counter]->vertexID;
				secondFound = TRUE;
				break;
			}
			counter++;
		}

		if(firstFound == TRUE && secondFound == TRUE)
		{
			mesh->LockVertexBuffer(0, (void**)&vertices);
			vertices[numCreatedVerts] = vertex(-3.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f); //New Vertex
			mesh->UnlockVertexBuffer();

			mesh->LockIndexBuffer(0, (void**)&indices);
			indices[numCreatedFaces*3  ] = numCreatedVerts;
			indices[numCreatedFaces*3+1] = firstVertID;
			indices[numCreatedFaces*3+2] = secondVertID;
			mesh->UnlockIndexBuffer();

			mesh->LockAttributeBuffer(0, &attributeBuffer); 
			attributeBuffer[numCreatedFaces] = 0; 
			mesh->UnlockAttributeBuffer();

			vertexSphere[numCreatedVerts] = new sphere_struct;
			vertexSphere[numCreatedVerts]->center.x = vertices[numCreatedVerts].pos.x;
			vertexSphere[numCreatedVerts]->center.y = vertices[numCreatedVerts].pos.y;
			vertexSphere[numCreatedVerts]->center.z = vertices[numCreatedVerts].pos.z;
			vertexSphere[numCreatedVerts]->vertexID = numCreatedVerts;
			vertexSphere[numCreatedVerts]->indexID  = numCreatedFaces*3;
			vertexSphere[numCreatedVerts]->radius   = 0.5f;
			vertexSphere[numCreatedVerts]->isPicked = FALSE;
			vertexSphere[numCreatedVerts]->startVertex = numCreatedVerts;

			triangles[numCreatedFaces] = new triangle;
			triangles[numCreatedFaces]->triType = Vert;
			triangles[numCreatedFaces]->verticesID[0] = numCreatedVerts;
			triangles[numCreatedFaces]->verticesID[1] = firstVertID;
			triangles[numCreatedFaces]->verticesID[2] = secondVertID;

			particles[numCreatedVerts] = particle(vertices[numCreatedVerts].pos.x,
				vertices[numCreatedVerts].pos.y,
				vertices[numCreatedVerts].pos.z,
				0.5f);

			numCreatedVerts += 1;
			numCreatedFaces += 1;
		}

		mesh->GenerateAdjacency(0.0001f, adjacencyInfo);

		D3DXComputeNormals(mesh, adjacencyInfo);
	}

	void swapIndices()
	{
		int counter = 0;
		bool firstFound(FALSE), secondFound(FALSE), thirdFound(FALSE);
		DWORD firstVertID, secondVertID, thirdVertID;

		for(;counter != numCreatedVerts;)
		{
			if(vertexSphere[counter]->isPicked)
			{
				firstVertID = vertexSphere[counter]->startVertex;
				firstFound = TRUE;
				counter++;
				break;
			}
			counter++;
		}

		for(;counter != numCreatedVerts;)
		{
			if(vertexSphere[counter]->isPicked)
			{
				secondVertID = vertexSphere[counter]->startVertex;
				secondFound = TRUE;
				counter++;
				break;
			}
			counter++;
		}

		for(;counter != numCreatedVerts;)
		{
			if(vertexSphere[counter]->isPicked)
			{
				thirdVertID = vertexSphere[counter]->startVertex;
				thirdFound = TRUE;
				break;
			}
			counter++;
		}

		if(firstFound == TRUE && secondFound == TRUE && thirdFound == TRUE)
		{
			DWORD indexBuffer;

			mesh->LockIndexBuffer(0, (void**)&indices);
			indexBuffer = indices[thirdVertID+2];
			indices[thirdVertID+2] = indices[thirdVertID+1];
			indices[thirdVertID+1] = indexBuffer;
			mesh->UnlockIndexBuffer();
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

		particles[vertexNumber].x = vertexSphere[vertexNumber]->center.x = vertices[vertexNumber].pos.x;
		particles[vertexNumber].y = vertexSphere[vertexNumber]->center.y = vertices[vertexNumber].pos.y;
		particles[vertexNumber].z = vertexSphere[vertexNumber]->center.z = vertices[vertexNumber].pos.z;
		mesh->UnlockVertexBuffer();	
	}

	void rotateXObject(float Angle)
	{	worldMatrices.worldMatrixRotateX(Angle);}

	void rotateYObject(float Angle)
	{	worldMatrices.worldMatrixRotateY(Angle);}

	void rotateZObject(float Angle)
	{	worldMatrices.worldMatrixRotateZ(Angle);}

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
		return material[0]->getMaterial(); //АСТАРОЖНА-НИАДЕКВАТА!!!11
	}

	void renameObject(HWND objectsList, HWND nameEditor, UINT objectNumber)
	{
		SendMessage(nameEditor, EM_LIMITTEXT, (WPARAM)80, NULL);
		SendMessage(nameEditor, EM_GETLINE, 0, (LPARAM)buffer);
		
		objectName = (LPCTSTR)buffer;
		
		SendMessage(objectsList, LB_DELETESTRING, (WPARAM)objectNumber, NULL);
		SendMessage(objectsList, LB_INSERTSTRING, (WPARAM)objectNumber, (LPARAM)objectName);
	}

	LPCTSTR getObjectName()
	{	return objectName;}

	void redraw()
	{
		worldMatrices.resetMatrices();

	
		device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
		device->SetRenderState(D3DRS_POINTSIZE, FtoDw(2.5f));
		device->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.2f));
		device->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDw(5.0f));

		for(DWORD vert = 0; vert != numCreatedVerts; vert++)
		{
			device->SetFVF(particle::FVF);
			device->SetStreamSource(0, vpb, 0, sizeof(particle));

			if(vertexSphere[vert]->isPicked == TRUE)
			{
				device->DrawPrimitive(D3DPT_POINTLIST, vertexSphere[vert]->startVertex, 1);
			}
		}


		for(DWORD subset = 0; subset != numSubsets; subset++)
		{
			material[subset]->resetMaterial();
			worldMatrices.resetMatrices();
			mesh->DrawSubset(subset);
		}
	}

	~object_class()
	{
		for(; numSubsets != NULL;)
		{
			numSubsets--;
			delete material[numSubsets];
			material[numSubsets] = NULL;
		}

		for(;numCreatedVerts != 0;)
		{
			numCreatedVerts--;
			delete vertexSphere[numCreatedVerts];
		}

		for(;numCreatedFaces != 0;)
		{
			numCreatedFaces--;
			delete triangles[numCreatedFaces];
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