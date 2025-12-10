//
// Created by xande on 6/9/2025.
//

#ifndef D3D12API_H
#define D3D12API_H

#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <D3D12.h>
#include <directxmath.h>
#include <string>
#include <wrl.h>

#include "System.h"

using namespace DirectX;
using namespace Microsoft::WRL;

namespace D3D12Engine {
class D3D12API : public System {
public:

    HRESULT Init() override;
    HRESULT Init(int screenWidth, int screenHeight, bool vsync, bool fullscreen, float screenDepth, float screenNear);
    void Update() override;
    HRESULT Shutdown() override;

    void LoadPipeline(int screenWidth, int screenHeight, HWND hwnd);

    void BeginScene(float red, float green, float blue, float alpha);
    void EndScene();


    void GetProjectionMatrix(XMMATRIX &projectionMatrix) const;
    void GetWorldMatrix(XMMATRIX &worldMatrix) const;
    void GetOrthoMatrix(XMMATRIX &orthoMatrix) const;
    void GetVideoCardInfo(std::wstring& card_name, int &memory) const;

    void SetBackBufferRenderTarget();
    void ResetViewport();

    ID3D12Device* GetDevice();

    inline static uint32_t g_ClientWidth = 640;
    inline static uint32_t g_ClientHeight = 480;

private:

    // The number of swap chain back buffers.
    inline static const uint8_t g_NumFrames = 3;
    // Use WARP adapter
    inline static bool g_UseWarp = false;

    // Set to true once the DX12 objects have been initialized.
    bool g_IsInitialized = false;

    bool m_vsync_enabled = false;
    size_t m_videoCardMemory = 0;
    std::wstring m_videoCardDescription;
    ComPtr<IDXGISwapChain> m_swapChain = nullptr;
    ComPtr<ID3D12Device> m_device = nullptr;

    // DirectX 12 Objects
    ComPtr<ID3D12Device2> m_device;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    ComPtr<IDXGISwapChain4> m_swapChain;
    ComPtr<ID3D12Resource> m_backBuffers[g_NumFrames];
    ComPtr<ID3D12GraphicsCommandList> m_commandList;
    ComPtr<ID3D12CommandAllocator> m_commandAllocators[g_NumFrames];
    ComPtr<ID3D12DescriptorHeap> m_RTVDescriptorHeap;
    UINT m_RTVDescriptorSize;
    UINT m_currentBackBufferIndex;

    XMMATRIX m_projectionMatrix = {};
    XMMATRIX m_worldMatrix = {};
    XMMATRIX m_orthoMatrix = {};
    D3D12_VIEWPORT m_viewport = {};
};
}


#endif //D3D12API_H
