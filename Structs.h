struct sphere_struct
{
	D3DXVECTOR3 center;
	float radius;
	DWORD vertexID;
};

struct ray_struct
{
	D3DXVECTOR3 origin;
	D3DXVECTOR3 direction;
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