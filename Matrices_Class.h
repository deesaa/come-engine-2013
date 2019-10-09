enum moveBy{View, World};

#define MOVE_FORWARD 9001
#define MOVE_BACK	 9002
#define MOVE_UP		 9003
#define MOVE_DOWN	 9004
#define MOVE_RIGHT	 9005
#define MOVE_LEFT	 9006

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

	D3DXMATRIX view;
	D3DXVECTOR3 viewRight;
	D3DXVECTOR3 viewUp;
	D3DXVECTOR3 viewLook;

	D3DXVECTOR3 localWorldRight;
	D3DXVECTOR3 localWorldUp;
	D3DXVECTOR3 localLook;

	D3DXVECTOR3 pos;

	D3DXMATRIX worldMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix;
	D3DXMATRIX vertsWorldMatrix;
	D3DXMATRIX finallyRotateMatrix;
	D3DXMATRIX finallyWorldMatrix;
public:
	worldMatrices_class() {}
	void fillMatrix(float x, float y, float z, IDirect3DDevice9* bDevice)
	{
		pos = D3DXVECTOR3(x, y, z);

		device = bDevice;

		D3DXMatrixTranslation(&worldMatrix, x, y, z);
	}

	void worldMatrixMove(float dX, float dY, float dZ, short moveType, moveBy moveSpace)
	{
		if(moveSpace == View)
		{
			switch(moveType)
			{
			case MOVE_FORWARD:
				break;
			case MOVE_BACK:
				break;
			case MOVE_UP:
				pos += viewUp * fabs(dY);
				break;
			case MOVE_DOWN:
				pos -= viewUp * fabs(dY);
				break;
			case MOVE_RIGHT:
				pos += viewRight * fabs(dX);
				break;
			case MOVE_LEFT:
				pos -= viewRight * fabs(dX);
				break;
			}
		}

		if(moveSpace == World)
		{
			switch(moveType)
			{
			case MOVE_FORWARD:
				break;
			case MOVE_BACK:
				break;
			case MOVE_UP:
				pos += D3DXVECTOR3(0.0f, 1.0f, 0.0f) * fabs(dY);
				break;
			case MOVE_DOWN:
				pos -= D3DXVECTOR3(0.0f, 1.0f, 0.0f) * fabs(dY);
				break;
			case MOVE_RIGHT:
				pos += D3DXVECTOR3(1.0f, 0.0f, 0.0f) * fabs(dX);
				break;
			case MOVE_LEFT:
				pos -= D3DXVECTOR3(1.0f, 0.0f, 0.0f) * fabs(dX);
				break;
			}
		}
	}

	void moveVert(D3DXVECTOR3* position, float dX, float dY, float dZ, short moveType)
	{
		switch(moveType)
		{
		case MOVE_FORWARD:
			break;
		case MOVE_BACK:
			break;
		case MOVE_UP:
			*position += localWorldUp * fabs(dY);
			break;
		case MOVE_DOWN:
			*position -= localWorldUp * fabs(dY);
			break;
		case MOVE_RIGHT:
			*position += localWorldRight * fabs(dX);
			break;
		case MOVE_LEFT:
			*position -= localWorldRight * fabs(dX);
			break;
		}
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

	D3DXVECTOR3* getRight()
	{
		return &viewRight;
	}
	D3DXVECTOR3* getUp()
	{
		return &viewUp;
	}
	D3DXVECTOR3* getLook()
	{
		return &viewLook;
	}
	void resetMatrices()
	{
		localWorldRight.x = (finallyRotateMatrix)(0, 0);
		localWorldUp.x    = (finallyRotateMatrix)(0, 1);
		localLook.x  = (finallyRotateMatrix)(0, 2);

		localWorldRight.y = (finallyRotateMatrix)(1, 0);
		localWorldUp.y    = (finallyRotateMatrix)(1, 1);
		localLook.y  = (finallyRotateMatrix)(1, 2);

		localWorldRight.z = (finallyRotateMatrix)(2, 0);
		localWorldUp.z    = (finallyRotateMatrix)(2, 1);
		localLook.z  = (finallyRotateMatrix)(2, 2);

		D3DXVec3Normalize(&localLook, &localLook);

		D3DXVec3Cross(&localWorldUp, &localLook, &localWorldRight);
		D3DXVec3Normalize(&localWorldUp, &localWorldUp);

		D3DXVec3Cross(&localWorldRight, &localWorldUp, &localLook);
		D3DXVec3Normalize(&localWorldRight, &localWorldRight);


		device->GetTransform(D3DTS_VIEW, &view);
		viewRight.x = (view)(0, 0);
		viewUp.x    = (view)(0, 1);
		viewLook.x  = (view)(0, 2);

		viewRight.y = (view)(1, 0);
		viewUp.y    = (view)(1, 1);
		viewLook.y  = (view)(1, 2);

		viewRight.z = (view)(2, 0);
		viewUp.z    = (view)(2, 1);
		viewLook.z  = (view)(2, 2);

		D3DXVec3Normalize(&viewLook, &viewLook);

		D3DXVec3Cross(&viewUp, &viewLook, &viewRight);
		D3DXVec3Normalize(&viewUp, &viewUp);

		D3DXVec3Cross(&viewRight, &viewUp, &viewLook);
		D3DXVec3Normalize(&viewRight, &viewRight);

		D3DXMatrixTranslation(&worldMatrix, pos.x, pos.y, pos.z);

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
