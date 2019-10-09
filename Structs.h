enum triangleType{Full, Vert, Ind};

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

	sphere_struct(){}
	sphere_struct(D3DXVECTOR3 bCenter, DWORD bVertexID, DWORD bIndexID, float bRadius, bool bIsPicked, DWORD bStartVertex)
	{
		center		= bCenter;
		vertexID	= bVertexID;
		indexID	    = bIndexID;
		radius		= bRadius;
		isPicked	= bIsPicked;
		startVertex = bStartVertex;
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

	triangle(){}
	triangle(triangleType bTriType, DWORD firstVert, DWORD secondVert, DWORD thirdVert, DWORD bTriangleID, DWORD bSubsetID, DWORD bMaterialID)
	{
		triType		  = bTriType;
		verticesID[0] = firstVert;
		verticesID[1] = secondVert;
		verticesID[2] = thirdVert;
		subsetID	  = bSubsetID;
		materialID	  = bMaterialID;
		triangleID	  = bTriangleID;
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

DWORD flToDw(float f)
{
	return *((DWORD*)&f);
}
