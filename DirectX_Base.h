class directx9_class
{
private:
	IDirect3D9* d3d9;
	D3DCAPS9 caps;
	D3DPRESENT_PARAMETERS d3dpp;
	IDirect3DDevice9* device;
public:
	directx9_class(HWND windowHandle, HINSTANCE hInstance)
	{
		d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
		d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

		d3dpp.BackBufferWidth            = DirectXWidth;
		d3dpp.BackBufferHeight           = DirectXHeight;
		d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
		d3dpp.BackBufferCount            = 1;
		d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality         = 0;
		d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow              = windowHandle;
		d3dpp.Windowed                   = true; 
		d3dpp.EnableAutoDepthStencil     = true;
		d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
		d3dpp.Flags                      = 0;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

		d3d9->CreateDevice(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, windowHandle, 
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device);
	}

	IDirect3DDevice9* getDevice()
	{
		return device;
	}

	~directx9_class()
	{
		device->Release();
		d3d9->Release();
	}
};