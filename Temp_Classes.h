class tempObject_class
{
public:
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

	tempObject_class()
	{
		numSubsetsAttributes = numSubsets = numFaces =
			numVerts = numMaterials = numTextures = numRendState = 0;
	}

	void loadVertex(std::vector<float> bVertex)
	{
		vertices[numVerts] = new vertex;
		vertices[numVerts]->pos.x = bVertex.at(0);
		vertices[numVerts]->pos.y = bVertex.at(1);
		vertices[numVerts]->pos.z = bVertex.at(2);
		vertices[numVerts]->nX	  = bVertex.at(3);
		vertices[numVerts]->nY	  = bVertex.at(4);
		vertices[numVerts]->nZ	  = bVertex.at(5);
		numVerts += 1;
	}

	~tempObject_class()
	{
		for(DWORD counter(0); counter != numVerts;)
		{
			delete vertices[counter];
			counter += 1;
		}
	}
};

class temp_manager
{
public:
	DWORD numObject, numFactObjects;					//Счетчик объектов
	DWORD numLight, numFactLight;						//Счетчик объектов света
	DWORD numCams, numFactCams;
	DWORD numGlobal, numFactGlobal;

	tempObject_class* object[512];				//Массив сылок на объекты
	//tempLight_class*  light[128];				//Массив сылок на объекты света
	//tempCamera_class* camera[64];

	temp_manager()
	{
		numObject =	numLight = numCams = numGlobal =
			numFactObjects = numFactLight = numFactCams = numFactGlobal = 0;
	}

	void addObject(std::string* file)
	{
		object[numFactObjects] = new tempObject_class;
		openObjectVertices(object[numFactObjects], file);
		numFactObjects += 1;
		numFactGlobal  += 1;
	}

	~temp_manager()
	{
		for(;numFactObjects != NULL;)
		{
			numFactObjects--;
			delete object[numFactObjects];
		}
	}
};

