struct COVertex
{
	float x, y, z;
	static const DWORD FVF;

	COVertex(){}
	COVertex(float bX, float bY, float bZ)
	{
		x = bX; y = bY; z = bZ;
	}
};
const DWORD COVertex::FVF = D3DFVF_XYZ;
void COFillBuffers(IDirect3DVertexBuffer9* vb, IDirect3DIndexBuffer9* ib, COVertex* vertices, WORD* indices);

class object_creator
{
private:
	IDirect3DDevice9* device;
	fullMatrices_class matrices;

	IDirect3DVertexBuffer9* vb;
	IDirect3DIndexBuffer9* ib;
	COVertex* vertices;
	WORD* indices;

public:
	object_creator(IDirect3DDevice9* bDevice)
	{
		device = bDevice;
		this->createBuffers();
		COFillBuffers(vb, ib, vertices, indices);
		matrices.fillMatrices(0, 0, 0, 0.5f, device);
	}

	void createBuffers()
	{
		device->CreateVertexBuffer(
			4 * sizeof(COVertex),
			D3DUSAGE_WRITEONLY,
			COVertex::FVF,
			D3DPOOL_MANAGED,
			&vb,
			0);

		device->CreateIndexBuffer(
			6 * sizeof(WORD),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&ib,
			0);
	}

	void redraw()
	{
		device->SetStreamSource(0, vb, 0, sizeof(COVertex));
		device->SetIndices(ib);
		device->SetFVF(COVertex::FVF);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0, 0, 4, 0, 2);
	}

	~object_creator()
	{
		ib->Release();
		vb->Release();
	}
};

void COFillBuffers(IDirect3DVertexBuffer9* vb, IDirect3DIndexBuffer9* ib, COVertex* vertices, WORD* indices)
{
	vb->Lock(NULL, NULL, (void**)&vertices, NULL);
	vertices[0] = COVertex(-1.0f, -1.0f, -1.0f);
	vertices[1] = COVertex(-1.0f,  1.0f, -1.0f);
	vertices[2] = COVertex( 1.0f,  1.0f, -1.0f);
	vertices[3] = COVertex( 1.0f, -1.0f, -1.0f);
	vb->Unlock();

	ib->Lock(0, 0, (void**)&indices, 0);
	indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;
	indices[3]  = 0; indices[4]  = 2; indices[5]  = 3;
	ib->Unlock();
}