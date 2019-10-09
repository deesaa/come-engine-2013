

class texture_class
{
private:
	IDirect3DDevice9* device;
	IDirect3DTexture9* texture;

	bool initialized;

public:
	void initBaseForTexture(IDirect3DDevice9* bDevice)
	{
		initialized = FALSE;
		device = bDevice;
	}

	void loadTexture(DWORD vertsID[], vertex* vertices, sphere_struct* vertsSpheres[])
	{
		initialized = TRUE;
		vertices[vertsID[0]].u = 0;
		vertices[vertsID[0]].v = 0;
		vertices[vertsID[1]].u = 1;
		vertices[vertsID[1]].v = 0;
		vertices[vertsID[2]].u = 0;
		vertices[vertsID[2]].v = 1;
		vertices[vertsID[3]].u = 1;
		vertices[vertsID[3]].v = 1;

		D3DXCreateTextureFromFile(device, L"Nyan.bmp", &texture);
	}

	void resetTexture()
	{
		if(initialized)
			device->SetTexture(0, texture);
		else
			device->SetTexture(0, 0);
	}
};