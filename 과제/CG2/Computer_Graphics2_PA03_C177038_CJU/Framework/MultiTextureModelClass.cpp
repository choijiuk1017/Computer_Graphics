#include "stdafx.h"
#include "TextureArrayClass.h"
#include "MultiTextureModelClass.h"

#include <fstream>
#include <sstream>   
#include <string> 
#include <vector>  
using namespace std;


MultiTextureModelClass::MultiTextureModelClass()
{
}


MultiTextureModelClass::MultiTextureModelClass(const  MultiTextureModelClass& other)
{
}


MultiTextureModelClass::~MultiTextureModelClass()
{
}


bool  MultiTextureModelClass::Initialize(ID3D11Device* device, const WCHAR* modelFilename, const WCHAR* textureFilename1, const WCHAR* textureFilename2, const WCHAR* textureFilename3, const WCHAR* textureFilename4)
{
	// 모델 데이터를 로드합니다.
	if (!LoadModel(modelFilename))
	{
		return false;
	}

	// 모델의 법선, 접선 및 이항 벡터를 계산합니다.
	CalculateModelVectors();

	// 정점 및 인덱스 버퍼를 초기화합니다.
	if (!InitializeBuffers(device))
	{
		return false;
	}

	// 이 모델의 텍스처를 로드합니다.
	return LoadTextures(device, textureFilename1, textureFilename2, textureFilename3, textureFilename4);
}


void  MultiTextureModelClass::Shutdown()
{
	// 모델 텍스쳐를 반환합니다.
	ReleaseTextures();

	// 버텍스 및 인덱스 버퍼를 종료합니다.
	ShutdownBuffers();

	// 모델 데이터 반환
	ReleaseModel();
}


void  MultiTextureModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// 그리기를 준비하기 위해 그래픽 파이프 라인에 꼭지점과 인덱스 버퍼를 놓습니다.
	RenderBuffers(deviceContext);
}


int  MultiTextureModelClass::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView** MultiTextureModelClass::GetTextureArray()
{
	return m_TextureArray->GetTextureArray();
}


