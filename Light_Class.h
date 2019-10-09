#define DIRECTION_UP	0
#define DIRECTION_DOWN	1
#define DIRECTION_RIGHT 2
#define DIRECTION_LEFT	3

#define DIRECTION_LIGHT 1
#define POINT_LIGHT		2
#define SPOT_LIGHT		2

class light_class
{
private:
	D3DLIGHT9 light;
	UINT lightID;
	LPCTSTR lightName;
	short lightType;
	material_class material;

	IDirect3DDevice9* device;
public:
	void initDirectionLightBase(IDirect3DDevice9* bDevice, UINT lightNumber)
	{
		device = bDevice;
		lightID = lightNumber;
		lightName = L"Direction Light";
		lightType = DIRECTION_LIGHT;
		ZeroMemory(&light, sizeof(light));

		light.Type			= D3DLIGHT_DIRECTIONAL;
		light.Diffuse		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light.Specular		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.6f;
		light.Ambient		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.4f;
		light.Direction		= D3DXVECTOR3(-0.5f, 1.5f, 0.0f);

		device->SetLight(lightID, &light);
		device->LightEnable(lightID, true);
	}

	void initPointLightBase(IDirect3DDevice9* bDevice, UINT lightNumber)
	{
		device = bDevice;
		lightID = lightNumber;
		lightName = L"Point Light";
		lightType = POINT_LIGHT;
		ZeroMemory(&light, sizeof(light));

		light.Type			= D3DLIGHT_POINT;
		light.Position		= D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		light.Diffuse		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light.Specular		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.6f;
		light.Ambient		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.4f;
		light.Attenuation0  = 0.0f;
		light.Attenuation1  = 1.0f;
		light.Attenuation2  = 0.0f;
		light.Range         = 100;

		device->SetLight(lightID, &light);
		device->LightEnable(lightID, true);
	}

	void initSpotLightBase(IDirect3DDevice9* bDevice, UINT lightNumber)
	{
		device = bDevice;
		lightID = lightNumber;
		lightName = L"Spot Light";
		lightType = SPOT_LIGHT;
		ZeroMemory(&light, sizeof(light));

		light.Type			= D3DLIGHT_SPOT;
		light.Position		= D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		light.Direction		= D3DXVECTOR3(-0.5f, 1.5f, 0.0f);
		light.Diffuse		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		light.Specular		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.6f;
		light.Ambient		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.4f;
		light.Attenuation0  = 0.0f;
		light.Attenuation1  = 1.0f;
		light.Attenuation2  = 0.0f;
		light.Theta			= 0.4f;
		light.Phi			= 1.2f;
		light.Falloff		= 1.0f;
		light.Range         = 100;

		device->SetLight(lightID, &light);
		device->LightEnable(lightID, true);
	}

	void redirectLight(int DIRECT_TYPE)
	{
		if(lightType == DIRECTION_LIGHT)
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
	}

	void createObjectOfLight()
	{
		
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
	{	return &light;}

	short getLightType()
	{

	}
};