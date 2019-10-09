
enum interType{interObject, interVertex};

struct vertex	
{
	float x, y, z;
	float nX, nY, nZ;
	static const DWORD FVF;

	vertex(){}
	vertex(float bX, float bY, float bZ, float bnX, float bnY, float bnZ)
	{
		x = bX;   y = bY;   z = bZ;
		nX = bnX; nY = bnY; nZ = bnZ;
	}
};
const DWORD vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

class object_class
{
private:
	IDirect3DDevice9* device;
	DWORD ObjectID;
	char buffer[256];
	LPCTSTR objectName;
	sphere_struct* vertexSphere[64];
	ray_struct rayOfPicking;

	ID3DXMesh* mesh;

	vertex* vertices;
	WORD* indices;
	DWORD* attributeBuffer;

	DWORD* adjacencyInfo;
	DWORD* optimizedAdjacencyInfo;
	D3DXATTRIBUTERANGE* attributeTable; 

	DWORD numSubsetsAttributes;
	DWORD numSubsets;
	DWORD numMaterials;

	DWORD numVertices;
	DWORD numFaces;
	DWORD numCreatedVerts;
	DWORD numCreatedFaces;

	material_class* material[64];
	worldMatrices_class worldMatrices;

public:
	void initObjectBase(IDirect3DDevice9* bDevice, DWORD numObject)
	{
		device = bDevice;
		ObjectID = numObject;
		objectName = L"Object";	
		numSubsets = 0;
		numCreatedVerts = 0;
		numCreatedFaces = 0;

		D3DXCreateMeshFVF(256, 512, D3DXMESH_MANAGED, vertex::FVF, device, &mesh);
	
		this->baseObject();

		numVertices = mesh->GetNumVertices();
		numFaces = mesh->GetNumFaces();

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

	void createFoundation()
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

		vertexSphere[numCreatedVerts] = new sphere_struct;
		vertexSphere[numCreatedVerts]->center.x = vertices[numCreatedVerts].x;
		vertexSphere[numCreatedVerts]->center.y = vertices[numCreatedVerts].y;
		vertexSphere[numCreatedVerts]->center.z = vertices[numCreatedVerts].z;
		vertexSphere[numCreatedVerts]->radius   = 0.6f;
		vertexSphere[numCreatedVerts]->vertexID = numCreatedVerts;
		vertexSphere[numCreatedVerts+1] = new sphere_struct;
		vertexSphere[numCreatedVerts+1]->center.x = vertices[numCreatedVerts+1].x;
		vertexSphere[numCreatedVerts+1]->center.y = vertices[numCreatedVerts+1].y;
		vertexSphere[numCreatedVerts+1]->center.z = vertices[numCreatedVerts+1].z;
		vertexSphere[numCreatedVerts+1]->radius   = 0.6f;
		vertexSphere[numCreatedVerts+1]->vertexID = numCreatedVerts+1;
		vertexSphere[numCreatedVerts+2] = new sphere_struct;
		vertexSphere[numCreatedVerts+2]->center.x = vertices[numCreatedVerts+2].x;
		vertexSphere[numCreatedVerts+2]->center.y = vertices[numCreatedVerts+2].y;
		vertexSphere[numCreatedVerts+2]->center.z = vertices[numCreatedVerts+2].z;
		vertexSphere[numCreatedVerts+2]->radius   = 0.6f;
		vertexSphere[numCreatedVerts+2]->vertexID = numCreatedVerts+2;

		numCreatedFaces++;
		numCreatedVerts += 3;
	}

	void createVertex()
	{
		mesh->LockVertexBuffer(0, (void**)&vertices);
		vertices[numCreatedVerts] = vertex(-1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f); //New 
		mesh->UnlockVertexBuffer();

		mesh->LockIndexBuffer(0, (void**)&indices);
		indices[numCreatedVerts  ] = numCreatedVerts; 
		indices[numCreatedVerts+1] = numCreatedVerts-3; 
		indices[numCreatedVerts+2] = numCreatedVerts-1;
		mesh->UnlockIndexBuffer();

		mesh->LockAttributeBuffer(0, &attributeBuffer); 
		attributeBuffer[numCreatedFaces] = 0; 
		mesh->UnlockAttributeBuffer();

		vertexSphere[numCreatedVerts] = new sphere_struct;
		vertexSphere[numCreatedVerts]->center.x = vertices[numCreatedVerts].x;
		vertexSphere[numCreatedVerts]->center.y = vertices[numCreatedVerts].y;
		vertexSphere[numCreatedVerts]->center.z = vertices[numCreatedVerts].z;
		vertexSphere[numCreatedVerts]->radius   = 0.6f;
		vertexSphere[numCreatedVerts]->vertexID = numCreatedVerts;

		numCreatedVerts++;
		numCreatedFaces++;	
	}

	void baseObject()
	{
		this->createFoundation();
		this->createVertex();

		material[numSubsets] = new material_class;
		material[numSubsets]->initMaterialBase(device);
		numSubsets++;
	}

	void moveObject(float x, float y, float z)
	{	worldMatrices.worldMatrixMove(x, y, z);}

	void moveVertex(DWORD vertexNumber, float x, float y, float z)
	{
		vertexNumber--;
		
		mesh->LockVertexBuffer(0, (void**)&vertices);
		vertices[vertexNumber].x += x;
		vertices[vertexNumber].y -= y;
		vertices[vertexNumber].z += z;
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
						return vertexSphere[counter]->vertexID+1;
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
			delete vertexSphere[numCreatedVerts];
			numCreatedVerts--;
		}

		delete adjacencyInfo;
		adjacencyInfo			 = NULL;
		delete optimizedAdjacencyInfo;
		optimizedAdjacencyInfo   = NULL;
		delete attributeTable;
		attributeTable			 = NULL;

		mesh->Release();
	}
};