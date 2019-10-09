class viewMatrices_class
{
private:
	IDirect3DDevice9* device;
	float AngleX, AngleY, AngleZ;
	D3DXMATRIX	 viewMatrix;
	D3DXMATRIX	 projMatrix;

	D3DXVECTOR3 right;
	D3DXVECTOR3 up;
	D3DXVECTOR3 look;
	D3DXVECTOR3 pos;

	D3DXMATRIX   rotateXMatrix, rotateYMatrix, rotateZMatrix;
	D3DXMATRIX   finallyCamMatrix;

public:
	viewMatrices_class(){}
	void fillMatrices(float fovY, IDirect3DDevice9* bDevice, D3DXVECTOR3* position, D3DXVECTOR3* target, D3DXVECTOR3* bUp)
	{
		device = bDevice;
		right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		up   = *bUp;
		look = *target;
		pos  = *position;

		D3DXVec3Normalize(&look, &look);

		D3DXVec3Cross(&up, &look, &right);
		D3DXVec3Normalize(&up, &up);

		D3DXVec3Cross(&right, &up, &look);
		D3DXVec3Normalize(&right, &right);

		// Строим матрицу вида:
	

		D3DXMatrixLookAtLH(&viewMatrix, &pos, &look, &up);

		D3DXMatrixPerspectiveFovLH(&projMatrix, D3DX_PI * fovY, (float)DirectXWidth/(float)DirectXHeight, 1.0f, 1000.0f);

		setViewMatrices(&viewMatrix, &projMatrix, device);
	}

	void fillSimpleMatrices(float x, float y, float z, float fovY, IDirect3DDevice9* bDevice, D3DXVECTOR3* position, D3DXVECTOR3* target, D3DXVECTOR3* up)
	{
		device = bDevice;

		D3DXMatrixLookAtLH(&viewMatrix, position, target, up);
		D3DXMatrixPerspectiveFovLH(&projMatrix, D3DX_PI * fovY, (float)DirectXWidth/(float)DirectXHeight, 1.0f, 1000.0f);

		setViewMatrices(&viewMatrix, &projMatrix, device);
	}

	void moveForward()
	{
		pos += D3DXVECTOR3(look.x, 0.0f, look.z);
	}

	void worldMatrixRotateX(float Angle)
	{
		AngleX += Angle;
		if(AngleX >= 6.28f)
			AngleX = 0.0f;
		D3DXMatrixRotationX(&rotateXMatrix, AngleX);
	}

	void worldMatrixRotateY(float Angle)
	{
		AngleY += Angle;
		if(AngleY >= 6.28f)
			AngleY = 0.0f;
		D3DXMatrixRotationY(&rotateYMatrix, AngleY);
	}

	void worldMatrixRotateZ(float Angle)
	{
		AngleZ += Angle;
		if(AngleZ >= 6.28f)
			AngleZ = 0.0f;
		D3DXMatrixRotationZ(&rotateZMatrix, AngleZ);
	}

	void resetMatrices()
	{
		D3DXVec3Normalize(&look, &look);

		D3DXVec3Cross(&up, &look, &right);
		D3DXVec3Normalize(&up, &up);

		D3DXVec3Cross(&right, &up, &look);
		D3DXVec3Normalize(&right, &right);

		// Строим матрицу вида:
		float x = -D3DXVec3Dot(&right, &pos);
		float y = -D3DXVec3Dot(&up, &pos);
		float z = -D3DXVec3Dot(&look, &pos);

		(viewMatrix)(0, 0) = right.x;
		(viewMatrix)(0, 1) = up.x;
		(viewMatrix)(0, 2) = look.x;
		(viewMatrix)(0, 3) = 0.0f;

		(viewMatrix)(1, 0) = right.y;
		(viewMatrix)(1, 1) = up.y;
		(viewMatrix)(1, 2) = look.y;
		(viewMatrix)(1, 3) = 0.0f;

		(viewMatrix)(2, 0) = right.z;
		(viewMatrix)(2, 1) = up.z;
		(viewMatrix)(2, 2) = look.z;
		(viewMatrix)(2, 3) = 0.0f;

		(viewMatrix)(3, 0) = x;
		(viewMatrix)(3, 1) = y;
		(viewMatrix)(3, 2) = z;
		(viewMatrix)(3, 3) = 1.0f;

		setViewMatrices(&viewMatrix, &projMatrix, device);
	}
};

class worldMatrices_class
{
private:
	IDirect3DDevice9* device;
	float moveX, moveY, moveZ;
	float AngleX, AngleY, AngleZ;

	D3DXMATRIX worldMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix;
	D3DXMATRIX finallyRotateMatrix;
	D3DXMATRIX finallyWorldMatrix;
public:
	worldMatrices_class() {}
	void fillMatrix(float x, float y, float z, IDirect3DDevice9* bDevice)
	{
		device = bDevice;
		D3DXMatrixTranslation(&worldMatrix, x, y, z);
	}
	void worldMatrixMove(float x, float y, float z)
	{
		moveX += x;
		moveY -= y;
		moveZ += z;
		D3DXMatrixTranslation(&worldMatrix, moveX, moveY, moveZ);
	}

	void worldMatrixRotateX(float Angle)
	{
		AngleX += Angle;
		if(AngleX >= 6.28f)
			AngleX = 0.0f;
		D3DXMatrixRotationX(&rotateXMatrix, AngleX);
	}

	void worldMatrixRotateY(float Angle)
	{
		AngleY += Angle;
		if(AngleY >= 6.28f)
			AngleY = 0.0f;
		D3DXMatrixRotationY(&rotateYMatrix, AngleY);
	}

	void worldMatrixRotateZ(float Angle)
	{
		AngleZ += Angle;
		if(AngleZ >= 6.28f)
			AngleZ = 0.0f;
		D3DXMatrixRotationZ(&rotateZMatrix, AngleZ);
	}

	D3DXMATRIX getFinallyWorldMatrix()
	{	return finallyWorldMatrix;}

	D3DXMATRIX getWorldMatrix()
	{	return worldMatrix;}

	D3DXMATRIX getFinallyRotateMatrix()
	{	return finallyRotateMatrix;}

	void resetMatrices()
	{
		finallyRotateMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;
		finallyWorldMatrix  = finallyRotateMatrix * worldMatrix;
		device->SetTransform(D3DTS_WORLD, &finallyWorldMatrix);
	}
};

void setAllMatrices(D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix, IDirect3DDevice9* device)
{
	device->SetTransform(D3DTS_WORLD, worldMatrix);
	device->SetTransform(D3DTS_VIEW, viewMatrix);
	device->SetTransform(D3DTS_PROJECTION, projMatrix);
}

void setViewMatrices(D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix, IDirect3DDevice9* device)
{
	device->SetTransform(D3DTS_VIEW, viewMatrix);
	device->SetTransform(D3DTS_PROJECTION, projMatrix);
}
