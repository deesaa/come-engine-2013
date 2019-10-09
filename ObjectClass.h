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
	DWORD numVertices;
	DWORD numFaces;

	material_class* material[64];
	worldMatrix_class worldMatrices;

public:
	void initObjectBase(IDirect3DDevice9* bDevice, DWORD numObject)
	{
		device = bDevice;
		ObjectID = numObject;
		objectName = L"Object";	
		numSubsets = 0;

		D3DXCreateMeshFVF(32, 64, D3DXMESH_MANAGED, vertex::FVF, device, &mesh);
		this->baseObject();
		numVertices = mesh->GetNumVertices();
		this->spheresOfVertices();

		numFaces = mesh->GetNumFaces();
		adjacencyInfo = new DWORD [numFaces * 3];
		mesh->GenerateAdjacency(0.0001f, adjacencyInfo);

		optimizedAdjacencyInfo = new DWORD [numFaces * 3];
		mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_COMPACT|D3DXMESHOPT_VERTEXCACHE,
			adjacencyInfo, optimizedAdjacencyInfo, 0, 0);


		mesh->GetAttributeTable(0, &numSubsetsAttributes);
		attributeTable = new D3DXATTRIBUTERANGE [numSubsetsAttributes];
		mesh->GetAttributeTable(attributeTable, &numSubsetsAttributes);

		material[numSubsets] = new material_class;
		material[numSubsets]->initMaterialBase(device);
		numSubsets++;

		worldMatrices.fillMatrix(0, 0, 0, device);
		worldMatrices.worldMatrixRotateX(0.0f);		//Установка углов наклона в 0 (для правильного начального отображения)
		worldMatrices.worldMatrixRotateY(0.0f);
		worldMatrices.worldMatrixRotateZ(0.0f);
	}

	void baseObject()
	{
		mesh->LockVertexBuffer(0, (void**)&vertices);
		vertices[0] = vertex(-1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);
		vertices[1] = vertex( 0.0f, 1.0f,  0.0f, 0.0f, 0.707f, -0.707f);
		vertices[2] = vertex( 1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);

		// левая грань
		vertices[3] = vertex(-1.0f, 0.0f,  1.0f, -0.707f, 0.707f, 0.0f);
		vertices[4] = vertex( 0.0f, 1.0f,  0.0f, -0.707f, 0.707f, 0.0f);
		vertices[5] = vertex(-1.0f, 0.0f, -1.0f, -0.707f, 0.707f, 0.0f);

		// правая грань
		vertices[6] = vertex( 1.0f, 0.0f, -1.0f, 0.707f, 0.707f, 0.0f);
		vertices[7] = vertex( 0.0f, 1.0f,  0.0f, 0.707f, 0.707f, 0.0f);
		vertices[8] = vertex( 1.0f, 0.0f,  1.0f, 0.707f, 0.707f, 0.0f);

		// задняя грань
		vertices[9]  = vertex( 1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);
		vertices[10] = vertex( 0.0f, 1.0f, 0.0f, 0.0f, 0.707f, 0.707f);
		vertices[11] = vertex(-1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);
		mesh->UnlockVertexBuffer();

		mesh->LockIndexBuffer(0, (void**)&indices);
		indices[0] = 0; indices[1] = 1; indices[2] = 2;
		indices[3] = 3; indices[4] = 4; indices[5] = 5;
		indices[6] = 6; indices[7] = 7; indices[8] = 8;
		indices[9] = 9; indices[10] = 10; indices[11] = 11;
		mesh->UnlockIndexBuffer();

		mesh->LockAttributeBuffer(0, &attributeBuffer);
		for(int a = 0; a < 4; a++)   
			attributeBuffer[a] = 0; 
		mesh->UnlockAttributeBuffer();
	}

	void spheresOfVertices()
	{
		for (DWORD v = 0; v != numVertices;)
		{
			vertexSphere[v] = new sphere_struct;
			vertexSphere[v]->center.x = vertices[v].x;
			vertexSphere[v]->center.y = vertices[v].y;
			vertexSphere[v]->center.z = vertices[v].z;
			vertexSphere[v]->radius	  = 0.5f; 
			v++;
		}
	}

	void moveObject(float x, float y, float z)
	{	worldMatrices.worldMatrixMove(x, y, z);}

	void rotateXObject(float Angle)
	{	worldMatrices.worldMatrixRotateX(Angle);}

	void rotateYObject(float Angle)
	{	worldMatrices.worldMatrixRotateY(Angle);}

	void rotateZObject(float Angle)
	{	worldMatrices.worldMatrixRotateZ(Angle);}

	DWORD checkIntersection(ray_struct clickRay)
	{
		BOOL  hit;	DWORD pFaceIndex; FLOAT pU; FLOAT pV; FLOAT pDist;

		D3DXMATRIX worldMatrix = worldMatrices.getWorldMatrix();
		
		D3DXMatrixInverse(&worldMatrix, NULL, &worldMatrix);
		D3DXVec3TransformCoord(&clickRay.origin, &clickRay.origin, &worldMatrix);
		D3DXVec3TransformNormal(&clickRay.direction, &clickRay.direction, &worldMatrix);
		D3DXVec3Normalize(&clickRay.direction, &clickRay.direction);

		D3DXIntersect(mesh, &clickRay.origin, &clickRay.direction,
			&hit, &pFaceIndex, &pU, &pV, &pDist, NULL, NULL);
		if(hit == TRUE)
			return ObjectID + 1;
		else
			return 0;
	}

	D3DMATERIAL9* getMaterial()
	{
		return material[0]->getMaterial();
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
		worldMatrices.resetWorldMatrices();
		for(DWORD subset = 0; subset != numSubsets; subset++)
		{
			material[subset]->resetMaterial();
			mesh->DrawSubset(0);
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

		for (; numVertices != NULL;)
		{
			numVertices--;
			delete vertexSphere[numVertices];
			vertexSphere[numVertices] = NULL;
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