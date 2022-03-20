#pragma once
#include <d3d11.h>
#include <wincodec.h>

class Game;

class TextureLoader
{
	IWICImagingFactory2* factory_ = nullptr;
	Game* game_ = nullptr;

public:
	TextureLoader(Game* inGame);
	~TextureLoader();

	HRESULT LoadTextureFromFile(LPCWSTR fileName, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& textureView, bool generateMips = false, bool useSrgb = false, UINT frameIndex = 0);
};
