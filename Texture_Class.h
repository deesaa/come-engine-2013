

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

	void loadTexture4Verts(DWORD vertsID[], vertex* vertices, sphere_struct* vertsSpheres[])
	{
		initialized = TRUE;
		D3DXVECTOR3 textureVerts[4];
		DWORD textureVertsID[4];
		for(DWORD counter(0); counter != 4;)
		{
			textureVerts[counter].x = (vertices[vertsID[0]].pos.x + vertices[vertsID[1]].pos.x + vertices[vertsID[2]].pos.x + vertices[vertsID[3]].pos.x) / 4;
			textureVerts[counter].y = (vertices[vertsID[0]].pos.y + vertices[vertsID[1]].pos.y + vertices[vertsID[2]].pos.y + vertices[vertsID[3]].pos.y) / 4;
			for (DWORD counter_2(0); counter_2 != 4;)
			{
				if(vertices[vertsID[counter_2]].pos.x < textureVerts[counter].x &&
				   vertices[vertsID[counter_2]].pos.y > textureVerts[counter].y)
				{
					textureVertsID[0] = vertsID[counter_2];
				}
				else
				if(vertices[vertsID[counter_2]].pos.x > textureVerts[counter].x && 
				   vertices[vertsID[counter_2]].pos.y > textureVerts[counter].y)
				{
					textureVertsID[1] = vertsID[counter_2];
				}
				else
				if(vertices[vertsID[counter_2]].pos.x > textureVerts[counter].x && 
				   vertices[vertsID[counter_2]].pos.y < textureVerts[counter].y)
				{
					textureVertsID[2] = vertsID[counter_2];
				}
				else
				if(vertices[vertsID[counter_2]].pos.x < textureVerts[counter].x && 
			       vertices[vertsID[counter_2]].pos.y < textureVerts[counter].y)
				{
					textureVertsID[3] = vertsID[counter_2];
				}
				counter_2 += 1;
			}
			counter += 1;
		}
		vertices[textureVertsID[0]].u = 0;
		vertices[textureVertsID[0]].v = 0;
		vertices[textureVertsID[1]].u = 1;
		vertices[textureVertsID[1]].v = 0;
		vertices[textureVertsID[2]].u = 1;
		vertices[textureVertsID[2]].v = 1;
		vertices[textureVertsID[3]].u = 0;
		vertices[textureVertsID[3]].v = 1;

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