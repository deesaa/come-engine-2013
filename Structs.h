enum triangleType{Full, Vert, Ind};
enum vertexType{Father, Child};

class sphere_struct
{
public:
	D3DXVECTOR3 center;
	float radius;
	DWORD vertexID;
	DWORD indexID;
	bool isPicked;
	DWORD startVertex;
	DWORD startIndex;
	DWORD numLinkedVerts;
	DWORD numLocatedTris;
	vertexType vertType;
	DWORD linkedVertices[12];
	DWORD locatedTriangles[12];

	sphere_struct(){}
	sphere_struct(D3DXVECTOR3 bCenter, DWORD bVertexID, DWORD bIndexID, float bRadius, bool bIsPicked, DWORD bStartVertex, 
		vertexType bVertType)
	{
		center			= bCenter;
		vertexID		= bVertexID;
		indexID		    = bIndexID;
		radius			= bRadius;
		isPicked		= bIsPicked;
		startVertex		= bStartVertex;
		vertType		= bVertType;
		numLinkedVerts	= 0;
	}

	void linkVert(DWORD bLinkedVert)
	{
		linkedVertices[numLinkedVerts] = bLinkedVert;
		numLinkedVerts += 1;
	}

	void locatedIn(DWORD bLocatedTri)
	{
		locatedTriangles[numLocatedTris] = bLocatedTri;
		numLocatedTris += 1;
	}

	void locatedIn(DWORD bLocatedTris[], DWORD numIDs)
	{
		for(DWORD counter(0); counter != numIDs;)
		{
			locatedTriangles[numLocatedTris] = bLocatedTris[counter];
			numLocatedTris += 1;
			counter += 1;
		}
	}

	~sphere_struct()
	{
		/*for(int counter(0); counter != numLinkedVerts;)
		{
			delete linkedVertices[counter];
			counter++;
		}*/
	}
};

struct ray_struct
{
	D3DXVECTOR3 origin;
	D3DXVECTOR3 direction;
};

struct triangle
{
	triangleType triType;
	DWORD subsetID;
	DWORD materialID;
	DWORD triangleID;
	DWORD verticesID[3];
	DWORD fathers[3];

	triangle(){}
	triangle(triangleType bTriType, DWORD firstVert, DWORD secondVert, DWORD thirdVert, DWORD bTriangleID, DWORD bSubsetID, 
		DWORD bMaterialID, DWORD firstFatherVert, DWORD secondFatherVert, DWORD thirdFatherVert)
	{
		triType		  = bTriType;
		verticesID[0] = firstVert;
		verticesID[1] = secondVert;
		verticesID[2] = thirdVert;
		subsetID	  = bSubsetID;
		materialID	  = bMaterialID;
		triangleID	  = bTriangleID;
		fathers[0]	  = firstFatherVert;
		fathers[1]	  = secondFatherVert;
		fathers[2]	  = thirdFatherVert;
	}
};

struct COVertex	
{
	D3DXVECTOR3 vert;
	static const DWORD FVF;

	COVertex(){}
	COVertex(float bX, float bY, float bZ)
	{
		vert.x = bX; vert.y = bY; vert.z = bZ;
	}
};
const DWORD COVertex::FVF = D3DFVF_XYZ;

struct vertex	
{
	D3DXVECTOR3 pos;
	float nX, nY, nZ;
	float u, v;
	static const DWORD FVF;

	vertex(){}
	vertex(float bX, float bY, float bZ, float bnX, float bnY, float bnZ, float bU, float bV)
	{
		pos = D3DXVECTOR3(bX, bY, bZ);
		nX = bnX; nY = bnY; nZ = bnZ;
		u = bU; v = bV;
	}
};
const DWORD vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

struct particle
{
	D3DXVECTOR3 pos;
	float size;
	static const DWORD FVF;

	particle(){}
	particle(D3DXVECTOR3 bPos, float bSize)
	{
		pos = bPos;
		size = bSize;
	}
};
const DWORD particle::FVF = D3DFVF_XYZ | D3DFVF_PSIZE;

DWORD flToDw(float f)
{
	return *((DWORD*)&f);
}
