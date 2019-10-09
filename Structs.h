enum triangleType{Full, Vert};

struct sphere_struct
{
	D3DXVECTOR3 center;
	float radius;
	DWORD vertexID;
	bool isPicked;
};

struct ray_struct
{
	D3DXVECTOR3 origin;
	D3DXVECTOR3 direction;
};

struct triangle
{
	triangleType triType;
	DWORD attributeID;
	DWORD verticesID[3];
	DWORD indicesID[3];
};

struct pickedVertex
{
	DWORD vertexID;
	bool isPicked;
	pickedVertex()
	{
		vertexID = 0;
		isPicked = FALSE;
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