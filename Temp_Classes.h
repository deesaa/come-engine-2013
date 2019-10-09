class tempObject_class
{
public:
	vertex* vertices[512];
	WORD* indices[1024];
	DWORD* attributeBuffer[1024];
	triangle* triangles[512];

	DWORD* adjacencyInfo[1024];
	D3DMATERIAL9* materials;

	DWORD numSubsetsAttributes;
	DWORD numSubsets;
	DWORD numFaces;
	DWORD numIndices;
	DWORD numAttributes;
	DWORD numAdjacency;
	DWORD numVerts;

	DWORD numMaterials;
	DWORD numTextures;
	DWORD numRendState;

	tempObject_class()
	{
		numSubsetsAttributes = numSubsets = numFaces = numAdjacency = numAttributes =
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

	void loadIndices(std::vector<DWORD> bInds)
	{
		numIndices = bInds.size();												//??/?/?/?///?????

		for(DWORD counter(0); counter != numIndices;)
		{
			indices[counter] = new WORD;
			*indices[counter] = (WORD)bInds.at(counter);
			counter += 1;
		}
	}

	void loadAttributes(std::vector<DWORD> bAttribs)
	{
		numAttributes = bAttribs.size();
		for(DWORD counter(0); counter != numAttributes;)
		{
			attributeBuffer[counter] = new DWORD;
			*attributeBuffer[counter] = bAttribs.at(counter);
			counter += 1;
		}
	}

	void loadAdjacency(std::vector<DWORD> bAdjacency)
	{
		numAdjacency = bAdjacency.size();

		for(DWORD counter(0); counter != numAdjacency;)
		{
			adjacencyInfo[counter] = new DWORD;
			*adjacencyInfo[counter] = bAdjacency.at(counter);
			counter += 1;
		}
	}

	void loadTriangle(DWORD objectID, std::vector<DWORD> bVerts, std::vector<DWORD> bVertsFathers, DWORD subsetID)
	{
		triangles[numFaces] = new triangle;
		*triangles[numFaces] = triangle(Full, bVerts.at(0), bVerts.at(1), bVerts.at(2), objectID, 
			subsetID, subsetID, bVertsFathers.at(0), bVertsFathers.at(1), bVertsFathers.at(2));
		numFaces += 1;
	}

	~tempObject_class()
	{
		for(DWORD counter(0); counter != numVerts;)
		{
			delete vertices[counter];
			counter += 1;
		}
		for(DWORD counter(0); counter != numIndices;)
		{
			delete indices[counter];
			counter += 1;
		}
		for(DWORD counter(0); counter != numAttributes;)
		{
			delete attributeBuffer[counter];
			counter += 1;
		}
		
		for(DWORD counter(0); counter != numAdjacency;)
		{
			delete adjacencyInfo[counter];
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
		openObjectIndices(object[numFactObjects], file);
		openObjectAttributes(object[numFactObjects], file);
		openObjectAdjacency(object[numFactObjects], file);
		openObjectTriangles(object[numFactObjects], file);
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

