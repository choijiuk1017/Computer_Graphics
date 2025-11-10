#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
};


struct cbPerObject
{
	XMMATRIX WVP;
	XMMATRIX World;
};


class SkyboxClass
{
public:
	SkyboxClass();
	~SkyboxClass();

	bool Initialize(ID3D11Device* device, const wchar_t* cubeMapFile);
	void Render(ID3D11DeviceContext* context, const XMMATRIX& view, const XMMATRIX& proj, const XMFLOAT3& camPosition);
	void Shutdown();

private:
	void CreateSphere(ID3D11Device* device, int LatLines, int LongLines);

private:
	XMMATRIX Rotationx;
	XMMATRIX Rotationy;

	XMMATRIX WVP;

	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertBuffer;

	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;
	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;

	ID3D11ShaderResourceView* smrv;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;
	ID3D11Buffer* cbPerObjectBuffer;

	int NumSphereVertices;
	int NumSphereFaces;
	XMMATRIX sphereWorld;
};