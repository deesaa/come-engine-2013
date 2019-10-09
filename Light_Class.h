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
		light.Diffuse		= D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		light.Specular		= D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f) * 0.6f;
		light.Ambient		= D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f) * 0.4f;
		light.Direction		= D3DXVECTOR3(-0.5f, 1.5f, 0.0f);
		device->SetLight(lightID, &light);
		device->LightEnable(lightID, true);
	}

	LPCTSTR getLightName()
	{	return lightName;}
};