#include "SkyboxClass.h"
#include <d3dcompiler.h>
#include "DDSTextureLoader.h"

using namespace DirectX;

SkyboxClass::SkyboxClass() :
	sphereIndexBuffer(nullptr), sphereVertBuffer(nullptr),
	SKYMAP_VS(nullptr), SKYMAP_PS(nullptr),
	SKYMAP_VS_Buffer(nullptr), SKYMAP_PS_Buffer(nullptr),
	smrv(nullptr), DSLessEqual(nullptr), RSCullNone(nullptr), cbPerObjectBuffer(nullptr),
	NumSphereVertices(0), NumSphereFaces(0)
{
}

SkyboxClass::~SkyboxClass()
{
	Shutdown();
}

bool SkyboxClass::Initialize(ID3D11Device* device, const wchar_t* cubeMapFile)
{
	CreateSphere(device, 10, 10);

	D3DCompileFromFile(L"./data/shaders.hlsl", 0, 0, "SKYMAP_VS", "vs_5_0", 0, 0, &SKYMAP_VS_Buffer, 0);
	D3DCompileFromFile(L"./data/shaders.hlsl", 0, 0, "SKYMAP_PS", "ps_5_0", 0, 0, &SKYMAP_PS_Buffer, 0);

	device->CreateVertexShader(SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), NULL, &SKYMAP_VS);
	device->CreatePixelShader(SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), NULL, &SKYMAP_PS);

	ID3D11Texture2D* SMTexture = nullptr;
	CreateDDSTextureFromFileEx(device, cubeMapFile, 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_SHADER_RESOURCE,
		0, D3D11_RESOURCE_MISC_TEXTURECUBE, false, (ID3D11Resource**)&SMTexture, nullptr);

	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;
	device->CreateShaderResourceView(SMTexture, &SMViewDesc, &smrv);
	SMTexture->Release();

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&dssDesc, &DSLessEqual);

	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_NONE;
	device->CreateRasterizerState(&cmdesc, &RSCullNone);

	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

	return true;
}

void SkyboxClass::Render(ID3D11DeviceContext* context, const XMMATRIX& view, const XMMATRIX& proj, const XMFLOAT3& camPosition)
{
	XMMATRIX Scale = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	XMMATRIX Translation = XMMatrixTranslation(camPosition.x, camPosition.y, camPosition.z);
	sphereWorld = Scale * Translation;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &sphereVertBuffer, &stride, &offset);
	context->IASetIndexBuffer(sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	WVP = sphereWorld * view * proj;
	cbPerObject cbPerObj;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	cbPerObj.World = XMMatrixTranspose(sphereWorld);

	context->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);

	context->VSSetShader(SKYMAP_VS, 0, 0);
	context->PSSetShader(SKYMAP_PS, 0, 0);
	context->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	context->PSSetShaderResources(0, 1, &smrv);
	context->OMSetDepthStencilState(DSLessEqual, 0);
	context->RSSetState(RSCullNone);

	context->DrawIndexed(NumSphereFaces * 3, 0, 0);

	context->OMSetDepthStencilState(NULL, 0);
}

void SkyboxClass::Shutdown()
{
	if (sphereVertBuffer) sphereVertBuffer->Release();
	if (sphereIndexBuffer) sphereIndexBuffer->Release();
	if (SKYMAP_VS) SKYMAP_VS->Release();
	if (SKYMAP_PS) SKYMAP_PS->Release();
	if (SKYMAP_VS_Buffer) SKYMAP_VS_Buffer->Release();
	if (SKYMAP_PS_Buffer) SKYMAP_PS_Buffer->Release();
	if (smrv) smrv->Release();
	if (DSLessEqual) DSLessEqual->Release();
	if (RSCullNone) RSCullNone->Release();
	if (cbPerObjectBuffer) cbPerObjectBuffer->Release();
}

void SkyboxClass::CreateSphere(ID3D11Device* device, int LatLines, int LongLines)
{
	NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
	NumSphereFaces = ((LatLines - 3) * (LongLines) * 2) + (LongLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<Vertex> vertices(NumSphereVertices);

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;

	for (DWORD i = 0; i < DWORD(LatLines - 2); ++i)
	{
		spherePitch = (float)(i + 1) * (3.14f / (float)(LatLines - 1));
		Rotationx = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < DWORD(LongLines); ++j)
		{
			sphereYaw = (float)j * (6.28f / (float)(LongLines));
			Rotationy = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
			currVertPos = XMVector3Normalize(currVertPos);
			vertices[i * LongLines + j + 1].pos.x = XMVectorGetX(currVertPos);
			vertices[i * LongLines + j + 1].pos.y = XMVectorGetY(currVertPos);
			vertices[i * LongLines + j + 1].pos.z = XMVectorGetZ(currVertPos);
		}
	}

	vertices[NumSphereVertices - 1].pos.x = 0.0f;
	vertices[NumSphereVertices - 1].pos.y = 0.0f;
	vertices[NumSphereVertices - 1].pos.z = -1.0f;


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * NumSphereVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];
	device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);


	std::vector<DWORD> indices(NumSphereFaces * 3);

	int k = 0;
	for (DWORD l = 0; l < DWORD(LongLines - 1); ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = LongLines;
	indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < DWORD(LatLines - 3); ++i)
	{
		for (DWORD j = 0; j < DWORD(LongLines - 1); ++j)
		{
			indices[k] = i * LongLines + j + 1;
			indices[k + 1] = i * LongLines + j + 2;
			indices[k + 2] = (i + 1) * LongLines + j + 1;

			indices[k + 3] = (i + 1) * LongLines + j + 1;
			indices[k + 4] = i * LongLines + j + 2;
			indices[k + 5] = (i + 1) * LongLines + j + 2;

			k += 6;
		}

		indices[k] = (i * LongLines) + LongLines;
		indices[k + 1] = (i * LongLines) + 1;
		indices[k + 2] = ((i + 1) * LongLines) + LongLines;

		indices[k + 3] = ((i + 1) * LongLines) + LongLines;
		indices[k + 4] = (i * LongLines) + 1;
		indices[k + 5] = ((i + 1) * LongLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < (DWORD)(LongLines - 1); ++l)
	{
		indices[k] = NumSphereVertices - 1;
		indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
		indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = NumSphereVertices - 1;
	indices[k + 1] = (NumSphereVertices - 1) - LongLines;
	indices[k + 2] = NumSphereVertices - 2;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	device->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);
}