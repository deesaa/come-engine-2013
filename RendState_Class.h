class rendState_class
{
private:
	IDirect3DDevice9* device;
	rendStateTypes_class* rendStateTypes;

public:
	std::vector<DWORD> vecRendStateType;
	std::vector<DWORD> vecRendStateValue;
	DWORD numElements;

	bool isShader;

	void initRendStateClass(IDirect3DDevice9* bDevice, rendStateTypes_class* bRendStateTypes)
	{
		device = bDevice;
		rendStateTypes = bRendStateTypes;
		numElements = 0;
		isShader = FALSE;

		for(DWORD counter(0); counter != rendStateTypes->numInitedTypes;)
		{
			vecRendStateType.push_back(counter);
			vecRendStateValue.push_back(0);
			numElements += 1;
			counter += 1;
		}
	}

	void setRendState()
	{
		{
			for(DWORD counter(0); counter != numElements;)
			{
				device->SetRenderState(rendStateTypes->rendStateType[vecRendStateType.at(counter)]->rendStateType, 
					rendStateTypes->rendStateType[vecRendStateType.at(counter)]->rendStateValues[vecRendStateValue.at(counter)]);
				counter += 1;
			}
		}
	}
};
