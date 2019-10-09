class material_class
{
private:
	IDirect3DDevice9* device;
	D3DMATERIAL9 material;
public:
	void initMaterialBase(IDirect3DDevice9* bDevice)
	{
		device = bDevice;

		ZeroMemory(&material, sizeof(material));
		material.Diffuse  = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f); 
		material.Ambient  = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f); 
		material.Specular = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		material.Emissive = D3DXCOLOR(0.15f, 0.0f, 0.0f, 1.0f); // нет свечения
		material.Power = 9.0f;
	}

	D3DMATERIAL9* getMaterial()
	{
		return &material;
	}

	void resetMaterial()
	{
		device->SetMaterial(&material);
	}
};