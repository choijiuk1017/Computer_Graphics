//=============================================================
//	## RMShader ## (로드된 셰이더 정보를 관리한다.)
//=============================================================
#pragma once
#include "ResourceManager.h"
#include "Shader.h"

struct CameraBuffer
{
	XMMATRIX matView;
	XMMATRIX matProj;
	XMFLOAT3 cameraPosition;
	float padding0;
};

struct TransBuffer
{
	XMMATRIX matWorld;
};

struct ColorBuffer
{
	XMFLOAT4 fColor;
};

struct BoneBuffer
{
	XMMATRIX matBones[128];
};

struct LightBuffer
{
	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
	XMFLOAT3 lightDirection;
	float specularPower;
	XMFLOAT4 specularColor;
};

struct LightPositionBuffer
{
	XMFLOAT4 lightPosition[4];
};

struct LightColorBuffer
{
	XMFLOAT4 diffuseColorPoint[4];
	XMFLOAT4 specularColorPoint[4];
};

class RMShader : public ResourceManager<Shader*, RMShader>
{
	
public:
	RMShader() {};
	~RMShader() {};

	HRESULT Init(ID3D11Device* device);

	bool SetShaderParameters(ID3D11DeviceContext * dc, vector<XMMATRIX>& matBoneList);
	bool SetShaderParameters(ID3D11DeviceContext * dc, XMMATRIX matWorld);
	bool SetShaderParameters(ID3D11DeviceContext * dc, XMMATRIX matView, XMMATRIX matProj, XMFLOAT3 camPos);
	bool SetShaderParameters(ID3D11DeviceContext * dc, XMFLOAT4 color);

	bool SetLightDirectional(
		ID3D11DeviceContext* dc,
		const XMFLOAT4& ambient,
		const XMFLOAT4& diffuse,
		const XMFLOAT3& direction,
		float specPower,
		const XMFLOAT4& specColor);

	bool SetLightPositions(ID3D11DeviceContext* dc, const XMFLOAT4 pos[4]);

	bool SetLightColors(
		ID3D11DeviceContext* dc,
		const XMFLOAT4 diffuse[4],
		const XMFLOAT4 spec[4]);

protected:
	Shader* loadResource(wstring fileName, void* param=nullptr);
	void releaseResource(Shader* data);
private:
	ID3D11Buffer*			_transBuffer	= nullptr;
	ID3D11Buffer*			_cameraBuffer	= nullptr;
	ID3D11Buffer*			_colorBuffer	= nullptr;
	ID3D11Buffer*			_boneBuffer		= nullptr;

	ID3D11Buffer* _lightBuffer = nullptr;       
	ID3D11Buffer* _lightPosBuffer = nullptr;       
	ID3D11Buffer* _lightColorBuffer = nullptr;
	
};
