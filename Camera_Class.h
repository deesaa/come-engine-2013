#define MOVE_FORVARD 1
#define MOVE_BACK	 2
#define MOVE_UP		 3
#define MOVE_DOWN	 4
#define MOVE_RIGHT	 5
#define MOVE_LEFT	 6

#define ROTATE_YAW   1
#define ROTATE_PITCH 2

class camera_class
{
private:
	IDirect3DDevice9* device;

	D3DXMATRIX	 viewMatrix;
	D3DXMATRIX	 projMatrix;
	D3DXMATRIX   rotateBuffer;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 look;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;

	DWORD ObjectID;
	char buffer[256];
	LPCTSTR camName;

public:
	void initCamBase(IDirect3DDevice9* bDevice)
	{
		device = bDevice;
		camName = L"Camera";
		pos   = D3DXVECTOR3(0.0f, 0.0f, -20.0f);
		look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

		this->resetMatrices();
	}

	void move(short moveType)
	{
		switch(moveType)
		{
		case MOVE_FORVARD:
			pos += look * 0.025;
			break;
		case MOVE_BACK:
			pos -= look * 0.025;
			break;
		case MOVE_UP:
			break;
		case MOVE_DOWN:
			break;
		case MOVE_RIGHT:
			pos += D3DXVECTOR3(right.x, 0.0f, right.z) * 0.025;
			break;
		case MOVE_LEFT:
			pos -= D3DXVECTOR3(right.x, 0.0f, right.z) * 0.025;
			break;
		}
		this->resetMatrices();
	}

	void rotate(float dX, float dY)
	{
		if(dX > 0)
		{
			D3DXMatrixRotationAxis(&rotateBuffer, &up, dX * 0.15f);
			D3DXVec3TransformCoord(&right, &right, &rotateBuffer);
			D3DXVec3TransformCoord(&look, &look, &rotateBuffer);
		}
		if(dX < 0)
		{
			D3DXMatrixRotationAxis(&rotateBuffer, &up, dX * 0.15f);
			D3DXVec3TransformCoord(&right, &right, &rotateBuffer);
			D3DXVec3TransformCoord(&look, &look, &rotateBuffer);
		}
		if(dY > 0)
		{
			D3DXMatrixRotationAxis(&rotateBuffer, &right, dY * 0.15f);
			D3DXVec3TransformCoord(&up, &up, &rotateBuffer);
			D3DXVec3TransformCoord(&look, &look, &rotateBuffer);
		}
		if(dY < 0)
		{
			D3DXMatrixRotationAxis(&rotateBuffer, &right, dY * 0.15f);
			D3DXVec3TransformCoord(&up, &up, &rotateBuffer);
			D3DXVec3TransformCoord(&look, &look, &rotateBuffer);
		}
		this->resetMatrices();
	}

	LPCTSTR getCamName()
	{	return camName; }

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

		D3DXMatrixPerspectiveFovLH(&projMatrix, D3DX_PI * 0.2f, (float)DirectXWidth/(float)DirectXHeight, 0.01f, 1000.0f);

		device->SetTransform(D3DTS_VIEW, &viewMatrix);
		device->SetTransform(D3DTS_PROJECTION, &projMatrix);
	}
};