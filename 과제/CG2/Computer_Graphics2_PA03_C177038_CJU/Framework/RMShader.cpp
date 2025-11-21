#include "pch.h"
#include "RMShader.h"


HRESULT RMShader::Init(ID3D11Device * device)
{
	D3D11_BUFFER_DESC matrixBufferDesc;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;


	matrixBufferDesc.ByteWidth = sizeof(CameraBuffer);
	if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &_cameraBuffer))) {
	//	AfxMessageBox(L"Shader Manager Error");
		return E_FAIL;
	}

	matrixBufferDesc.ByteWidth = sizeof(TransBuffer);
	if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &_transBuffer))) {
	//	AfxMessageBox(L"Shader Manager Error");

		return E_FAIL;
	}

	matrixBufferDesc.ByteWidth = sizeof(ColorBuffer);
	if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &_colorBuffer))) {
	//	MessageBox(L"Shader Manager Error");

		return E_FAIL;
	}

	matrixBufferDesc.ByteWidth = sizeof(BoneBuffer);
	if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &_boneBuffer))) {
	//	MessageBox(_hWnd, L"Shader Manager Error");

		return E_FAIL;
	}

	matrixBufferDesc.ByteWidth = sizeof(LightBuffer);
	if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &_lightBuffer)))
		return E_FAIL;

	// ===== LightPositionBuffer (b5) =====
	matrixBufferDesc.ByteWidth = sizeof(LightPositionBuffer);
	if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &_lightPosBuffer)))
		return E_FAIL;

	// ===== LightColorBuffer (b6) =====
	matrixBufferDesc.ByteWidth = sizeof(LightColorBuffer);
	if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &_lightColorBuffer)))
		return E_FAIL;

	return S_OK;
}

bool RMShader::SetShaderParameters(ID3D11DeviceContext * dc, vector<XMMATRIX>& matBoneList)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;

	//셰이더와 연결된 매트릭스 버퍼 주소를 가져온다.
	auto matBuff = _boneBuffer;

	//버퍼 정보에 접근한다.
	if (FAILED(dc->Map(matBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	//버퍼의 행렬 데이터 위치를 가져온다.
	BoneBuffer* dataPtr = (BoneBuffer*)mappedResource.pData;

	//행렬 정보 넘기기
	for(UINT i = 0; i< matBoneList.size(); i++)
		dataPtr->matBones[i] = matBoneList[i];

	dc->Unmap(matBuff, 0);

	//bufferSturct.h 의 정보와 일치해야 한다.
	bufferNumber = 3;

	dc->VSSetConstantBuffers(bufferNumber, 1, &matBuff);



	return true;
}

bool RMShader::SetShaderParameters(ID3D11DeviceContext * dc, XMMATRIX matWorld)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;

	//셰이더와 연결된 매트릭스 버퍼 주소를 가져온다.
	auto matBuff = _transBuffer;

	//버퍼 정보에 접근한다.
	if (FAILED(dc->Map(matBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	//버퍼의 행렬 데이터 위치를 가져온다.
	TransBuffer* dataPtr = (TransBuffer*)mappedResource.pData;

	//행렬 정보 넘기기
	dataPtr->matWorld = XMMatrixTranspose(matWorld);
	
	dc->Unmap(matBuff, 0);
	
	//bufferSturct.h 의 정보와 일치해야 한다.
	bufferNumber = 1;

	dc->VSSetConstantBuffers(bufferNumber, 1, &matBuff);

	

	return true;
}

bool RMShader::SetShaderParameters(ID3D11DeviceContext * dc, XMMATRIX matView, XMMATRIX matProj, XMFLOAT3 camPos)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber =0;

	//셰이더와 연결된 매트릭스 버퍼 주소를 가져온다.
	auto matBuff = _cameraBuffer;

	//버퍼 정보에 접근한다.
	if (FAILED(dc->Map(matBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	//버퍼의 행렬 데이터 위치를 가져온다.
	CameraBuffer* dataPtr = (CameraBuffer*)mappedResource.pData;

	//행렬 정보 넘기기
	dataPtr->matView = XMMatrixTranspose(matView);
	dataPtr->matProj = XMMatrixTranspose(matProj);
	dataPtr->cameraPosition = camPos;
	dataPtr->padding0 = 0.0f;

	dc->Unmap(matBuff, 0);

	dc->VSSetConstantBuffers(bufferNumber, 1, &matBuff);

	return false;
}

bool RMShader::SetShaderParameters(ID3D11DeviceContext * dc, XMFLOAT4 color)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;

	//셰이더와 연결된 매트릭스 버퍼 주소를 가져온다.
	auto buff = _colorBuffer;

	//버퍼 정보에 접근한다.
	if (FAILED(dc->Map(buff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	//버퍼의 행렬 데이터 위치를 가져온다.
	ColorBuffer* dataPtr = (ColorBuffer*)mappedResource.pData;

	//행렬 정보 넘기기
	dataPtr->fColor = color;
	dc->Unmap(buff, 0);

	bufferNumber = 2;

	dc->VSSetConstantBuffers(bufferNumber, 1, &buff);

	return true;
}

bool RMShader::SetLightDirectional(
	ID3D11DeviceContext* dc,
	const XMFLOAT4& ambient,
	const XMFLOAT4& diffuse,
	const XMFLOAT3& direction,
	float specPower,
	const XMFLOAT4& specColor)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (FAILED(dc->Map(_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	LightBuffer* dataPtr = (LightBuffer*)mappedResource.pData;

	dataPtr->ambientColor = ambient;
	dataPtr->diffuseColor = diffuse;
	dataPtr->lightDirection = direction;
	dataPtr->specularPower = specPower;
	dataPtr->specularColor = specColor;

	dc->Unmap(_lightBuffer, 0);

	// PS로 전달
	dc->PSSetConstantBuffers(4, 1, &_lightBuffer);

	return true;
}


bool RMShader::SetLightPositions(ID3D11DeviceContext* dc, const XMFLOAT4 pos[4])
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (FAILED(dc->Map(_lightPosBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	LightPositionBuffer* dataPtr = (LightPositionBuffer*)mappedResource.pData;

	for (int i = 0; i < 4; i++)
		dataPtr->lightPosition[i] = pos[i];

	dc->Unmap(_lightPosBuffer, 0);

	dc->PSSetConstantBuffers(5, 1, &_lightPosBuffer);

	return true;
}

bool RMShader::SetLightColors(
	ID3D11DeviceContext* dc,
	const XMFLOAT4 diffuse[4],
	const XMFLOAT4 spec[4])
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (FAILED(dc->Map(_lightColorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	LightColorBuffer* dataPtr = (LightColorBuffer*)mappedResource.pData;

	for (int i = 0; i < 4; i++)
	{
		dataPtr->diffuseColorPoint[i] = diffuse[i];
		dataPtr->specularColorPoint[i] = spec[i];
	}

	dc->Unmap(_lightColorBuffer, 0);

	dc->PSSetConstantBuffers(6, 1, &_lightColorBuffer);

	return true;
}

Shader* RMShader::loadResource(wstring fileName, void * param)
{
	//셰이더 추가.
	
	wstring vsFilename = fileName + L".hlsl";
//	wstring psFilename = fileName + L".ps";


	//없는 경우
	Shader* shader = new Shader;
	if (!shader->InsertShader(DEVICEMANAGER.GetDevice(), _hWnd, (WCHAR*)vsFilename.c_str())) {
		SAFE_DELETE(shader);
	}
	
	return shader;
}


void RMShader::releaseResource(Shader* data)
{
	SAFE_DELETE(data);
}

