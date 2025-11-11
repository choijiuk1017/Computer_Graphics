#pragma once

#include "CameraClass.h"

class InputClass;
class GraphicsClass;
class SoundClass;

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input = nullptr;
	GraphicsClass* m_Graphics = nullptr;

	SoundClass* m_Sound = nullptr;

	int currentAmbientMode = 1;
	int currentDiffuseMode = 1;
	int currentSpecularMode = 1;

	bool prevAmbientKeyDown = false;
	bool prevDiffuseKeyDown = false;


	bool prevKeyDown = false;

};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle = 0;