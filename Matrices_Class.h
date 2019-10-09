class fullMatrices_class
{
private:
	IDirect3DDevice9* device;
	D3DXMATRIX	 worldMatrix;
	D3DXMATRIX	 viewMatrix;
	D3DXMATRIX	 projMatrix;

public:
	fullMatrices_class(){}
	fullMatrices_class(float x, float y, float z, float fovY, IDirect3DDevice9* bDevice)
	{
		device = bDevice;
		D3DXMatrixTranslation(&worldMatrix, x, y, z);

		viewMatrix._11=1; viewMatrix._12=0; viewMatrix._13=0;  viewMatrix._14=0;
		viewMatrix._21=0; viewMatrix._22=1; viewMatrix._23=0;  viewMatrix._24=0;
		viewMatrix._31=0; viewMatrix._32=0; viewMatrix._33=1;  viewMatrix._34=0;
		viewMatrix._41=0; viewMatrix._42=0; viewMatrix._43=10; viewMatrix._44=1;

		D3DXMatrixPerspectiveFovLH(&projMatrix, PI * fovY, DirectXWidth/DirectXHeight, 1.0f, 1000.0f);
		setMatrices(&worldMatrix, &viewMatrix, &projMatrix, bDevice);
	}

	void fillMatrices(float x, float y, float z, float fovY, IDirect3DDevice9* bDevice)
	{
		device = bDevice;
		D3DXMatrixTranslation(&worldMatrix, x, y, z);

		viewMatrix._11=1; viewMatrix._12=0; viewMatrix._13=0;  viewMatrix._14=0;
		viewMatrix._21=0; viewMatrix._22=1; viewMatrix._23=0;  viewMatrix._24=0;
		viewMatrix._31=0; viewMatrix._32=0; viewMatrix._33=1;  viewMatrix._34=0;
		viewMatrix._41=0; viewMatrix._42=0; viewMatrix._43=10; viewMatrix._44=1;

		D3DXMatrixPerspectiveFovLH(&projMatrix, D3DX_PI * fovY, (float)DirectXWidth/(float)DirectXHeight, 1.0f, 1000.0f);
		setMatrices(&worldMatrix, &viewMatrix, &projMatrix, device);
	}
};

void setMatrices(D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix, IDirect3DDevice9* device)
{
	device->SetTransform(D3DTS_WORLD, worldMatrix);
	device->SetTransform(D3DTS_VIEW, viewMatrix);
	device->SetTransform(D3DTS_PROJECTION, projMatrix);
}