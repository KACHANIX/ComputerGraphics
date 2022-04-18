#pragma once 
#include <chrono>
#include <d3d11.h> 
#include "GameComponent.h"
#include "DisplayWin32.h" 
#include "Windows.h"
#include <wrl.h> 
#include <d3d.h> 
#include "DirectXMath.h" 

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#include <d3d11_1.h> 
#include <vector>

#include "SimpleMath.h"
#include "TextureLoader.h"
class InputDevice;
class LightSource;

class Game
{
public:
	static Game* instance;
	LPCWSTR name;

	std::vector<GameComponent*> components;
	DirectX::SimpleMath::Vector2 screen_resized[2];
	DisplayWin32* display;
	InputDevice* input_device;

	ID3D11Device* device = nullptr;
	IDXGISwapChain* swap_chain = nullptr;
	ID3D11DeviceContext* context = nullptr;

	ID3D11Texture2D* back_buffer = nullptr;
	ID3D11Texture2D* depth_buffer = nullptr;

	ID3D11RenderTargetView* render_view = nullptr;
	ID3D11DepthStencilView* depth_view = nullptr;

	ID3D11ShaderResourceView* render_SRV = nullptr;

	ID3D11RasterizerState* rast_state;
	ID3D11DepthStencilState* depth_state;

	ID3DUserDefinedAnnotation* debug_annotation = nullptr;

	std::chrono::time_point<std::chrono::steady_clock>* start_time = nullptr;
	std::chrono::time_point<std::chrono::steady_clock>* prev_time = nullptr;
	float total_time;

	bool is_exit_requested = false;
	bool is_active = false;
	LightSource* light_source_ = nullptr;
	TextureLoader* texture_loader = nullptr;

	Game(LPCWSTR window_name);
	virtual ~Game();

	void Exit();
	void RestoreTargets();
	void RestoreTargets(ID3D11DepthStencilView* in_depth_view);
	void Run(int window_width, int window_height);
	virtual LRESULT MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

private:
	DXGI_SWAP_CHAIN_DESC swap_desc_ = {};

	void CreateBackBuffer();

protected:
	virtual void Initialize();
	virtual void Update(float delta_time);
	virtual void Draw(float delta_time);
	virtual void PostDraw(float delta_time);

	void DestroyResources();
	void PrepareResources();
	void UpdateInternal();
	void PrepareFrame();
	void EndFrame();
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);