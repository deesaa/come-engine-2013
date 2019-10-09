struct particle
{
	D3DXVECTOR3  position;
	D3DCOLOR     color;
	float        size;
	static const DWORD FVF;
};
const DWORD particle::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE;



class staticParticles_class
{
private:
	IDirect3DDevice9* device;
	IDirect3DVertexBuffer9* vb;
	DWORD vbSize;

public:
	void initParticleClass(IDirect3DDevice9* bDevice, DWORD bVbSize)
	{
		device = bDevice;
		vbSize = bVbSize;

		device->CreateVertexBuffer(
			vbSize * sizeof(particle),
			D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
			particle::FVF,
			D3DPOOL_DEFAULT,
			&vb,
			0);

	}
};