bool  MultiTextureModelClass::InitializeBuffers(ID3D11Device* device)
{
	// 정점 배열을 만듭니다.
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// 인덱스 배열을 만듭니다.
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// 정점 배열과 인덱스 배열을 데이터로 읽어옵니다.
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		vertices[i].tangent = XMFLOAT3(m_model[i].tx, m_model[i].ty, m_model[i].tz);
		vertices[i].binormal = XMFLOAT3(m_model[i].bx, m_model[i].by, m_model[i].bz);

		indices[i] = i;
	}

	// 정적 정점 버퍼의 구조체를 설정합니다.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 이제 정점 버퍼를 만듭니다.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// 정적 인덱스 버퍼의 구조체를 설정합니다.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성합니다.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성합니다.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// 생성되고 값이 할당된 정점 버퍼와 인덱스 버퍼를 해제합니다.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void  MultiTextureModelClass::ShutdownBuffers()
{
	// 인덱스 버퍼를 해제합니다.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// 정점 버퍼를 해제합니다.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}


void  MultiTextureModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// 정점 버퍼의 단위와 오프셋을 설정합니다.
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	// 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정합니다.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// 렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정합니다.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 정점 버퍼로 그릴 기본형을 설정합니다. 여기서는 삼각형으로 설정합니다.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool  MultiTextureModelClass::LoadTextures(ID3D11Device* device, const  WCHAR* filename1, const WCHAR* filename2, const  WCHAR* filename3, const WCHAR* filename4)
{
	// 텍스처 배열 오브젝트를 생성한다.
	m_TextureArray = new TextureArrayClass;
	if (!m_TextureArray)
	{
		return false;
	}

	// 텍스처 배열 오브젝트를 초기화한다.
	return m_TextureArray->Initialize(device, filename1, filename2, filename3, filename4);
}


void  MultiTextureModelClass::ReleaseTextures()
{
	// 텍스처 배열 오브젝트를 릴리즈한다.
	if (m_TextureArray)
	{
		m_TextureArray->Shutdown();
		delete m_TextureArray;
		m_TextureArray = 0;
	}
}

bool  MultiTextureModelClass::LoadModel(const WCHAR* filename)
{
	wstring fname(filename);
	bool isObj = fname.find(L".obj") != wstring::npos;
	bool isTxt = fname.find(L".txt") != wstring::npos;

	ifstream fin;
	fin.open(filename);
	if (fin.fail())
		return false;

	if (isObj)
	{
		// ====== OBJ 로더 ======
		vector<XMFLOAT3> vertices;
		vector<XMFLOAT2> texcoords;
		vector<XMFLOAT3> normals;
		struct FaceIndex { int v, t, n; };
		vector<FaceIndex> faces;

		string line;
		while (getline(fin, line))
		{
			if (line.substr(0, 2) == "v ")
			{
				istringstream ss(line.substr(2));
				XMFLOAT3 v;
				ss >> v.x >> v.y >> v.z;
				vertices.push_back(v);
			}
			else if (line.substr(0, 3) == "vt ")
			{
				istringstream ss(line.substr(3));
				XMFLOAT2 t;
				ss >> t.x >> t.y;
				t.y = 1.0f - t.y;
				texcoords.push_back(t);
			}
			else if (line.substr(0, 3) == "vn ")
			{
				istringstream ss(line.substr(3));
				XMFLOAT3 n;
				ss >> n.x >> n.y >> n.z;
				normals.push_back(n);
			}
			else if (line.substr(0, 2) == "f ")
			{
				istringstream ss(line.substr(2));
				string token;
				for (int i = 0; i < 3; ++i)
				{
					ss >> token;
					size_t p1 = token.find('/');
					size_t p2 = token.find('/', p1 + 1);

					FaceIndex f;
					f.v = stoi(token.substr(0, p1)) - 1;
					f.t = stoi(token.substr(p1 + 1, p2 - p1 - 1)) - 1;
					f.n = stoi(token.substr(p2 + 1)) - 1;
					faces.push_back(f);
				}
			}
		}

		fin.close();

		m_vertexCount = static_cast<int>(faces.size());
		m_indexCount = m_vertexCount;
		m_model = new ModelType[m_vertexCount];

		for (int i = 0; i < m_vertexCount; ++i)
		{
			const FaceIndex& f = faces[i];
			m_model[i].x = vertices[f.v].x;
			m_model[i].y = vertices[f.v].y;
			m_model[i].z = vertices[f.v].z;
			m_model[i].tu = texcoords[f.t].x;
			m_model[i].tv = texcoords[f.t].y;
			m_model[i].nx = normals[f.n].x;
			m_model[i].ny = normals[f.n].y;
			m_model[i].nz = normals[f.n].z;
			m_model[i].tx = m_model[i].ty = m_model[i].tz = 0.0f;
			m_model[i].bx = m_model[i].by = m_model[i].bz = 0.0f;
		}

		return true;
	}
	else if (isTxt)
	{
		// ====== TXT 로더 ======
		char input;
		int i;

		// 첫 문자 'Vertex Count:' 찾아서 정점 개수 읽기
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}
		fin >> m_vertexCount;

		m_indexCount = m_vertexCount;
		m_model = new ModelType[m_vertexCount];
		if (!m_model)
			return false;

		// "Data:" 위치로 이동
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}
		fin.get(input); // 줄바꿈 처리

		// 정점 데이터 읽기
		for (i = 0; i < m_vertexCount; i++)
		{
			fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
			fin >> m_model[i].tu >> m_model[i].tv;
			fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
			// 나머지 벡터 0 초기화
			m_model[i].tx = m_model[i].ty = m_model[i].tz = 0.0f;
			m_model[i].bx = m_model[i].by = m_model[i].bz = 0.0f;
		}

		fin.close();
		return true;
	}

	// 확장자가 둘 다 아닐 때
	fin.close();
	return false;
}


void  MultiTextureModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}
}


