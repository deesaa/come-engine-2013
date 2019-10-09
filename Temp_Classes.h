class tempObject_class
{
public:
	vertex* vertices[512];
	sphere_struct* vertexSphere[512];
	WORD* indices[1024];
	DWORD* attributeBuffer[1024];
	triangle* triangles[512];

	DWORD* adjacencyInfo[1024];
	D3DMATERIAL9* materials[512];

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

	void loadVertex(std::vector<float> bVertex, DWORD vertType, std::vector<DWORD> linkedVerts)
	{
		vertices[numVerts] = new vertex;
		vertexSphere[numVerts] = new sphere_struct; 
		vertices[numVerts]->pos.x		 = bVertex.at(0);
		vertices[numVerts]->pos.y		 = bVertex.at(1);
		vertices[numVerts]->pos.z		 = bVertex.at(2);
		vertices[numVerts]->nX			 = bVertex.at(3);
		vertices[numVerts]->nY			 = bVertex.at(4);
		vertices[numVerts]->nZ			 = bVertex.at(5);

		vertexSphere[numVerts]->vertType = (vertexType)vertType;
		vertexSphere[numVerts]->numLinkedVerts = linkedVerts.size();

		for(DWORD counter(0); counter != linkedVerts.size();)
		{
			vertexSphere[numVerts]->linkedVertices[counter] = linkedVerts.at(counter);
			counter += 1;
		}

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

	void loadMaterial(std::vector<float> Ambient, std::vector<float> Diffuse, std::vector<float> Emissive, 
		std::vector<float> Specular, float Power)
	{
		materials[numMaterials] = new D3DMATERIAL9;
		materials[numMaterials]->Ambient.r = Ambient.at(0);
		materials[numMaterials]->Ambient.g = Ambient.at(1);
		materials[numMaterials]->Ambient.b = Ambient.at(2);
		materials[numMaterials]->Ambient.a = Ambient.at(3);

		materials[numMaterials]->Diffuse.r = Diffuse.at(0);
		materials[numMaterials]->Diffuse.g = Diffuse.at(1);
		materials[numMaterials]->Diffuse.b = Diffuse.at(2);
		materials[numMaterials]->Diffuse.a = Diffuse.at(3);

		materials[numMaterials]->Emissive.r = Emissive.at(0);
		materials[numMaterials]->Emissive.g = Emissive.at(1);
		materials[numMaterials]->Emissive.b = Emissive.at(2);
		materials[numMaterials]->Emissive.a = Emissive.at(3);

		materials[numMaterials]->Specular.r = Specular.at(0);
		materials[numMaterials]->Specular.g = Specular.at(1);
		materials[numMaterials]->Specular.b = Specular.at(2);
		materials[numMaterials]->Specular.a = Specular.at(3);

		materials[numMaterials]->Power = Power;
		numMaterials += 1;
	}

	~tempObject_class()
	{
		for(DWORD counter(0); counter != numVerts;)
		{
			delete vertices[counter];
			delete vertexSphere[counter];
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
		for(DWORD counter(0); counter != numMaterials;)
		{
			delete materials[counter];
			counter += 1;
		}
	}
};

class tempLight_class
{
public:
	D3DLIGHT9 light;
	tempLight_class(){}
	void loadLight(DWORD lightType, std::vector<float> Diffuse, std::vector<float> Ambient, std::vector<float> Specular,
		float Phi, float Theta, float Range, float Falloff, std::vector<float> Attenuation)
	{
		light.Type		 = (D3DLIGHTTYPE)lightType;
		light.Diffuse.r	 = Diffuse.at(0);
		light.Diffuse.g	 = Diffuse.at(1);
		light.Diffuse.b	 = Diffuse.at(2);
		light.Diffuse.a	 = Diffuse.at(3);
		light.Ambient.r  = Ambient.at(0);
		light.Ambient.g  = Ambient.at(1);
		light.Ambient.b  = Ambient.at(2);
		light.Ambient.a  = Ambient.at(3);
		light.Specular.r = Specular.at(0);
		light.Specular.g = Specular.at(1);
		light.Specular.b = Specular.at(2);
		light.Specular.a = Specular.at(3);
		light.Phi = Phi;
		light.Theta = Theta;
		light.Range = Range;
		light.Falloff = Falloff;
		light.Attenuation0 = Attenuation.at(0);
		light.Attenuation1 = Attenuation.at(1);
		light.Attenuation2 = Attenuation.at(2);

		light.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		light.Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	tempLight_class*  light[128];				//Массив сылок на объекты света
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
		//	MessageBox(NULL, L"Can't open Vertices", NULL, MB_OK);
		openObjectIndices(object[numFactObjects], file);
		//	MessageBox(NULL, L"Can't open Indices", NULL, MB_OK);
		openObjectAttributes(object[numFactObjects], file);
		//	MessageBox(NULL, L"Can't open Attributes", NULL, MB_OK);
		openObjectAdjacency(object[numFactObjects], file);
		//	MessageBox(NULL, L"Can't open Adjacency", NULL, MB_OK);
		openObjectTriangles(object[numFactObjects], file);
		//	MessageBox(NULL, L"Can't open Triangles", NULL, MB_OK);
		openObjectMaterials(object[numFactObjects], file);
		//	MessageBox(NULL, L"Can't open Materials", NULL, MB_OK);
		numFactObjects += 1;
		numFactGlobal  += 1;
	}

	void addLight(std::string* file)
	{
		light[numFactLight] = new tempLight_class;
		openLight(light[numFactLight], file);

		numFactLight += 1;
		numFactGlobal  += 1;
	}

	~temp_manager()
	{
		for(;numFactObjects != NULL;)
		{
			numFactObjects--;
			delete object[numFactObjects];
		}
		for(;numFactLight != NULL;)
		{
			numFactLight--;
			delete light[numFactLight];
		}
	}
};

