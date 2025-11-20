#pragma once

/////////////
// GLOBALS //
/////////////
#include <Vector>
#include "RMModel.h"
#include "ModelLoader.h"


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class D3DClass;
class CameraClass;
class MultiTextureModelClass;
class ModelClass;
class FBXModelClass;
class BumpMapShaderClass;
class LightClass;
class SkyboxClass;
class LightShaderClass;
class BitmapClass;
class TextureShaderClass;
class TextClass;


class GraphicsClass
{

public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int);
	bool Render();

	CameraClass* GetCamera()
	{
		return m_Camera;
	}


	void SetAmbient(int);
	void SetDiffuse(int);
	void SetSpecular(int);

	void IncreaseIntensity();
	void DecreaseIntensity();

	bool m_ShowTitle = true;

	void SetAniNum(int);

	void ShootBullet();


private:
	void CalculateAABB(
		const std::vector<XMFLOAT3>& localBox,
		const XMMATRIX& world,
		XMVECTOR& outMin,
		XMVECTOR& outMax)
	{
		XMFLOAT3 minV(FLT_MAX, FLT_MAX, FLT_MAX);
		XMFLOAT3 maxV(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		for (int i = 0; i < 8; ++i)
		{
			XMVECTOR v = XMVectorSet(localBox[i].x, localBox[i].y, localBox[i].z, 1.0f);
			v = XMVector3TransformCoord(v, world);

			minV.x = std::min(minV.x, XMVectorGetX(v));
			minV.y = std::min(minV.y, XMVectorGetY(v));
			minV.z = std::min(minV.z, XMVectorGetZ(v));

			maxV.x = std::max(maxV.x, XMVectorGetX(v));
			maxV.y = std::max(maxV.y, XMVectorGetY(v));
			maxV.z = std::max(maxV.z, XMVectorGetZ(v));
		}

		outMin = XMLoadFloat3(&minV);
		outMax = XMLoadFloat3(&maxV);
	}

	bool AABB_Intersect(XMVECTOR minA, XMVECTOR maxA, XMVECTOR minB, XMVECTOR maxB)
	{
		if (XMVectorGetX(maxA) < XMVectorGetX(minB) || XMVectorGetX(minA) > XMVectorGetX(maxB)) return false;
		if (XMVectorGetY(maxA) < XMVectorGetY(minB) || XMVectorGetY(minA) > XMVectorGetY(maxB)) return false;
		if (XMVectorGetZ(maxA) < XMVectorGetZ(minB) || XMVectorGetZ(minA) > XMVectorGetZ(maxB)) return false;

		return true;
	}

private:

	float m_AnimTime = 0.0f;
	float intensity;

	int animationNum = 1;


	D3DClass* m_Direct3D = nullptr;
	CameraClass* m_Camera = nullptr;
	MultiTextureModelClass* m_MultiModel = nullptr;
	BumpMapShaderClass* m_BumpMapShader = nullptr;
	LightClass* m_Light = nullptr;

	LightShaderClass* m_LightShader;
	LightClass* m_Light1, * m_Light2, * m_Light3, * m_Light4;

	XMFLOAT4 baseColor[4];

	SkyboxClass* m_Skybox = nullptr;

	std::vector<ModelClass*> m_Models;

	ModelClass* m_Bullet = nullptr;
	XMMATRIX m_BulletWorld = XMMatrixIdentity();
	XMFLOAT3 m_BulletPos;
	XMFLOAT3 m_BulletDir;

	bool m_bulletFlying = false;

	
	TextureShaderClass* m_TextureShader = nullptr;
	BitmapClass* m_Title = nullptr;

	std::vector<TextClass*> m_Texts;

	FBXModelClass* m_FBXModel = nullptr;

	ModelLoader* m_modelLoader;
	SkinModel* m_skinModel;
};