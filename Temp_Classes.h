class temp_manager
{
public:
	DWORD numObject, numFactObjects;					//������� ��������
	DWORD numLight, numFactLight;						//������� �������� �����
	DWORD numCams, numFactCams;
	DWORD numGlobal, numFactGlobal;

	tempObject_class* object[512];				//������ ����� �� �������
	//tempLight_class*  light[128];				//������ ����� �� ������� �����
	//tempCamera_class* camera[64];

	temp_manager()
	{
		numObject =	numLight = numCams = numGlobal = 0;
		numFactObjects = numFactLight = numFactCams = numFactGlobal = 0;
	}

	void loadObject()
	{
		numObject += 1;
	}

	void loadVertex(std::vector<float> vertex)
	{

	}
};

class tempObject_class
{
	vertex* vertices[512];
	WORD* indices;
	DWORD* attributeBuffer;
	triangle* triangles[512];

	DWORD* adjacencyInfo;
	D3DMATERIAL9* materials;

	DWORD numSubsetsAttributes;
	DWORD numSubsets;
	DWORD numFaces;
	DWORD numVerts;

	DWORD numMaterials;
	DWORD numTextures;
	DWORD numRendState;
};