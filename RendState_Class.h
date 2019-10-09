class rendState_class
{
private:
	IDirect3DDevice9* device;
	rendStateTypes_class* rendStateTypes;

public:
	DWORD* rendStateType[64];
	DWORD* rendStateValue[64];
	DWORD numElements;

	void initRendStateClass(IDirect3DDevice9* bDevice, rendStateTypes_class* bRendStateTypes)
	{
		device = bDevice;
		rendStateTypes = bRendStateTypes;
		numElements = 0;
	}

	void addRendState(DWORD RSType, DWORD RSValue)
	{
		rendStateType[numElements]   = new DWORD;
		*rendStateType[numElements]  = RSType;
		rendStateValue[numElements]  = new DWORD;
		*rendStateValue[numElements] = RSValue;
		numElements += 1;
	}

	void setRendState()
	{
		for(DWORD counter(0); counter != numElements;)
		{
			device->SetRenderState(rendStateTypes->rendStateType[*rendStateType[counter]]->rendStateType, 
				rendStateTypes->rendStateType[*rendStateType[counter]]->rendStateValues[*rendStateValue[counter]]);
			counter += 1;
		}
	}

	~rendState_class()
	{
		for(;numElements != 0;)
		{
			numElements -= 1;
			delete rendStateType[numElements];
			delete rendStateValue[numElements];
		}
	}
};
