class renderState_class
{
private:
	IDirect3DDevice9* device;
	D3DRENDERSTATETYPE* rendStateType[32];
	DWORD* rendStateValue[32];
	DWORD numElements;
public:
	void initRendStateClass(IDirect3DDevice9* bDevice)
	{
		device = bDevice;
		numElements = 0;
		rendStateType[numElements]   = new D3DRENDERSTATETYPE;
		*rendStateType[numElements]  = D3DRS_FILLMODE;
		rendStateValue[numElements]  = new DWORD;
		*rendStateValue[numElements] = D3DFILL_WIREFRAME;
		numElements += 1;
	}
	void setRenderState()
	{
		for(DWORD counter(0); counter != numElements;)
		{
			device->SetRenderState(*rendStateType[counter], *rendStateValue[counter]);
			counter += 1;
		}
	}
	~renderState_class()
	{
		for(;numElements != 0;)
		{
			numElements -= 1;
			delete rendStateType[numElements];
			delete rendStateValue[numElements];
		}
	}
};