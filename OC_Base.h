//Структура вершины сетки редактора объектов
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

//Прототипы (внутренних) функций редактора объектов
void COFillBuffers(IDirect3DVertexBuffer9* vb, IDirect3DIndexBuffer9* ib, COVertex* vertices, WORD* indices);

class object_creator
{
private:
	IDirect3DDevice9* device;
	HWND windowHandle;
	HINSTANCE hInstace;
	fullMatrices_class matrices;		//Объект класса набора матриц (мира, вида, проэктирования) для сетки редактора объектов

	IDirect3DVertexBuffer9* vb;
	IDirect3DIndexBuffer9* ib;

	COVertex* vertices;					//Вершины сетки редактора объектов
	WORD* indices;						//Индексы сетки редактора объектов

	d3dInput_class d3dInput;
	IDirectInputDevice8* KBDevice;
	char buffer[256];					// Буфер для хранения состояния клавиотуры

public:
	object_creator(IDirect3DDevice9* bDevice, HWND bWindowHandle, HINSTANCE bhInstance)
	{
		device = bDevice;
		windowHandle = bWindowHandle;
		hInstace = bhInstance;
		this->createBuffers();  //Создание буфера вершин и индексов					
		COFillBuffers(vb, ib, vertices, indices);  //Заполнение буфера вершин и интексов сетки редактора объектов
		matrices.fillMatrices(0, 0, 0, 0.5f, device);  //Заполнение и установка всех трех матриц
		d3dInput.fillInputClass(windowHandle, hInstace);
		KBDevice = d3dInput.getKBDevice();
	}

	//Создание буфера вершин и индексов	
	void createBuffers()
	{
		device->CreateVertexBuffer(
			8 * sizeof(COVertex),
			D3DUSAGE_WRITEONLY,
			COVertex::FVF,
			D3DPOOL_MANAGED,
			&vb,
			0);

		device->CreateIndexBuffer(
			36 * sizeof(WORD),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&ib,
			0);
	}

	void applyKBChanges()
	{
		if(FAILED(KBDevice->GetDeviceState(sizeof(buffer),buffer)))
			KBDevice->Acquire();
		if (buffer[DIK_RIGHT] & 0x80) 
			device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0x00000000, 1.0f, 0);
	}

	//Перерисовка редактора объектов
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

//Заполнение буфера вершин и интексов сетки редактора объектов
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