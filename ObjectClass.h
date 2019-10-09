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
	UINT ObjectID;
	char buffer[256];
	LPCTSTR objectName;

	IDirect3DVertexBuffer9* vb;
	IDirect3DIndexBuffer9* ib;
	vertex* vertices;
	WORD* indices;
	material_class material;

	worldMatrix_class worldMatrices;

public:
	void initObjectBase(IDirect3DDevice9* bDevice, UINT numObject, IDirect3DVertexBuffer9* bVb, IDirect3DIndexBuffer9* bIb)
	{
		vb = bVb;
		ib = bIb;
		device = bDevice;
		ObjectID = numObject;
		objectName = L"Object";

		material.initMaterialBase(device);

		worldMatrices.fillMatrix(0, 0, 0, device);
		worldMatrices.worldMatrixRotateX(0.0f);		//Установка углов наклона в 0 (для правильного начального отображения)
		worldMatrices.worldMatrixRotateY(0.0f);
		worldMatrices.worldMatrixRotateZ(0.0f);
		this->setBaseVertices();
	}

	void setBaseVertices()
	{
		vb->Lock(NULL, NULL, (void**)&vertices, D3DLOCK_DISCARD);
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
		vb->Unlock();
	}

	void computeNormals()
	{
		D3DXVECTOR3 p0;
		D3DXVECTOR3 p1;
		D3DXVECTOR3 p2;
		D3DXVECTOR3 out;
		p0.x = vertices[indices[0]].x;
		p0.y = vertices[indices[0]].y;
		p0.z = vertices[indices[0]].z;
		p1.x = vertices[indices[1]].x;
		p1.y = vertices[indices[1]].y;
		p1.z = vertices[indices[1]].z;
		p2.x = vertices[indices[2]].x;
		p2.y = vertices[indices[2]].y;
		p2.z = vertices[indices[2]].z;

		D3DXVECTOR3 u = p1 - p0;
		D3DXVECTOR3 v = p2 - p0;

		D3DXVec3Cross(&out, &u, &v);
		D3DXVec3Normalize(&out, &out);

		vertices[0].nX = out.x;
		vertices[0].nY = out.y;
		vertices[0].nZ = out.z;
	}


	void moveObject(float x, float y, float z)
	{	worldMatrices.worldMatrixMove(x, y, z);}

	void rotateXObject(float Angle)
	{	worldMatrices.worldMatrixRotateX(Angle);}

	void rotateYObject(float Angle)
	{	worldMatrices.worldMatrixRotateY(Angle);}

	void rotateZObject(float Angle)
	{	worldMatrices.worldMatrixRotateZ(Angle);}

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
		material.resetMaterial();
		worldMatrices.resetWorldMatrices();
		device->SetStreamSource(0, vb, 0, sizeof(vertex));
		device->SetFVF(vertex::FVF);
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);
	}
};