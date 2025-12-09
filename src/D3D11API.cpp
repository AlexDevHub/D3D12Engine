//
// Created by xande on 6/9/2025.
//

#include "D3D11API.h"
// #include <vector>

#define IID_PPV_ARGS(ppType) __uuidof(**(ppType)), IID_PPV_ARGS_Helper(ppType)


HRESULT D3D11Engine::D3D11API::Init() {
    return S_OK;
}

HRESULT D3D11Engine::D3D11API::Init(int screen_width, int screen_height, bool vsync, HWND hwnd, bool is_fullscreen, float screen_depth, float screen_near) {

    // Store the vsync setting
    m_vsync_enabled = vsync;

    // Create a DirectX graphics interface factory.
    ComPtr<IDXGIFactory> factory;
    RETURN_FAIL_IF_FAILED(CreateDXGIFactory(IID_PPV_ARGS(&factory)));

    // Use the factory to create an adapter for the primary graphics interface (video card).
    ComPtr<IDXGIAdapter> adapter;
    RETURN_FAIL_IF_FAILED(factory->EnumAdapters(0, &adapter))

    unsigned int numerator = 0;
    unsigned int denominator = 1;

    // Enumerate the primary adapter output (monitor).
    ComPtr<IDXGIOutput> adapterOutput;
    HRESULT res = adapter->EnumOutputs(0, &adapterOutput);
    if (SUCCEEDED(res)) {
        // Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
        unsigned int numModes;
        RETURN_FAIL_IF_FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr))

        // Create a list to hold all the possible display modes for this monitor/video card combination.
        DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
        RETURN_FALSE_IF_NULL(displayModeList)

        // Now fill the display mode list structures.
        RETURN_FAIL_IF_FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList))

        // Now go through all the display modes and find the one that matches the screen width and height.
        // When a match is found, store the numerator and denominator of the refresh rate for that monitor.
        for(unsigned int i = 0; i < numModes; ++i)
        {
            if(displayModeList[i].Width == static_cast<unsigned int>(screen_width) &&
               displayModeList[i].Height == static_cast<unsigned int>(screen_height))
            {
                numerator = displayModeList[i].RefreshRate.Numerator;
                denominator = displayModeList[i].RefreshRate.Denominator;
                break;
            }
        }
        delete [] displayModeList;
    }


    // Get the adapter (video card) description
    DXGI_ADAPTER_DESC adapterDesc;
    RETURN_FAIL_IF_FAILED(adapter->GetDesc(&adapterDesc));

    // Store the dedicated video card memory in bytes.
    m_videoCardMemory = adapterDesc.DedicatedVideoMemory;

    // Convert the name of the video card to a character array and store it.
    m_videoCardDescription = std::move(std::wstring(adapterDesc.Description));

    // Release the display mode list.

    // Initialize the swap chain description.
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

    // Set to a single back buffer.
    swapChainDesc.BufferCount = 1;

    // Set the width and height of the back buffer.
    swapChainDesc.BufferDesc.Width = screen_width;
    swapChainDesc.BufferDesc.Height = screen_height;

    // Set regular 32-bit surface for the back buffer.
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // Set the refresh rate of the back buffer.
    swapChainDesc.BufferDesc.RefreshRate.Numerator = m_vsync_enabled ? numerator : 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = m_vsync_enabled ? denominator : 1;

    // Set the usage of the back buffer.
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    // Set the handle for the window to render to.
    swapChainDesc.OutputWindow = hwnd;

    // Turn multisampling off.
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    // Set to full screen or windowed mode.
    swapChainDesc.Windowed = !is_fullscreen;

    // Set the scan line ordering and scaling to unspecified.
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // Discard the back buffer contents after presenting.
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // Don't set the advanced flags.
    swapChainDesc.Flags = 0;

    // Set the feature level to DirectX 11.
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    unsigned int flags = 0;

