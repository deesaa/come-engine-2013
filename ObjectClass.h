struct vertex	
{
	float x, y, z;
	static const DWORD FVF;

	vertex(){}
	vertex(float bX, float bY, float bZ)
	{
		x = bX; y = bY; z = bZ;
	}
};
const DWORD vertex::FVF = D3DFVF_XYZ;

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

	worldMatrix_class worldMatrices;

public:
	void initObjectBase(IDirect3DDevice9* bDevice, UINT numObject, IDirect3DVertexBuffer9* bVb, IDirect3DIndexBuffer9* bIb)
	{
		vb = bVb;
		ib = bIb;
		device = bDevice;
		ObjectID = numObject;
		objectName = L"Object";

		worldMatrices.fillMatrix(0, 0, 0, device);
		worldMatrices.worldMatrixRotateX(0.0f);		//Установка углов наклона в 0 (для правильного начального отображения)
		worldMatrices.worldMatrixRotateY(0.0f);
		worldMatrices.worldMatrixRotateZ(0.0f);
	
		vb->Lock(NULL, NULL, (void**)&vertices, D3DLOCK_DISCARD);
		vertices[0] = vertex(-1.0f, -1.0f, -1.0f);
		vertices[1] = vertex(-1.0f,  1.0f, -1.0f);
		vertices[2] = vertex( 1.0f,  1.0f, -1.0f);
		vertices[3] = vertex( 1.0f, -1.0f, -1.0f);
		vertices[4] = vertex(-1.0f, -1.0f,  1.0f);
		vertices[5] = vertex(-1.0f,  1.0f,  1.0f);
		vertices[6] = vertex( 1.0f,  1.0f,  1.0f);
		vertices[7] = vertex( 1.0f, -1.0f,  1.0f);
		vb->Unlock();

		ib->Lock(0, 0, (void**)&indices, D3DLOCK_DISCARD);
		// передняя грань
		indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;
		indices[3]  = 0; indices[4]  = 2; indices[5]  = 3;
		// задняя грань
		indices[6]  = 4; indices[7]  = 6; indices[8]  = 5;
		indices[9]  = 4; indices[10] = 7; indices[11] = 6;
		// левая грань
		indices[12] = 4; indices[13] = 5; indices[14] = 1;
		indices[15] = 4; indices[16] = 1; indices[17] = 0;
		// правая грань
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
		worldMatrices.resetWorldMatrices();
		device->SetStreamSource(0, vb, 0, sizeof(vertex));
		device->SetIndices(ib);
		device->SetFVF(vertex::FVF);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0, 0, 8, 0, 12);
	}
};