class fullMatrices_class
{
private:
	IDirect3DDevice9* device;
	D3DXMATRIX	 worldMatrix;
	D3DXMATRIX	 viewMatrix;
	D3DXMATRIX	 projMatrix;

	D3DXMATRIX   rotateXMatrix, rotateYMatrix, rotateZMatrix;

	D3DXMATRIX   finallyWorldMatrix;

public:
	fullMatrices_class(){}

	void fillMatrices(float x, float y, float z, float fovY, IDirect3DDevice9* bDevice, D3DXVECTOR3* position, D3DXVECTOR3* target, D3DXVECTOR3* up)
	{
		device = bDevice;
		D3DXMatrixTranslation(&worldMatrix, x, y, z);

		D3DXMatrixLookAtLH(&viewMatrix, position, target, up);

		D3DXMatrixPerspectiveFovLH(&projMatrix, D3DX_PI * fovY, (float)DirectXWidth/(float)DirectXHeight, 1.0f, 1000.0f);
		setMatrices(&worldMatrix, &viewMatrix, &projMatrix, device);
	}

	void worldMatrixMove(float x, float y, float z)
	{
		D3DXMatrixTranslation(&worldMatrix, x, y, z);
	}

	void worldMatrixRotateX(float Angle)
	{
		D3DXMatrixRotationX(&rotateXMatrix, Angle);
	}

	void worldMatrixRotateY(float Angle)
	{
		D3DXMatrixRotationY(&rotateYMatrix, Angle);
	}

	void worldMatrixRotateZ(float Angle)
	{
		D3DXMatrixRotationZ(&rotateZMatrix, Angle);
	}

	void resetWorldMatrices()
	{
		finallyWorldMatrix = (rotateXMatrix * rotateYMatrix * rotateZMatrix) * worldMatrix;
		device->SetTransform(D3DTS_WORLD, &finallyWorldMatrix);
	}
};

class worldMatrix_class
{
private:
	D3DXMATRIX worldMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix;
	D3DXMATRIX finallyWorldMatrix;
	IDirect3DDevice9* device;
public:
	worldMatrix_class() {}
	void fillMatrix(float x, float y, float z, IDirect3DDevice9* bDevice)
	{
		device = bDevice;
		D3DXMatrixTranslation(&worldMatrix, x, y, z);
	}
	void worldMatrixMove(float x, float y, float z)
	{
		D3DXMatrixTranslation(&worldMatrix, x, y, z);
	}

	void worldMatrixRotateX(float Angle)
	{
		D3DXMatrixRotationX(&rotateXMatrix, Angle);
	}

	void worldMatrixRotateY(float Angle)
	{
		D3DXMatrixRotationY(&rotateYMatrix, Angle);
	}

	void worldMatrixRotateZ(float Angle)
	{
		D3DXMatrixRotationZ(&rotateZMatrix, Angle);
	}

	void resetWorldMatrices()
	{
		finallyWorldMatrix = (rotateXMatrix * rotateYMatrix * rotateZMatrix) * worldMatrix;
		device->SetTransform(D3DTS_WORLD, &finallyWorldMatrix);
	}
};

void setMatrices(D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix, IDirect3DDevice9* device)
{
	device->SetTransform(D3DTS_WORLD, worldMatrix);
	device->SetTransform(D3DTS_VIEW, viewMatrix);
	device->SetTransform(D3DTS_PROJECTION, projMatrix);
}
