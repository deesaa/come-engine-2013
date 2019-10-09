struct rendState_struct
{
	std::wstring strRendStateType;
	std::vector<std::wstring> strRendStateValues;

	D3DRENDERSTATETYPE rendStateType;
	DWORD rendStateValues[32];

	DWORD numValues;

	rendState_struct(){}
	rendState_struct(std::wstring strType, std::vector<std::wstring> strValues, DWORD bNumValues, D3DRENDERSTATETYPE type, DWORD values[])
	{
		strRendStateType   = strType;
		strRendStateValues = strValues;
		rendStateType	   = type;
		numValues		   = bNumValues;
		for(DWORD counter(0); counter != numValues;)
		{
			rendStateValues[counter] = values[counter];
			counter += 1;
		}
	}
};

class rendStateTypes_class
{
public:
	std::vector<std::wstring> strValues;
	std::wstring strType;

	rendState_struct* rendStateType[128];
	DWORD numInitedTypes;

	void initRendStateStructs()
	{
		numInitedTypes = 0;

		rendStateType[numInitedTypes] = new rendState_struct;
		strType = L"RS_FILLMODE";
		strValues.clear();
		strValues.push_back(L"FILL_SOLID");
		strValues.push_back(L"FILL_POINT");
		strValues.push_back(L"FILL_WIREFRAME");
		{
			DWORD values[] = {D3DFILL_SOLID, D3DFILL_POINT, D3DFILL_WIREFRAME};
			*rendStateType[numInitedTypes] = rendState_struct(strType, strValues, strValues.size(), D3DRS_FILLMODE, values);
		}
		numInitedTypes += 1;

		rendStateType[numInitedTypes] = new rendState_struct;
		strType = L"RS_CULLMODE";
		strValues.clear();
		strValues.push_back(L"CULL_CCW");
		strValues.push_back(L"CULL_CW");
		strValues.push_back(L"CULL_NONE");
		{
			DWORD values[] = {D3DCULL_CCW, D3DCULL_CW, D3DCULL_NONE};
			*rendStateType[numInitedTypes] = rendState_struct(strType, strValues, strValues.size(), D3DRS_CULLMODE, values);
		}
		numInitedTypes += 1;

		rendStateType[numInitedTypes] = new rendState_struct;
		strType = L"RS_ZENABLE";
		strValues.clear();
		strValues.push_back(L"ZB_TRUE");
		strValues.push_back(L"ZB_FALSE");
		strValues.push_back(L"ZB_USEW");
		{
			DWORD values[] = {D3DZB_TRUE, D3DZB_FALSE, D3DZB_USEW};
			*rendStateType[numInitedTypes] = rendState_struct(strType, strValues, strValues.size(), D3DRS_ZENABLE, values);
		}
		numInitedTypes += 1;

		rendStateType[numInitedTypes] = new rendState_struct;
		strType = L"RS_LIGHTING";
		strValues.clear();
		strValues.push_back(L"LIGHTING_TRUE");
		strValues.push_back(L"LIGHTING_FALSE");
		{
			DWORD values[] = {TRUE, FALSE};
			*rendStateType[numInitedTypes] = rendState_struct(strType, strValues, strValues.size(), D3DRS_LIGHTING, values);
		}
		numInitedTypes += 1;

		rendStateType[numInitedTypes] = new rendState_struct;
		strType = L"RS_ALPHABLEND";
		strValues.clear();
		strValues.push_back(L"ALPHABLEND_FALSE");
		strValues.push_back(L"ALPHABLEND_TRUE");
		{
			DWORD values[] = {FALSE, TRUE};
			*rendStateType[numInitedTypes] = rendState_struct(strType, strValues, strValues.size(), D3DRS_ALPHABLENDENABLE, values);
		}
		numInitedTypes += 1;

		rendStateType[numInitedTypes] = new rendState_struct;
		strType = L"RS_SRCBLEND";
		strValues.clear();
		strValues.push_back(L"BLEND_SRCALPHA");
		strValues.push_back(L"BLEND_INVSRCALPHA");	
		strValues.push_back(L"BLEND_INVSRCCOLOR");	
		strValues.push_back(L"BLEND_INVSRCALPHA");	
		strValues.push_back(L"BLEND_INVDESTALPHA");	
		strValues.push_back(L"BLEND_INVDESTCOLOR");	
		strValues.push_back(L"BLEND_SRCALPHASAT");	
		strValues.push_back(L"BLEND_INVSRCALPHA");
		strValues.push_back(L"BLEND_BOTHINVSRCALPHA");
		{
			DWORD values[] = {D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, D3DBLEND_INVSRCCOLOR, 
				D3DBLEND_INVSRCALPHA, D3DBLEND_INVDESTALPHA, D3DBLEND_INVDESTCOLOR, 
				D3DBLEND_SRCALPHASAT, D3DBLEND_INVSRCALPHA, D3DBLEND_BOTHINVSRCALPHA};
			*rendStateType[numInitedTypes] = rendState_struct(strType, strValues, strValues.size(), D3DRS_SRCBLEND, values);
		}
		numInitedTypes += 1;

		rendStateType[numInitedTypes] = new rendState_struct;
		strType = L"RS_DESTBLEND";
		strValues.clear();
		strValues.push_back(L"BLEND_INVSRCALPHA");	
		strValues.push_back(L"BLEND_SRCALPHA");	
		strValues.push_back(L"BLEND_INVSRCCOLOR");	
		strValues.push_back(L"BLEND_INVSRCALPHA");	
		strValues.push_back(L"BLEND_INVDESTALPHA");	
		strValues.push_back(L"BLEND_INVDESTCOLOR");	
		strValues.push_back(L"BLEND_SRCALPHASAT");	
		strValues.push_back(L"BLEND_INVSRCALPHA");
		{
			DWORD values[] = {D3DBLEND_INVSRCALPHA, D3DBLEND_SRCALPHA, D3DBLEND_INVSRCCOLOR, 
				D3DBLEND_INVSRCALPHA, D3DBLEND_INVDESTALPHA, D3DBLEND_INVDESTCOLOR, 
				D3DBLEND_SRCALPHASAT, D3DBLEND_INVSRCALPHA};
			*rendStateType[numInitedTypes] = rendState_struct(strType, strValues, strValues.size(), D3DRS_DESTBLEND, values);
		}
		numInitedTypes += 1;
	}

	~rendStateTypes_class()
	{
		for(;numInitedTypes != 0;)
		{
			numInitedTypes -= 1;
			delete rendStateType[numInitedTypes];
		}
	}
};