#if defined(_DEBUG)
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // Create the swap chain, Direct3D device, and Direct3D device context.
    RETURN_FAIL_IF_FAILED(
        D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            flags,
            &featureLevel,
            1,
            D3D11_SDK_VERSION,
            &swapChainDesc,
            &m_swapChain,
            &m_device,
            nullptr,
            &m_deviceContext))


    // Get the pointer to the back buffer.
    ComPtr<ID3D11Texture2D> backBufferPtr;
    RETURN_FAIL_IF_FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr));

    // Create the render target view with the back buffer pointer.
    RETURN_FAIL_IF_FAILED(m_device->CreateRenderTargetView(backBufferPtr.Get(), nullptr, &m_renderTargetView));

    // Initialize the description of the depth buffer.
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    // Set up the description of the depth buffer.
    depthBufferDesc.Width = screen_width;
    depthBufferDesc.Height = screen_height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // Create the texture for the depth buffer using the filled out description.
    RETURN_FAIL_IF_FAILED(m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBuffer))

    // Initialize the description of the stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    // Set up the description of the stencil state.
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create the depth stencil state.
    RETURN_FAIL_IF_FAILED(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState))

    // Set the depth stencil state.
    m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);

    // Initialize the depth stencil view.
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    // Set up the depth stencil view description.
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view.
    RETURN_FAIL_IF_FAILED(m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &depthStencilViewDesc, &m_depthStencilView))

    // Bind the render target view and depth stencil buffer to the output render pipeline.
    m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Setup the raster description which will determine how and what polygons will be drawn.
    D3D11_RASTERIZER_DESC rasterDesc;
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    // Create the rasterizer state from the description we just filled out.
    RETURN_FAIL_IF_FAILED(m_device->CreateRasterizerState(&rasterDesc, &m_rasterState))

    // Now set the rasterizer state.
    m_deviceContext->RSSetState(m_rasterState.Get());

    // Setup the viewport for rendering.
    m_viewport.Width = static_cast<float>(screen_width);
    m_viewport.Height = static_cast<float>(screen_height);
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;

    // Create the viewport.
    m_deviceContext->RSSetViewports(1, &m_viewport);

    // Setup the projection matrix.
    float fieldOfView = XM_PI / 4.0f;
    float screenAspect = static_cast<float>(screen_width) / static_cast<float>(screen_height);

    // Create the projection matrix for 3D rendering.
    m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screen_near, screen_depth);

    // Initialize the world matrix to the identity matrix.
    m_worldMatrix = XMMatrixIdentity();

    // Create an orthographic projection matrix for 2D rendering.
    m_orthoMatrix = XMMatrixOrthographicLH(static_cast<float>(screen_width), static_cast<float>(screen_height), screen_near, screen_depth);

    return S_OK;
}

void D3D11Engine::D3D11API::Update() {
}

HRESULT D3D11Engine::D3D11API::Shutdown() {
    // Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
    if(m_swapChain)
    {
        m_swapChain->SetFullscreenState(false, nullptr);
    }
    return S_OK;
}

void D3D11Engine::D3D11API::BeginScene(float red, float green, float blue, float alpha) {
    // Setup the color to clear the buffer to.
    float color[4] = { red, green, blue, alpha };

    // Clear the back buffer.
    m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);

    // Clear the depth buffer.
    m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3D11Engine::D3D11API::EndScene()
{
    // Present the back buffer to the screen since rendering is complete.
    if(m_vsync_enabled)
    {
        // Lock to screen refresh rate.
        m_swapChain->Present(1, 0);
    }
    else
    {
        // Present as fast as possible.
        m_swapChain->Present(0, 0);
    }
}

ID3D11Device* D3D11Engine::D3D11API::GetDevice()
{
    return m_device.Get();
}


ID3D11DeviceContext* D3D11Engine::D3D11API::GetDeviceContext()
{
    return m_deviceContext.Get();
}

void D3D11Engine::D3D11API::GetProjectionMatrix(XMMATRIX& projectionMatrix) const {
    projectionMatrix = m_projectionMatrix;
}


void D3D11Engine::D3D11API::GetWorldMatrix(XMMATRIX& worldMatrix) const {
    worldMatrix = m_worldMatrix;
}


void D3D11Engine::D3D11API::GetOrthoMatrix(XMMATRIX& orthoMatrix) const {
    orthoMatrix = m_orthoMatrix;
}

void D3D11Engine::D3D11API::GetVideoCardInfo(std::wstring& card_name, int& memory) const {
    card_name = m_videoCardDescription;
    memory = m_videoCardMemory;
}

void D3D11Engine::D3D11API::SetBackBufferRenderTarget() {
    // Bind the render target view and depth stencil buffer to the output render pipeline.
    m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
}


void D3D11Engine::D3D11API::ResetViewport() {
    // Set the viewport.
    m_deviceContext->RSSetViewports(1, &m_viewport);
}