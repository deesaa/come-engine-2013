class material_class
{
private:
	IDirect3DDevice9* device;
	D3DMATERIAL9 material;
	DWORD materialID; 
public:
	void initMaterialBase(IDirect3DDevice9* bDevice, DWORD bMaterialID)
	{
		device = bDevice;
		materialID = bMaterialID;

		ZeroMemory(&material, sizeof(material));
		material.Diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); 
		material.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); 
		material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f); // нет свечения
		material.Power = 9.0f;	
	}

	void initAbsolutelyWhiteMaterial(IDirect3DDevice9* bDevice, DWORD bMaterialID)
	{
		device = bDevice;
		materialID = bMaterialID;

		ZeroMemory(&material, sizeof(material));
		material.Diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); 
		material.Ambient  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); 
		material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		material.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // нет свечения
		material.Power = 0.0f;	
	}

	D3DMATERIAL9* getMaterial()
	{
		return &material;
	}
	DWORD getMaterialID()
	{
		return materialID;
	}
	void resetMaterial()
	{
		device->SetMaterial(&material);
	}
	material_class* getThis()
	{
		return this;
	}
};