#pragma once

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class D3DClass;
class CameraClass;
class MultiTextureModelClass;
class BumpMapShaderClass;
class LightClass;
class SkyboxClass;
class LightShaderClass;
class ModelClass;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	bool Render();

	CameraClass* GetCamera()
	{
		return m_Camera;
	}

private:
	D3DClass* m_Direct3D = nullptr;
	CameraClass* m_Camera = nullptr;
	MultiTextureModelClass* m_MultiModel = nullptr;
	BumpMapShaderClass* m_BumpMapShader = nullptr;
	LightClass* m_Light = nullptr;

	LightShaderClass* m_LightShader;
	LightClass* m_Light1, * m_Light2, * m_Light3, * m_Light4;
	ModelClass* m_Model = nullptr;

	SkyboxClass* m_Skybox = nullptr;

};