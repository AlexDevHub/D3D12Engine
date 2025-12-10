//
// Created by xande on 6/9/2025.
//

#include "D3D12API.h"
// #include <vector>

#define IID_PPV_ARGS(ppType) __uuidof(**(ppType)), IID_PPV_ARGS_Helper(ppType)


HRESULT D3D12Engine::D3D12API::Init() {
    return S_OK;
}

HRESULT D3D12Engine::D3D12API::Init(int screen_width, int screen_height, bool vsync, bool is_fullscreen, float screen_depth, float screen_near) {

    // Store the vsync setting
    m_vsync_enabled = vsync;

    LoadPipeline();

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
    flags |= D3D12_CREATE_DEVICE_DEBUG;
#endif

    // Create the swap chain, Direct3D device, and Direct3D device context.
    RETURN_FAIL_IF_FAILED(
        D3D12CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            flags,
            &featureLevel,
            1,
            D3D12_SDK_VERSION,
            &swapChainDesc,
            &m_swapChain,
            &m_device,
            nullptr,
            &m_deviceContext))


    // Get the pointer to the back buffer.
    ComPtr<ID3D12Texture2D> backBufferPtr;
    RETURN_FAIL_IF_FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D12Texture2D), (LPVOID*)&backBufferPtr));

    // Create the render target view with the back buffer pointer.
    RETURN_FAIL_IF_FAILED(m_device->CreateRenderTargetView(backBufferPtr.Get(), nullptr, &m_renderTargetView));

    // Initialize the description of the depth buffer.
    D3D12_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    // Set up the description of the depth buffer.
    depthBufferDesc.Width = screen_width;
    depthBufferDesc.Height = screen_height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D12_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D12_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // Create the texture for the depth buffer using the filled out description.
    RETURN_FAIL_IF_FAILED(m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBuffer))

    // Initialize the description of the stencil state.
    D3D12_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    // Set up the description of the stencil state.
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D12_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_ALWAYS;

    // Create the depth stencil state.
    RETURN_FAIL_IF_FAILED(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState))

    // Set the depth stencil state.
    m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);

    // Initialize the depth stencil view.
    D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    // Set up the depth stencil view description.
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view.
    RETURN_FAIL_IF_FAILED(m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &depthStencilViewDesc, &m_depthStencilView))

    // Bind the render target view and depth stencil buffer to the output render pipeline.
    m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Setup the raster description which will determine how and what polygons will be drawn.
    D3D12_RASTERIZER_DESC rasterDesc;
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D12_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D12_FILL_SOLID;
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

void D3D12Engine::D3D12API::Update() {
}

HRESULT D3D12Engine::D3D12API::Shutdown() {
    // Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
    if(m_swapChain)
    {
        m_swapChain->SetFullscreenState(false, nullptr);
    }
    return S_OK;
}

HRESULT D3D12Engine::D3D12API::LoadPipeline(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear) {
    UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
    // Enable the debug layer (requires the Graphics Tools "optional feature").
    // NOTE: Enabling the debug layer after device creation will invalidate the active device.
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();

            // Enable additional debug layers.
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
#endif

    // Create a DirectX graphics interface factory.
    ComPtr<IDXGIFactory4> factory;
    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

    if (g_UseWarp)
    {
        ComPtr<IDXGIAdapter> warpAdapter;
        ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

        ThrowIfFailed(D3D12CreateDevice(
            warpAdapter.Get(),
            D3D_FEATURE_LEVEL_12_2,
            IID_PPV_ARGS(&m_device)
        ));
    }
    else
    {
        ComPtr<IDXGIAdapter1> hardwareAdapter;
        GetHardwareAdapter(factory.Get(), &hardwareAdapter);

        ThrowIfFailed(D3D12CreateDevice(
            hardwareAdapter.Get(),
            D3D_FEATURE_LEVEL_12_2,
            IID_PPV_ARGS(&m_device)
        ));
    }

    // Describe and create the command queue.
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&g_commandQueue)));

    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = g_NumFrames;
    swapChainDesc.Width = screenWidth;
    swapChainDesc.Height = screenHeight;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain1;
    ThrowIfFailed(factory->CreateSwapChainForHwnd(
        m_commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
        hwnd,
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain1
    ));

    ThrowIfFailed(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));

    ThrowIfFailed(swapChain1.As(&m_swapChain));
    m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

    // Create descriptor heaps.
    {
        // Describe and create a render target view (RTV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = FrameCount;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

        m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }


}

void D3D12Engine::D3D12API::BeginScene(float red, float green, float blue, float alpha) {
    // Setup the color to clear the buffer to.
    float color[4] = { red, green, blue, alpha };

    // Clear the back buffer.
    m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);

    // Clear the depth buffer.
    m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D12_CLEAR_DEPTH, 1.0f, 0);
}

void D3D12Engine::D3D12API::EndScene()
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

ID3D12Device* D3D12Engine::D3D12API::GetDevice()
{
    return m_device.Get();
}


ID3D12DeviceContext* D3D12Engine::D3D12API::GetDeviceContext()
{
    return m_deviceContext.Get();
}

void D3D12Engine::D3D12API::GetProjectionMatrix(XMMATRIX& projectionMatrix) const {
    projectionMatrix = m_projectionMatrix;
}


void D3D12Engine::D3D12API::GetWorldMatrix(XMMATRIX& worldMatrix) const {
    worldMatrix = m_worldMatrix;
}


void D3D12Engine::D3D12API::GetOrthoMatrix(XMMATRIX& orthoMatrix) const {
    orthoMatrix = m_orthoMatrix;
}

void D3D12Engine::D3D12API::GetVideoCardInfo(std::wstring& card_name, int& memory) const {
    card_name = m_videoCardDescription;
    memory = m_videoCardMemory;
}

void D3D12Engine::D3D12API::SetBackBufferRenderTarget() {
    // Bind the render target view and depth stencil buffer to the output render pipeline.
    m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
}


void D3D12Engine::D3D12API::ResetViewport() {
    // Set the viewport.
    m_deviceContext->RSSetViewports(1, &m_viewport);
}
