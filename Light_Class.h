#define DIRECTION_UP	0
#define DIRECTION_DOWN	1
#define DIRECTION_RIGHT 2
#define DIRECTION_LEFT	3

class light_class
{
private:
	D3DLIGHT9 light;
	UINT lightID;
	LPCTSTR lightName;

	IDirect3DDevice9* device;
public:
	void initDirectionLightBase(IDirect3DDevice9* bDevice, UINT lightNumber)
	{
		device = bDevice;
		lightID = lightNumber;
		lightName = L"Direction Light";
		ZeroMemory(&light, sizeof(light));

		light.Type			= D3DLIGHT_DIRECTIONAL;
		light.Diffuse		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light.Specular		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.6f;
		light.Ambient		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.4f;
		light.Direction		= D3DXVECTOR3(-0.5f, 1.5f, 0.0f);
		device->SetLight(lightID, &light);
		device->LightEnable(lightID, true);
	}

	void redirectLight(int DIRECT_TYPE)
	{
		
		switch(DIRECT_TYPE)
		{
		case DIRECTION_UP:
			light.Direction =  D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			break;
		case DIRECTION_DOWN:
			light.Direction = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			break;
		case DIRECTION_RIGHT: 
			light.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			break;
		case DIRECTION_LEFT:
			light.Direction = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			break;
		}
		device->SetLight(lightID, &light);
	}

	void setLight()
	{
		device->SetLight(lightID, &light);
	}

	LPCTSTR getLightName()
	{	return lightName;}

	void enableLight()
	{	device->LightEnable(lightID, true);}

	void disableLight()
	{	device->LightEnable(lightID, false);}

	D3DLIGHT9* getLight()
	{
		return &light;
	}
};