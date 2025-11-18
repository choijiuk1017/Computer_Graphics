#include "pch.h"
#include "RMTexture.h"
#include "DDSTextureLoader.h"

//Load Texture
ID3D11ShaderResourceView* RMTexture::LoadTexture(std::wstring fileName)
{
	ID3D11ShaderResourceView* textureView = nullptr;

	DirectX::CreateDDSTextureFromFile(
		DEVICEMANAGER.GetDevice(),
		fileName.c_str(),
		nullptr,
		&textureView
	);

	return textureView;
}
//Load Resource
Texture * RMTexture::loadResource(wstring fileName, void * param)
{
	Texture* tex = new Texture;
	tex->name = MYUTIL::getFileName(fileName.c_str());
	tex->data = LoadTexture(fileName);

	
	if (!tex->data) {
		SAFE_DELETE(tex);
	}

	return tex;
}

//Delete Texture
void RMTexture::releaseResource(Texture * data)
{
	SAFE_DELETE(data);
}
