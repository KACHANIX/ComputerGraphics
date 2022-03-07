#include "Game.h"

#include <iostream> 

#include "InputDevice.h"

Game* Game::instance = nullptr;


Game::Game(LPCWSTR window_name)
{
	name = window_name;
	instance = this;
}
void Game::Initialize()
{

}

void Game::RestoreTargets()
{
	context->OMSetRenderTargets(1, &render_view, depth_view);
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(display->ClientWidth);
	viewport.Height = static_cast<float>(display->ClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);
}

Game::~Game()
{
	DestroyResources();
}

void Game::Run(int window_width, int window_height)
{
	display = new DisplayWin32(name, window_width, window_height, WndProc);
	if (!display->hWnd)
	{
		DestroyResources();
		return;
	}
	PrepareResources();
	Initialize();
	for (GameComponent* component : components)
	{
		component->Initialize();
	}

	start_time = new std::chrono::time_point<std::chrono::steady_clock>();
	prev_time = new std::chrono::time_point<std::chrono::steady_clock>();
	*start_time = std::chrono::steady_clock::now();
	*prev_time = *start_time; 

	MSG msg = {};

	while (!is_exit_requested)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		UpdateInternal();
	}
	delete start_time;
	delete prev_time;
	DestroyResources();
}

void Game::Exit()
{
	is_exit_requested = true;
}

void Game::Update(float delta_time)
{
	for (GameComponent* component : components)
	{
		component->Update(delta_time);
	}
}

void Game::Draw(float delta_time)
{
	for (GameComponent* component : components)
	{
		component->Draw(delta_time);
	}
}

void Game::PostDraw(float delta_time)
{
}

void Game::CreateBackBuffer()
{

	if (back_buffer != nullptr) back_buffer->Release();
	if (render_view != nullptr) render_view->Release();
	if (depth_buffer != nullptr) depth_buffer->Release();
	if (depth_view != nullptr) depth_view->Release();
	if (render_SRV != nullptr) render_SRV->Release();

	HRESULT result = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);
	result = device->CreateRenderTargetView(back_buffer, nullptr, &render_view);

}

void Game::PrepareFrame()
{

	context->ClearState();

	context->OMSetRenderTargets(1, &render_view, depth_view); //depth nahui, 0 instead of 1

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(display->ClientWidth);
	viewport.Height = static_cast<float>(display->ClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);
	context->RSSetState(rast_state);


	float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	context->ClearRenderTargetView(render_view, color);

}

void Game::EndFrame()
{
	swap_chain->Present(1, 0);
}

void Game::UpdateInternal()
{
	auto current_time = std::chrono::steady_clock::now();
	float delta_time = std::chrono::duration_cast<std::chrono::microseconds>(current_time - *prev_time).count() / 1000000.0f;
	*prev_time = current_time;
	total_time += delta_time;

	Update(delta_time);
	PrepareFrame();
	Draw(delta_time);
	PostDraw(delta_time);
	EndFrame();
}

void Game::DestroyResources()
{

}
void Game::PrepareResources()
{
	HRESULT res;
	DXGI_SWAP_CHAIN_DESC swap_desc = {};
	swap_desc.BufferCount = 2;
	swap_desc.BufferDesc.Width = display->ClientWidth;
	swap_desc.BufferDesc.Height = display->ClientHeight;
	swap_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_desc.OutputWindow = display->hWnd;
	swap_desc.Windowed = true;
	swap_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swap_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swap_desc.SampleDesc.Count = 1;
	swap_desc.SampleDesc.Quality = 0;

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };
	res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swap_desc,
		&swap_chain,
		&device,
		nullptr,
		&context);

	CreateBackBuffer();

	this->input_device = new InputDevice(this);
}

LRESULT Game::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		//case WM_KEYDOWN:
		//{
		//	//std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;
		//	auto key = static_cast<unsigned int>(wparam);
		//	if (key == 27)
		//	{
		//		PostQuitMessage(0);
		//		Exit();
		//	}
		//	input_device->AddPressedKey(static_cast<Keys>(key));
		//	break;
		//}
		//case WM_KEYUP:
		//{
		//	auto key = static_cast<unsigned int>(wparam);

		//	input_device->RemovePressedKey(static_cast<Keys>(key));
		//	break;
		//}
	case WM_CLOSE:
	{
		Exit();
		break;
	}
	case WM_INPUT:
	{
		UINT dwSize = 0;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == nullptr) {
			return 0;
		}

		if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

		RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			instance->input_device->OnKeyDown({
				raw->data.keyboard.MakeCode,
				raw->data.keyboard.Flags,
				raw->data.keyboard.VKey,
				raw->data.keyboard.Message
				});
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			instance->input_device->OnMouseMove({
								raw->data.mouse.usFlags,
				raw->data.mouse.usButtonFlags,
				static_cast<int>(raw->data.mouse.ulExtraInformation),
				static_cast<int>(raw->data.mouse.ulRawButtons),
				static_cast<short>(raw->data.mouse.usButtonData),
				raw->data.mouse.lLastX,
				raw->data.mouse.lLastY
				});
		}
		delete[] lpb;
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return Game::instance->MessageHandler(hwnd, umessage, wparam, lparam);
}