void  MultiTextureModelClass::CalculateModelVectors()
{
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal, normal;


	// 모델의 면 수를 계산합니다.
	int faceCount = m_vertexCount / 3;

	// 모델 데이터에 대한 인덱스를 초기화합니다.
	int index = 0;

	// 모든면을 살펴보고 접선, 비공식 및 법선 벡터를 계산합니다.
	for(int i=0; i<faceCount; i++)
	{
		// 모델에서이면에 대한 세 개의 정점을 가져옵니다.
		vertex1.x = m_model[index].x;
		vertex1.y = m_model[index].y;
		vertex1.z = m_model[index].z;
		vertex1.tu = m_model[index].tu;
		vertex1.tv = m_model[index].tv;
		vertex1.nx = m_model[index].nx;
		vertex1.ny = m_model[index].ny;
		vertex1.nz = m_model[index].nz;
		index++;

		vertex2.x = m_model[index].x;
		vertex2.y = m_model[index].y;
		vertex2.z = m_model[index].z;
		vertex2.tu = m_model[index].tu;
		vertex2.tv = m_model[index].tv;
		vertex2.nx = m_model[index].nx;
		vertex2.ny = m_model[index].ny;
		vertex2.nz = m_model[index].nz;
		index++;

		vertex3.x = m_model[index].x;
		vertex3.y = m_model[index].y;
		vertex3.z = m_model[index].z;
		vertex3.tu = m_model[index].tu;
		vertex3.tv = m_model[index].tv;
		vertex3.nx = m_model[index].nx;
		vertex3.ny = m_model[index].ny;
		vertex3.nz = m_model[index].nz;
		index++;

		// 표면의 탄젠트와 바이 노멀을 계산합니다.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// 접선과 binormal을 사용하여 새 법선을 계산합니다.
		CalculateNormal(tangent, binormal, normal);

		// 모델 구조에서 면의 법선, 접선 및 바이 노멀을 저장합니다.
		m_model[index-1].nx = normal.x;
		m_model[index-1].ny = normal.y;
		m_model[index-1].nz = normal.z;
		m_model[index-1].tx = tangent.x;
		m_model[index-1].ty = tangent.y;
		m_model[index-1].tz = tangent.z;
		m_model[index-1].bx = binormal.x;
		m_model[index-1].by = binormal.y;
		m_model[index-1].bz = binormal.z;

		m_model[index-2].nx = normal.x;
		m_model[index-2].ny = normal.y;
		m_model[index-2].nz = normal.z;
		m_model[index-2].tx = tangent.x;
		m_model[index-2].ty = tangent.y;
		m_model[index-2].tz = tangent.z;
		m_model[index-2].bx = binormal.x;
		m_model[index-2].by = binormal.y;
		m_model[index-2].bz = binormal.z;

		m_model[index-3].nx = normal.x;
		m_model[index-3].ny = normal.y;
		m_model[index-3].nz = normal.z;
		m_model[index-3].tx = tangent.x;
		m_model[index-3].ty = tangent.y;
		m_model[index-3].tz = tangent.z;
		m_model[index-3].bx = binormal.x;
		m_model[index-3].by = binormal.y;
		m_model[index-3].bz = binormal.z;
	}
}


void  MultiTextureModelClass::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3,
										  VectorType& tangent, VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];


	// 현재 표면의 두 벡터를 계산합니다.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// tu 및 tv 텍스처 공간 벡터를 계산합니다.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// 탄젠트 / 바이 노멀 방정식의 분모를 계산합니다.
	float den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// 교차 곱을 계산하고 계수로 곱하여 접선과 비 구식을 얻습니다.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// 이 법선의 길이를 계산합니다.
	float length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));
			
	// 법선을 표준화 한 다음 저장합니다.
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// 이 법선의 길이를 계산합니다.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));
			
	// 법선을 표준화 한 다음 저장합니다.
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;
}


void  MultiTextureModelClass::CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal)
{
	// 법선 벡터를 줄 수있는 접선과 binormal의 외적을 계산합니다.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// 법선의 길이를 계산합니다.
	float length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// 법선을 표준화합니다.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;
}